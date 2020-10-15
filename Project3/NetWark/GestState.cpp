#include <DxLib.h>
#include "GestState.h"

GestState::GestState()
{
	active_ = false;
}

GestState::~GestState()
{
}

bool GestState::ConnectHost(IPDATA hostIP)
{
	netHandle_ = ConnectNetWork(hostIP, portNum_);
	active_ = (netHandle_ >= 0 ? true : false);
	return active_;
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
