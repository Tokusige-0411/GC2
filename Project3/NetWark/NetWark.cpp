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
	revState_ = MesType::TMX_Size;
	revStanby = false;
	revCnt_ = 0;
	ipData_ = { 0 };
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
	while(!ProcessMessage())
	{
		if (!netState_)
		{
			continue;
		}

		if (netState_->Update())
		{
			if (netState_->GetMode() == NetWorkMode::Host)
			{
				// �ްѽ��ď��擾
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
							TRACE("�Q�[���X�^�[�g����M\n");
							continue;
						}
					}
				}
			}

			if (netState_->GetMode() == NetWorkMode::Gest)
			{
				if (revStanby)
				{
					netState_->SetActiveState(ActiveState::Play);
					continue;
				}
				else
				{
					auto handle = netState_->GetNetHandle();
					MesHeader recvData;
					if (GetNetWorkDataLength(handle) >= sizeof(MesHeader))
					{
						NetWorkRecv(handle, &recvData, sizeof(MesHeader));
						if (recvData.type == MesType::TMX_Size)
						{
							SizeData sizeData;
							NetWorkRecv(handle, &sizeData, recvData.length);
							revBox_.resize(sizeData.size);
							TRACE("1��Ŏ󂯎��t�@�C���̑傫��:%d\n", revBox_.size());
							continue;
						}
						if (recvData.type == MesType::TMX_Data)
						{
							// �����Ă����f�[�^���i�[���₷���悤��
							unionData csvData;
							//csvData.iData[0] = recvData.data[0];
							//csvData.iData[1] = recvData.data[1];
							//revBox_[recvData.sData] = csvData;
							start = std::chrono::system_clock::now();
							continue;
						}
						if (recvData.type == MesType::Stanby)
						{
							// ���M����
							end = std::chrono::system_clock::now();
							double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
							TRACE("������������:%p�b\n", elapsed);

							revStanby = true;

							std::ifstream ifp("cash/TmxHeader.tmx");
							std::ofstream ofp("cash/RevData.tmx");
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

							// ���������ꂽ��
							unionData unionData = { 0 };
							int strCnt = 0;

							for (auto& data : revBox_)
							{
								unionData = data;
								for (int byteCnt = 0; byteCnt < 16; byteCnt++)
								{
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
													TRACE("���������擾\n");
													break;
												}
												ofp << stringLine << std::endl;
											} while (stringLine.find("data encoding") == std::string::npos);
										}
									}
								}
							}
						}
						{
							// �������b�Z�[�W�ɑ΂���lock��unlock��������
						}
					}
				}
			}
		}
		else
		{
			// �ؒf���ꂽ�ꍇ
			CloseNetWork();
		}
	}
}

void NetWark::CloseNetWork(void)
{
	netState_.reset();
	revStanby = false;
}

bool NetWark::SendMes(MesPacket& data)
{
	if (!netState_)
	{
		return false;
	}

	NetWorkSend(netState_->GetNetHandle(), &data[0], data.size() * sizeof(data[0]));

	return true;
}

void NetWark::SendStanby(void)
{
	MesHeader header = { MesType::Stanby, 0, 0, 0 };
	auto iHeader = Header{ header };
	MesPacket data;
	data.resize(sizeof(header) / sizeof(int));
	data[0].iData = iHeader.data[0];
	data[1].iData = iHeader.data[1];
	SendMes(data);
	netState_->SetActiveState(ActiveState::Stanby);
}

void NetWark::SendStart(void)
{
	MesHeader header = { MesType::Game_Start, 0, 0, 0 };
	auto iHeader = Header{ header };
	MesPacket data;
	data.resize(sizeof(header) / sizeof(int));
	data[0].iData = iHeader.data[0];
	data[1].iData = iHeader.data[1];
	SendMes(data);
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
	case NetWorkMode::Gest:
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
