#include "../_debug/_DebugConOut.h"
#include "NetWark.h"
#include "HostState.h"
#include "GestState.h"

std::unique_ptr<NetWark, NetWark::NetWorkDeleter> NetWark::s_Instance(new NetWark());

NetWark::NetWark()
{
	revStanby = false;
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
			// �ްѽ��ď��擾
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
						TRACE("�Q�[���X�^�[�g����M\n");
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
					if (recvData.type == MesType::Stanby)
					{
						revStanby = true;
						TRACE("���������擾\n");
					}
				}
			}
		}
	}
	else
	{
		// �ؒf���ꂽ�ꍇ
		netState_.reset();
		revStanby = false;
		return false;
	}
	return true;
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