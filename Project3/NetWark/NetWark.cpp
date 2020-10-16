#include "../_debug/_DebugConOut.h"
#include "NetWark.h"
#include "HostState.h"
#include "GestState.h"

std::unique_ptr<NetWark, NetWark::NetWorkDeleter> NetWark::s_Instance(new NetWark());

NetWark::NetWark()
{
}

NetWark::~NetWark()
{
}

bool NetWark::Update(void)
{
	if (netState_->Update())
	{
		if (netState_->GetMode() == NetWorkMode::Host)
		{
			if (netState_->GetActiveState() == ActiveState::Init)
			{

			}
		}
	}
}

bool NetWark::SendMes(MesData& data)
{
	if (!netState_)
	{
		return false;
	}

	return true;
}

void NetWark::SendStanby(void)
{
	MesData sendData;
	sendData.type = MesType::Stanby;
	NetWorkSend(netState_->GetNetHandle(), &sendData, sizeof(MesData));
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
	return (*netState_).GetMode();
}

ActiveState NetWark::GetActive(void)
{
	return (*netState_).GetActiveState();;
}

int NetWark::GetNetHandle(void)
{
	return netState_->GetNetHandle();
}

bool NetWark::ConnectHost(IPDATA hostIP)
{
	return (*netState_).ConnectHost(hostIP);
}

IPDATA NetWark::GetIP(void)
{
	IPDATA data;
	GetMyIPAddress(&data);
	return data;
}
