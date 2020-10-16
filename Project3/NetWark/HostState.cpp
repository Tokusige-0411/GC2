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
		TRACE("ê⁄ë±ämîF\n");
	}

	if (GetLostNetWork() == -1)
	{
		PreparationListenNetWork(portNum_);
		return false;
	}

	return true;
}

