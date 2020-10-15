#include <DxLib.h>
#include "HostState.h"

HostState::HostState()
{
	active_ = !(PreparationListenNetWork(portNum_));
}

HostState::~HostState()
{

}

bool HostState::CheckNetWork(void)
{
	auto data = GetNewAcceptNetWork();

	if (data != -1)
	{
		netHandle_ = data;
		StopListenNetWork();
	}

	if (GetLostNetWork() == -1)
	{
		PreparationListenNetWork(portNum_);
		return false;
	}

	return true;
}

