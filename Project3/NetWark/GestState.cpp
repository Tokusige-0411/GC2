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
	auto handle = ConnectNetWork(hostIP, portNum_);
	if (handle != -1)
	{
		netHandleList_.push_back(PlayerHandle{handle, 0, 0 });
	}

	if (netHandleList_.size())
	{
		if (netHandleList_.front().handle != -1)
		{
			active_ = ActiveState::Init;
			return active_;
		}
	}

	return ActiveState::Non;
}

bool GestState::CheckNetWork(void)
{
	if (!(netHandleList_.size()))
	{
		return false;
	}
	if (GetLostNetWork() != -1)
	{
		CloseNetWork(netHandleList_.front().handle);
		netHandleList_.clear();
		TRACE("ÉzÉXÉgÇ©ÇÁêÿífÇ≥ÇÍÇ‹ÇµÇΩ\n");
		return false;
	}
	return true;
}
