#include <fstream>
#include <iostream>
#include "../_debug/_DebugConOut.h"
#include "NetWark.h"
#include "HostState.h"
#include "GestState.h"
#include "../TileLoader.h"

std::unique_ptr<NetWark, NetWark::NetWorkDeleter> NetWark::s_Instance(new NetWark());

NetWark::NetWark()
{
	revStanby = false;
	revCnt_ = 0;
}

NetWark::~NetWark()
{
}

bool NetWark::Update(void)
{
	if (!netState_)
	{
		return false;
	}

	if (netState_->Update())
	{
		if (netState_->GetMode() == NetWorkMode::Host)
		{
			// ｹﾞｰﾑｽﾀｰﾄ情報取得
			if (netState_->GetActiveState() == ActiveState::Stanby)
			{
				auto handle = netState_->GetNetHandle();
				MesData recvData;
				if (GetNetWorkDataLength(handle) >= sizeof(MesData))
				{
					NetWorkRecv(handle, &recvData, sizeof(MesData));
					if (recvData.type == MesType::Game_Start)
					{
						netState_->SetActiveState(ActiveState::Play);
						TRACE("ゲームスタート情報受信\n");
					}
				}
			}
		}

		if (netState_->GetMode() == NetWorkMode::Gest)
		{
			if (revStanby)
			{
				netState_->SetActiveState(ActiveState::Play);
			}
			else
			{
				auto handle = netState_->GetNetHandle();
				MesData recvData;
				if (GetNetWorkDataLength(handle) >= sizeof(MesData))
				{
					NetWorkRecv(handle, &recvData, sizeof(MesData));
					if (recvData.type == MesType::TMX_Size)
					{
						revBox_.resize(recvData.data[0]);
						TRACE("TMXファイルの大きさ:%d\n", revBox_.size());
					}
					if (recvData.type == MesType::TMX_Data)
					{
						revBox_[recvData.data[0]] = recvData.data[1];
						TRACE("%c", revBox_[recvData.data[0]]);
						//if (recvData.data[1] == '\n')
						//{
						//	revBox_[recvData.data[0] + revCnt_] = ' ';
						//	revCnt_++;
						//}
						//revBox_[recvData.data[0] + revCnt_] = static_cast<char>(recvData.data[1]);
						//TRACE("%c", revBox_[recvData.data[0] + revCnt_]);
						//if (recvData.data[0] + revCnt_ + 1 == revBox_.size())
						//{
						//	TRACE("マップ情報書き出し\n");
						//	std::ofstream ofp("cash/MapData.tmx");
						//	for (auto& data : revBox_)
						//	{
						//		
						//		if (data=='\n')
						//		{
						//			ofp << std::endl;
						//		}
						//		else
						//		{
						//			ofp << data;
						//		}
						//		//ofp.write();
						//	}
						//}
					}
					if (recvData.type == MesType::Stanby)
					{
						std::ofstream ofp("cash/MapData.tmx");
						for (auto& data : revBox_)
						{
							if (data != 0)
							{
								char* string = reinterpret_cast<char*>(&data);
								for (int i = 0; i < sizeof(data); i++)
								{
									ofp << string[i];
								}
							}
						}
						revStanby = true;
						TRACE("初期化情報取得\n");
					}
				}
			}
		}
	}
	else
	{
		// 切断された場合
		CloseNetWork();
		return false;
	}
	return true;
}

void NetWark::CloseNetWork(void)
{
	netState_.reset();
	revStanby = false;
}

bool NetWark::SendMes(MesData& data)
{
	if (!netState_)
	{
		return false;
	}

	NetWorkSend(netState_->GetNetHandle(), &data, sizeof(MesData));

	return true;
}

void NetWark::SendStanby(void)
{
	MesData sendData = {MesType::Stanby, 0, 0};
	SendMes(sendData);
	netState_->SetActiveState(ActiveState::Stanby);
}

void NetWark::SendStart(void)
{
	MesData sendData = { MesType::Game_Start, 0, 0 };
	SendMes(sendData);
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
		break;
	case NetWorkMode::Gest:
		netState_ = std::make_unique<GestState>();
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
