#include <fstream>
#include <iostream>
#include <sstream>
#include "../_debug/_DebugConOut.h"
#include "NetWark.h"
#include "HostState.h"
#include "GestState.h"
#include "../TileLoader.h"

std::unique_ptr<NetWark, NetWark::NetWorkDeleter> NetWark::s_Instance(new NetWark());

NetWark::NetWark()
{
	Init();
}

NetWark::~NetWark()
{
	if (updata_.joinable())
	{
		updata_.join();
	}
}

void NetWark::RunUpdata(void)
{
	updata_ = std::thread(&NetWark::Update, this);
	updata_.detach();
}

void NetWark::Update(void)
{
	while (!ProcessMessage())
	{
		if (!netState_)
		{
			continue;
		}
		if (netState_->Update())
		{
			break;
		}
	}

	MesHeader recvHeader = { MesType::Non, 0, 0, 0 };
	MesPacket recvPacket;
	recvPacket.reserve(200);
	unsigned int writePos;
	int guestCount = 0;
	std::map<MesType, std::function<void(void)>> netFunc;

	// ゲスト
	netFunc.emplace(MesType::Count_Down_Room, [&]() {
		if (!connectFlag_)
		{
			std::lock_guard<std::mutex> mut(mtx_);
			TimeUnion data{};
			data.data[0] = recvPacket[0].iData;
			data.data[1] = recvPacket[1].iData;
			countStartTime_ = data.time;
			connectFlag_ = true;
			TRACE("Count_Down_Room受け取り\n");
		}
		else
		{
			TRACE("不正な時間値です:Count_Down_Room\n");
		}
	});

	// ゲスト
	netFunc.emplace(MesType::ID, [&]() {
		if (!(recvPacket[0].iData % UNIT_ID_NUM))
		{
			if (recvPacket[0].iData / UNIT_ID_NUM < recvPacket[1].iData)
			{
				std::lock_guard<std::mutex> mut(mtx_);
				playerInf_.first = recvPacket[0].iData;
				playerInf_.second = recvPacket[1].iData;
				return;
			}
		}
		TRACE("不正なPlayerIDです。ID:%d, PlayerMax:%d\n", recvPacket[0].iData, recvPacket[1].iData);
	});

	// ゲスト
	netFunc.emplace(MesType::TMX_Size, [&]() {
		std::lock_guard<std::mutex> mut(mtx_);
		revBox_.resize((recvPacket[0].cData[0] * recvPacket[0].cData[1]) / BIT_NUM + 1);
	});

	// ゲスト
	netFunc.emplace(MesType::TMX_Data, [&]() {
		// 送られてきたデータを格納しやすいように
		std::lock_guard<std::mutex> mut(mtx_);
		revBox_ = std::move(recvPacket);
		MakeTmx(revBox_);
	});

	// ゲスト
	netFunc.emplace(MesType::Stanby_Host, [&]() {
		// 送信時間
		std::lock_guard<std::mutex> mut(mtx_);
		end = std::chrono::system_clock::now();
		double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		netState_->SetActiveState(ActiveState::Play);
	});

	// ゲスト
	netFunc.emplace(MesType::Count_Down_Game, [&]() {
		if (netState_->GetActiveState() == ActiveState::Play)
		{
			std::lock_guard<std::mutex> mut(mtx_);
			TimeUnion data{};
			data.data[0] = recvPacket[0].iData;
			data.data[1] = recvPacket[1].iData;
			gameStartTime_ = data.time;
			startState_ = StartState::Countdown;
			TRACE("Count_Down_Game受け取り\n");
		}
		else
		{
			TRACE("不正な時間値です:Count_Down_Game\n");
		}
	});

	// ホスト
	netFunc.emplace(MesType::Stanby_Guest, [&]() {
		if (netState_->GetMode() == NetWorkMode::Host)
		{
			guestCount++;
			if (guestCount == netState_->GetNetHandle().size())
			{
				gameStartTime_ = lpSceneMng.GetTime();
				startState_ = StartState::Countdown;
				netState_->SetActiveState(ActiveState::Play);
				TRACE("ゲームスタート情報受信\n");
			}
		}
	});

	netFunc.emplace(MesType::Pos, [&]() {
		if (recvPacket.size() > 4)
		{
			return;
		}
		if ((recvPacket[0].iData / UNIT_ID_NUM) < playerMesList_.size())
		{
			std::lock_guard<std::mutex> lock(playerMesList_[recvPacket[0].iData / UNIT_ID_NUM].second);
			playerMesList_[recvPacket[0].iData / UNIT_ID_NUM].first.emplace_back(MesPair{ recvHeader.type, recvPacket });
		}
		else
		{
			TRACE("PlayerIDが不正です。:ID%d\n", recvPacket[0].iData);
		}
	});

	netFunc.emplace(MesType::Set_Bomb, [&]() {
		if (recvPacket.size() > 7)
		{
			return;
		}
		if ((recvPacket[0].iData / UNIT_ID_NUM) < playerMesList_.size())
		{
			if (recvPacket[1].iData % UNIT_ID_NUM)
			{
				std::lock_guard<std::mutex> lock(playerMesList_[recvPacket[0].iData / UNIT_ID_NUM].second);
				playerMesList_[recvPacket[0].iData / UNIT_ID_NUM].first.emplace_back(MesPair{ recvHeader.type, recvPacket });
			}
			else
			{
				TRACE("bombIDが不正です。:ID%d\n", recvPacket[1].iData);
			}
		}
		else
		{
			TRACE("PlayerIDが不正です。:ID%d\n", recvPacket[0].iData);
		}
	});

	netFunc.emplace(MesType::Deth, [&]() {
		if (recvPacket.size() > 1)
		{
			return;
		}
		if ((recvPacket[0].iData / UNIT_ID_NUM) < playerMesList_.size())
		{
			std::lock_guard<std::mutex> lock(playerMesList_[recvPacket[0].iData / UNIT_ID_NUM].second);
			playerMesList_[recvPacket[0].iData / UNIT_ID_NUM].first.emplace_back(MesPair{ recvHeader.type, recvPacket });
		}
		else
		{
			TRACE("PlayerIDが不正です。:ID%d\n", recvPacket[0].iData);
		}
	});

	netFunc.emplace(MesType::Lost, [&]() {
		if (recvPacket.size() > 1)
		{
			return;
		}
		if ((recvPacket[0].iData / UNIT_ID_NUM) < playerMesList_.size())
		{
			std::lock_guard<std::mutex> lock(playerMesList_[recvPacket[0].iData / UNIT_ID_NUM].second);
			playerMesList_[recvPacket[0].iData / UNIT_ID_NUM].first.emplace_back(MesPair{ recvHeader.type, recvPacket });
		}
		else
		{
			TRACE("PlayerIDが不正です。:ID%d\n", recvPacket[0].iData);
		}
	});

	while (!ProcessMessage() && netState_->GetNetHandle().size())
	{
		if (netState_->Update())
		{
			auto& handle = netState_->GetNetHandle();
			for (auto& data : handle)
			{
				if (GetNetWorkDataLength(data.handle) >= sizeof(MesHeader))
				{
					if (!recvHeader.next)
					{
						writePos = 0;
					}

					NetWorkRecv(data.handle, &recvHeader, sizeof(MesHeader));

					if (recvHeader.length)
					{
						recvPacket.resize(writePos + recvHeader.length);
						NetWorkRecv(data.handle, recvPacket.data() + writePos, recvHeader.length * sizeof(unionData));
						writePos = static_cast<unsigned int>(recvPacket.size());
					}
					// nextがあるかどうか
					if (recvHeader.next)
					{
						TRACE("追加情報アリ\n");
						continue;
					}
					SendMesAll(recvPacket, recvHeader.type, data.handle);
					(netFunc[recvHeader.type])();
				}
			}
		}
	}

	// 切断された場合
	InitCloseNetWork();
}

void NetWark::InitCloseNetWork(void)
{
	netState_.reset();
	std::lock_guard<std::mutex> mut(revStanbyMtx_);
	revStanby_ = false;
}

void NetWark::AddMesList(int id, MesPacketList& list, std::mutex& mutex)
{
	playerMesList_.emplace_back(std::pair<MesPacketList&, std::mutex&>( list, mutex ));
}

bool NetWark::SendMesAll(MesPacket packet, MesType type, int handle)
{
	if (!netState_)
	{
		return false;
	}

	Header header{ type, 0, 0, 0 };
	packet.insert(packet.begin(), { header.data[1] });
	packet.insert(packet.begin(), { header.data[0] });

	auto list = netState_->GetNetHandle();
	for (auto& data : list)
	{
		if (data.handle == handle)
		{
			continue;
		}
		MesPacket tmpPacket = packet;
		do
		{
			unsigned int sendSize = static_cast<unsigned int>((intSendCnt_ < tmpPacket.size() ? intSendCnt_ : tmpPacket.size()));

			tmpPacket[1].iData = sendSize - BIT_NUM;

			if (sendSize == tmpPacket.size())
			{
				header.mes.next = 0;
			}
			else
			{
				header.mes.next = 1;
			}

			tmpPacket[0].iData = header.data[0];
			NetWorkSend(data.handle, tmpPacket.data(), sendSize * sizeof(tmpPacket[0]));

			header.mes.sendID++;
			tmpPacket.erase(tmpPacket.begin() + BIT_NUM, tmpPacket.begin() + sendSize);
		} while (tmpPacket.size() > BIT_NUM);
	}
	return true;
}

bool NetWark::SendMes(MesPacket& packet, MesType type, int handle)
{
	if (!netState_)
	{
		return false;
	}

	Header header{ type, 0, 0, 0 };
	packet.insert(packet.begin(), { header.data[1] });
	packet.insert(packet.begin(), { header.data[0] });

	auto list = netState_->GetNetHandle();
	do
	{
		unsigned int sendSize = static_cast<unsigned int>((intSendCnt_ < packet.size() ? intSendCnt_ : packet.size()));

		packet[1].iData = sendSize - BIT_NUM;

		if (sendSize == packet.size())
		{
			header.mes.next = 0;
		}
		else
		{
			header.mes.next = 1;
		}

		packet[0].iData = header.data[0];
		if (list.size())
		{
			NetWorkSend(handle , packet.data(), sendSize * sizeof(packet[0]));
		}
		header.mes.sendID++;
		packet.erase(packet.begin() + BIT_NUM, packet.begin() + sendSize);
	} while (packet.size() > BIT_NUM);

	return true;
}

bool NetWark::SendMes(MesPacket& packet, MesType type)
{
	if (!netState_)
	{
		return false;
	}

	Header header{ type, 0, 0, 0 };
	packet.insert(packet.begin(), { header.data[1] });
	packet.insert(packet.begin(), { header.data[0] });

	auto list = netState_->GetNetHandle();
	do
	{
		unsigned int sendSize = static_cast<unsigned int>((intSendCnt_ < packet.size() ? intSendCnt_ : packet.size()));

		packet[1].iData = sendSize - BIT_NUM;

		if (sendSize == packet.size())
		{
			header.mes.next = 0;
		}
		else
		{
			header.mes.next = 1;
		}

		packet[0].iData = header.data[0];
		if (list.size())
		{
			NetWorkSend(list.back().handle, packet.data(), sendSize * sizeof(packet[0]));
		}
		header.mes.sendID++;
		packet.erase(packet.begin() + BIT_NUM, packet.begin() + sendSize);
	} 
	while (packet.size() > BIT_NUM);

	return true;
}

bool NetWark::SendMes(MesType type)
{
	if (!netState_)
	{
		return false;
	}

	MesPacket data;
	SendMes(data, type);

	return true;
}

void NetWark::SendStanby(void)
{
	if (!netState_)
	{
		return;
	}

	MesPacket data;
	SendMesAll(data, MesType::Stanby_Host, -1);
	netState_->SetActiveState(ActiveState::Stanby);
}

void NetWark::SendStart(void)
{
	if (!netState_)
	{
		return;
	}
	SendMes(MesType::Stanby_Guest);
}

void NetWark::SendCountRoom(void)
{
	if (!netState_)
	{
		return;
	}
	MesPacket data;
	data.resize(2);
	TimeUnion time{};
	time.time = countStartTime_;
	data[0].iData = time.data[0];
	data[1].iData = time.data[1];
	SendMes(data, MesType::Count_Down_Room);
}

void NetWark::SendCountGame(void)
{
	if (!netState_)
	{
		return;
	}
	MesPacket data;
	data.resize(2);
	TimeUnion time{};
	time.time = gameStartTime_;
	data[0].iData = time.data[0];
	data[1].iData = time.data[1];
	SendMesAll(std::move(data), MesType::Count_Down_Game, 0);
}

void NetWark::SendPlayerID(void)
{
	if (!netState_)
	{
		return;
	}
	auto list = netState_->GetNetHandle();
	MesPacket sendData;
	sendData.resize(2);
	for (auto& data : list)
	{
		sendData[0].uiData = data.playerID;
		sendData[1].iData = playerInf_.second;
		SendMes(sendData, MesType::ID, data.handle);
	}
}

bool NetWark::SetNetWorkMode(NetWorkMode mode)
{
	switch (mode)
	{
	case NetWorkMode::Offline:
		netState_ = std::make_unique<NetWorkState>();
		break;
	case NetWorkMode::Host:
		netState_ = std::make_unique<HostState>();
		RunUpdata();
		break;
	case NetWorkMode::Guest:
		netState_ = std::make_unique<GestState>();
		RunUpdata();
		break;
	default:
		netState_ = std::make_unique<NetWorkState>();
		AST();
		break;
	}
	return true;
}

NetWorkMode NetWark::GetNetWorkMode(void)
{
	if (!netState_)
	{
		return NetWorkMode::Non;
	}
	return netState_->GetMode();
}

ActiveState NetWark::GetActive(void)
{
	if (!netState_)
	{
		return ActiveState::Non;
	}
	return netState_->GetActiveState();;
}

int NetWark::GetNetHandleList(void)
{
	if (!netState_)
	{
		return -1;
	}

	if (netState_->GetNetHandle().size())
	{
		return netState_->GetNetHandle().front().handle;
	}
	else
	{
		return -1;
	}
}

ActiveState NetWark::ConnectHost(IPDATA hostIP)
{
	if (!netState_)
	{
		return ActiveState::Non;
	}
	return netState_->ConnectHost(hostIP);
}

ArrayIP NetWark::GetIP(void)
{
	GetMyIPAddress(&ipData_[0], 5);
	return ipData_;
}

PairInt NetWark::GetPlayerInf(void)
{
	return playerInf_;
}

void NetWark::SetPlayerInf(int max)
{
	playerInf_.second = max;
}

time_point NetWark::GetConnectTime(void)
{
	return countStartTime_;
}

void NetWark::SetConnectTime(time_point time)
{
	countStartTime_ = time;
}

bool NetWark::GetConnectFlag(void)
{
	return connectFlag_;
}

void NetWark::SetConnectFlag(bool flag)
{
	connectFlag_ = flag;
}

time_point NetWark::GetStartTime(void)
{
	return gameStartTime_;
}

StartState NetWark::GetStartState(void)
{
	return startState_;
}

void NetWark::SetStartState(StartState state)
{
	startState_ = state;
}

bool NetWark::Init(void)
{
	revStanby_ = false;
	connectFlag_ = false;
	ipData_ = { 0 };
	intSendCnt_ = 0;
	startState_ = StartState::Wait;
	playerInf_ = { 0, 0 };
	// ﾌｧｲﾙから送るバイト数を読み込む
	std::ifstream ifp("ini/setting.txt");
	std::string strLine;
	while (!ifp.eof())
	{
		std::getline(ifp, strLine);
		if (strLine.find("byte length") != std::string::npos)
		{
			auto byteCnt = strLine.substr(strLine.find_first_of('"') + 1, strLine.find_last_of('"'));
			intSendCnt_ = std::atoi(byteCnt.c_str()) / sizeof(unionData);
		}
	}

	return true;
}

void NetWark::MakeTmx(MesPacket tmxData)
{
	std::ifstream ifp("cash/TmxHeader.tmx");
	std::ofstream ofp("cash/RevData.tmx");
	if ((!ifp) || (!ofp))
	{
		AST();
		return;
	}
	std::string stringLine;

	do
	{
		std::getline(ifp, stringLine);
		if (ifp.eof())
		{
			break;
		}
		ofp << stringLine << std::endl;
	} while (stringLine.find("data encoding") == std::string::npos);

	// 何文字入れたか
	unionData unionData = { 0 };
	int strCnt = 0;

	for (auto& data : tmxData)
	{
		unionData = data;
		for (int byteCnt = 0; byteCnt < 8; byteCnt++)
		{
			if (ifp.eof())
			{
				break;
			}
			std::ostringstream stream;
			stream << ((unionData.cData[byteCnt / BIT_NUM] >> (INT_BYTE_CNT * (byteCnt % BIT_NUM))) & 0x0f);
			ofp << stream.str();
			strCnt++;
			if (strCnt % 21 != 0)
			{
				ofp << ',';
			}
			else
			{
				if ((strCnt % (21 * 17)) != 0)
				{
					ofp << ',' << std::endl;
				}
				else
				{
					ofp << std::endl;
					do
					{
						std::getline(ifp, stringLine);
						if (ifp.eof())
						{
							TRACE("初期化情報取得\n");
							break;
						}
						ofp << stringLine << std::endl;
					} while (stringLine.find("data encoding") == std::string::npos);
				}
			}
		}
	}
}
