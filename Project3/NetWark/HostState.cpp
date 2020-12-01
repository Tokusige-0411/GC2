#include <DxLib.h>
#include <chrono>
#include "HostState.h"
#include "NetWark.h"
#include "../_debug/_DebugConOut.h"
#include "../Scene/SceneMng.h"

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
		netHandleList_.push_back(std::pair<int, int>( handle, netHandleList_.size() * UNIT_ID_NUM + UNIT_ID_NUM ));
		TRACE("接続確認\n");
		if (netHandleList_.size() == 1)
		{
			// ｶｳﾝﾄﾀﾞｳﾝ開始する
			lpNetWork.SetConnectTime(lpSceneMng.GetTime());
			lpNetWork.SetConnectFlag(true);
		}
		lpNetWork.SendCountDown();
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

