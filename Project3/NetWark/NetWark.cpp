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
	auto handleChack = [&]() {
		auto& handleList = netState_->GetNetHandle();
		for (auto data : handleList)
		{
			if (data.first <= 0)
			{
				return false;
			}
		}
		return true;
	};
	while (!ProcessMessage())
	{
		if (!netState_)
		{
			continue;
		}
		netState_->Update();
		if (handleChack())
		{
			break;
		}
	}

	MesHeader recvHeader = { MesType::Non, 0, 0, 0 };
	MesPacket recvPacket;
	recvPacket.reserve(200);
	unsigned int writePos;
	std::map<MesType, std::function<void(void)>> netFunc;

	netFunc.emplace(MesType::Count_Down_Room, [&]() {
		std::lock_guard<std::mutex> mut(mtx_);
		TimeUnion data{};
		data.data[0] = recvPacket[0].iData;
		data.data[1] = recvPacket[1].iData;
		countStartTime_ = data.time;
		connectFlag_ = true;
	});

	netFunc.emplace(MesType::ID, [&]() {
		std::lock_guard<std::mutex> mut(mtx_);
		playerInf_.first = recvPacket[0].iData;
		playerInf_.second = recvPacket[1].iData;
	});

	netFunc.emplace(MesType::TMX_Size, [&]() {
		std::lock_guard<std::mutex> mut(mtx_);
		revBox_.resize((recvPacket[0].cData[0] * recvPacket[0].cData[1]) / BIT_NUM + 1);
	});

	netFunc.emplace(MesType::TMX_Data, [&]() {
		// 送られてきたデータを格納しやすいように
		std::lock_guard<std::mutex> mut(mtx_);
		revBox_ = std::move(recvPacket);
		MakeTmx(revBox_);
	});

	netFunc.emplace(MesType::Stanby_Host, [&]() {
		// 送信時間
		std::lock_guard<std::mutex> mut(mtx_);
		end = std::chrono::system_clock::now();
		double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		netState_->SetActiveState(ActiveState::Play);
	});

	netFunc.emplace(MesType::Stanby_Guest, [&]() {
		netState_->SetActiveState(ActiveState::Play);
		TRACE("ゲームスタート情報受信\n");
	});

	netFunc.emplace(MesType::Count_Down_Game, [&]() {
		std::lock_guard<std::mutex> mut(mtx_);
		TimeUnion data{};
		data.data[0] = recvPacket[0].iData;
		data.data[1] = recvPacket[1].iData;
		gameStartTime_ = data.time;
		startState_ = StartState::Countdown;
	});

	netFunc.emplace(MesType::Pos, [&]() {
		std::lock_guard<std::mutex> lock(playerMesList_[recvPacket[0].iData / 5].second);
		playerMesList_[recvPacket[0].iData / 5].first.emplace_back(MesPair{ recvHeader.type, recvPacket });
	});

	netFunc.emplace(MesType::Set_Bomb, [&]() {
		std::lock_guard<std::mutex> lock(playerMesList_[recvPacket[0].iData / 5].second);
		playerMesList_[recvPacket[0].iData / 5].first.emplace_back(MesPair{ recvHeader.type, recvPacket });
		});

	netFunc.emplace(MesType::Deth, [&]() {
		std::lock_guard<std::mutex> lock(playerMesList_[recvPacket[0].iData / 5].second);
		playerMesList_[recvPacket[0].iData / 5].first.emplace_back(MesPair{ recvHeader.type, recvPacket });
	});

	netFunc.emplace(MesType::Lost, [&]() {

	});

	while (!ProcessMessage() && GetLostNetWork() == -1)
	{
		if (netState_->Update())
		{
			auto& handle = netState_->GetNetHandle();
			for (auto data : handle)
			{
				if (GetNetWorkDataLength(data.first) >= sizeof(MesHeader))
				{
					if (!recvHeader.next)
					{
						//recvPacket.clear();
						writePos = 0;
					}

					NetWorkRecv(data.first, &recvHeader, sizeof(MesHeader));

					if (recvHeader.length)
					{
						recvPacket.resize(writePos + recvHeader.length);
						NetWorkRecv(data.first, recvPacket.data() + writePos, recvHeader.length * sizeof(unionData));
						writePos = static_cast<unsigned int>(recvPacket.size());
					}
					// nextがあるかどうか
					if (recvHeader.next)
					{
						TRACE("追加情報アリ\n");
						continue;
					}

					(netFunc[recvHeader.type])();
				}
			}
		}
		else
		{
			// 切断された場合
			InitCloseNetWork();
		}
	}
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

bool NetWark::SendMes(MesPacket& packet, MesType type)
{
	if (!netState_)
	{
		return false;
	}

	Header header{ type, 0, 0, 0 };
	packet.insert(packet.begin(), { header.data[1] });
	packet.insert(packet.begin(), { header.data[0] });

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
		NetWorkSend(GetNetHandle(), packet.data(), sendSize * sizeof(packet[0]));

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

	SendMes(MesType::Stanby_Host);

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

int NetWark::GetNetHandle(void)
{
	if (!netState_)
	{
		return -1;
	}
	if (netState_->GetNetHandle().size())
	{
		return netState_->GetNetHandle().front().first;
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

time_point NetWark::GetConnectTime(void)
{
	return countStartTime_;
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
