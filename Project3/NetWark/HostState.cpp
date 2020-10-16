#include <DxLib.h>
#include "HostState.h"
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
		netHandle_ = handle;
		active_ = ActiveState::Init;
		StopListenNetWork();
		TRACE("接続確認\n");
	}

	if (GetLostNetWork() != -1)
	{
		//PreparationListenNetWork(portNum_);
		CloseNetWork(netHandle_);
		TRACE("ゲストが切断しました\n");
		return false;
	}

	return true;
}

