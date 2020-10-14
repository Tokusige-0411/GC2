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
