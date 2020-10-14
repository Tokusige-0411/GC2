#include <DxLib.h>
#include "HostState.h"

HostState::HostState()
{
	active_ = !(PreparationListenNetWork(portNum_));
}

HostState::~HostState()
{
}
