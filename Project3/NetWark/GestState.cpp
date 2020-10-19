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
	netHandle_ = ConnectNetWork(hostIP, portNum_);

	if (netHandle_ != -1)
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
		CloseNetWork(netHandle_);
		TRACE("�z�X�g���ؒf���܂���\n");
		return false;
	}
	return true;
}
