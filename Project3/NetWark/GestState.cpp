#include <DxLib.h>
#include "GestState.h"

GestState::GestState()
{
}

GestState::~GestState()
{
}

bool GestState::ConnectHost(IPDATA hostIP)
{
	netHandle_ = ConnectNetWork(hostIP, portNum_);

	if (netHandle_ != -1)
	{
		active_ = ActiveState::Init;
		return true;
	}

	return false;
}

bool GestState::CheckNetWork(void)
{
	if (GetLostNetWork() != -1)
	{
		CloseNetWork(netHandle_);
		return false;
	}
	return true;
}
