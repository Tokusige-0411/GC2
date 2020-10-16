#include <DxLib.h>
#include "GestState.h"
#include "../_debug/_DebugConOut.h"

GestState::GestState()
{
}

GestState::~GestState()
{
}

bool GestState::ConnectHost(IPDATA hostIP)
{
	netHandle_ = ConnectNetWork(hostIP, portNum_);

	if (netHandle_ != -1)
	{
		active_ = ActiveState::Init;
		return true;
	}

	return false;
}

bool GestState::CheckNetWork(void)
{
	if (GetLostNetWork() != -1)
	{
		CloseNetWork(netHandle_);
		TRACE("ƒzƒXƒg‚ªØ’f‚µ‚Ü‚µ‚½\n");
		return false;
	}
	return true;
}
