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
		TRACE("�ڑ��m�F\n");
		if (netHandleList_.size() == 1)
		{
			// �����޳݊J�n����
			lpNetWork.SetConnectTime(lpSceneMng.GetTime());
			lpNetWork.SetConnectFlag(true);
		}
		lpNetWork.SendCountDown();
	}

	if (GetLostNetWork() != -1)
	{
		//PreparationListenNetWork(portNum_);
		//CloseNetWork(netHandle_);
		TRACE("�Q�X�g���ؒf���܂���\n");
		return false;
	}

	return true;
}

