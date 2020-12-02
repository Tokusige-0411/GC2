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
		netHandleList_.push_back(PlayerHandle{ handle, netHandleList_.size() * UNIT_ID_NUM + UNIT_ID_NUM, 0 });
		TRACE("接続確認\n");
		if (netHandleList_.size() == 1)
		{
			// ｶｳﾝﾄﾀﾞｳﾝ開始する
			lpNetWork.SetConnectTime(lpSceneMng.GetTime());
			lpNetWork.SetConnectFlag(true);
		}
		lpNetWork.SendCountDown();
	}

	auto lost = GetLostNetWork();
	if (lost != -1)
	{
		for (auto data : netHandleList_)
		{
			if (data.handle == lost)
			{
				data.netState = -1;
				CloseNetWork(data.handle);
				TRACE("切断がありました:ID%d\n", data.playerID);
			}
		}

		if (!(netHandleList_.size()))
		{
			PreparationListenNetWork(portNum_);
			return false;
		}
	}

	return true;
}

