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
	MesHeader recvHeader = { MesType::Non, 0, 0, 0 };
	MesPacket recvPacket;
	unsigned int writePos;

	while(!ProcessMessage())
	{
		if (!netState_)
		{
			continue;
		}

		if (netState_->Update())
		{
			if (netState_->GetActiveState() != ActiveState::Play)
			{
				if (netState_->GetMode() == NetWorkMode::Host)
				{
					// ｹﾞｰﾑｽﾀｰﾄ情報取得
					if (netState_->GetActiveState() == ActiveState::Stanby)
					{
						auto handle = netState_->GetNetHandle();
						MesHeader recvData;
						if (GetNetWorkDataLength(handle) >= sizeof(MesHeader))
						{
							NetWorkRecv(handle, &recvData, sizeof(MesHeader));
							if (recvData.type == MesType::Game_Start)
							{
								netState_->SetActiveState(ActiveState::Play);
								TRACE("ゲームスタート情報受信\n");
								continue;
							}
						}
					}
				}

				if (netState_->GetMode() == NetWorkMode::Guest)
				{
					if (revStanby_)
					{
						netState_->SetActiveState(ActiveState::Play);
						continue;
					}
					else
					{
						auto handle = netState_->GetNetHandle();
						if (GetNetWorkDataLength(handle) >= sizeof(MesHeader))
						{
							if (!recvHeader.next)
							{
								recvPacket.clear();
								writePos = 0;
							}

							NetWorkRecv(handle, &recvHeader, sizeof(MesHeader));

							if (recvHeader.length)
							{
								recvPacket.resize(recvPacket.size() + recvHeader.length);
								NetWorkRecv(handle, recvPacket.data() + writePos, recvHeader.length * sizeof(int));
								writePos = static_cast<unsigned int>(recvPacket.size());
							}
							// nextがあるかどうか
							if (recvHeader.next)
							{
								TRACE("追加情報アリ\n");
								continue;
							}

							if (recvHeader.type == MesType::TMX_Size)
							{
								std::lock_guard<std::mutex> mut(mtx_);
								revBox_.resize(recvPacket[0].iData);
								//TRACE("1回で受け取るファイルの大きさ:%d\n", revBox_.size());
								continue;
							}
							if (recvHeader.type == MesType::TMX_Data)
							{
								// 送られてきたデータを格納しやすいように
								std::lock_guard<std::mutex> mut(mtx_);
								revBox_ = std::move(recvPacket);
								start = std::chrono::system_clock::now();
								continue;
							}
							if (recvHeader.type == MesType::Stanby)
							{
								// 送信時間
								std::lock_guard<std::mutex> mut(mtx_);
								end = std::chrono::system_clock::now();
								double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
								TRACE("かかった時間:%p秒\n", elapsed);

								revStanby_ = true;
								MakeTmx();
								continue;
							}
						}
					}
				}
			}
			else
			{
				auto handle = netState_->GetNetHandle();
				NetWorkRecv(handle, &recvHeader, sizeof(MesHeader));
				if (recvHeader.type == MesType::Instance)
				{
					MesPacket mes;
					mes.resize(recvHeader.length);
					// 来たメッセージに対してlockとunlockをかける
					std::lock_guard<std::mutex> mut(mtx_);
					NetWorkRecv(handle, mes.data(), recvHeader.length * sizeof(int));
					mesList_.emplace_back(recvHeader, mes);
				}
				if (recvHeader.type == MesType::Pos)
				{
					MesPacket mes;
					mes.resize(recvHeader.length);
					// 来たメッセージに対してlockとunlockをかける
					std::lock_guard<std::mutex> mut(mtx_);
					NetWorkRecv(handle, mes.data(), recvHeader.length * sizeof(int));
					mesList_.emplace_back(recvHeader, mes);
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

// 来たメッセージに対してlockとunlockをかける
MesList NetWark::PickMes(void)
{
	std::lock_guard<std::mutex>mut(mtx_);
	MesList mesList;
	mesList.first = { MesType::Non, 0, 0, 0 };
	if (mesList_.size())
	{
		mesList = mesList_.front();
		mesList_.erase(mesList_.begin());
	}
	return mesList;
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

		packet[1].iData = sendSize - 2;

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
		packet.erase(packet.begin() + 2, packet.begin() + sendSize);
	} 
	while (packet.size() > 2);

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

	SendMes(MesType::Stanby);

	netState_->SetActiveState(ActiveState::Stanby);
}

void NetWark::SendStart(void)
{
	if (!netState_)
	{
		return;
	}
	SendMes(MesType::Game_Start);
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
	return netState_->GetMode();
}

ActiveState NetWark::GetActive(void)
{
	return netState_->GetActiveState();;
}

int NetWark::GetNetHandle(void)
{
	return netState_->GetNetHandle();
}

ActiveState NetWark::ConnectHost(IPDATA hostIP)
{
	return netState_->ConnectHost(hostIP);
}

ArrayIP NetWark::GetIP(void)
{
	GetMyIPAddress(&ipData_[0], 5);
	return ipData_;
}

bool NetWark::Init(void)
{
	revStanby_ = false;
	ipData_ = { 0 };
	intSendCnt_ = 0;
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

void NetWark::MakeTmx(void)
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

	for (auto& data : revBox_)
	{
		unionData = data;
		for (int byteCnt = 0; byteCnt < 8; byteCnt++)
		{
			if (ifp.eof())
			{
				break;
			}
			std::ostringstream stream;
			stream << ((unionData.cData[byteCnt / 2] >> (4 * (byteCnt % 2))) & 0x0f);
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
