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
	active_ = !netHandle_;
	return active_;
}
