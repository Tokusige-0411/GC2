#include <DxLib.h>
#include "NetWorkState.h"

NetWorkState::NetWorkState()
{
	active_ = ActiveState::Offline;
}

NetWorkState::~NetWorkState()
{
}

ActiveState NetWorkState::GetActiveState(void)
{
	return active_;
}

void NetWorkState::SetActiveState(ActiveState state)
{
	active_ = state;
}

bool NetWorkState::Update(void)
{
	return CheckNetWork();
}

int NetWorkState::GetNetHandle(void)
{
	return netHandle_;
}

bool NetWorkState::ConnectHost(IPDATA hostIP)
{
	return false;
}
