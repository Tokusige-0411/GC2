#include <DxLib.h>
#include "HostState.h"
#include "NetWark.h"
#include "../_debug/_DebugConOut.h"

HostState::HostState()
{
	auto flag = !(PreparationListenNetWork(portNum_));
	active_ = ActiveState::Wait;
}

HostState::~HostState()
{

}

bool HostState::CheckNetWork(void)
{
	auto handle = GetNewAcceptNetWork();

	if (handle != -1)
	{
		//netHandle_ = handle;
		//StopListenNetWork();
		netHandleList_.push_back(std::pair<int, int>( handle, netHandleList_.size() * UNIT_ID_NUM + UNIT_ID_NUM ));
		if (netHandleList_.size() == 1)
		{

		}

		active_ = ActiveState::Init;
		TRACE("接続確認\n");
	}

	if (GetLostNetWork() != -1)
	{
		//PreparationListenNetWork(portNum_);
		//CloseNetWork(netHandle_);
		TRACE("ゲストが切断しました\n");
		return false;
	}

	return true;
}

