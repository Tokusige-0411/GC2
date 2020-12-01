#include <DxLib.h>
#include "GestState.h"
#include "../_debug/_DebugConOut.h"

GestState::GestState()
{
}

GestState::~GestState()
{
}

ActiveState GestState::ConnectHost(IPDATA hostIP)
{
	//netHandle_ = ConnectNetWork(hostIP, portNum_);
	netHandleList_.push_back(std::pair<int, int>(ConnectNetWork(hostIP, portNum_), 0));

	if (netHandleList_.front().first != -1)
	{
		active_ = ActiveState::Init;
		return active_;
	}

	return ActiveState::Non;
}

bool GestState::CheckNetWork(void)
{
	if (GetLostNetWork() != -1)
	{
		CloseNetWork(netHandleList_.front().first);
		TRACE("ÉzÉXÉgÇ©ÇÁêÿífÇ≥ÇÍÇ‹ÇµÇΩ\n");
		return false;
	}
	return true;
}
