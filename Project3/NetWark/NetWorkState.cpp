#include <DxLib.h>
#include "NetWorkState.h"

NetWorkState::NetWorkState()
{
	active_ = false;
}

NetWorkState::~NetWorkState()
{
}

bool NetWorkState::GetActive(void)
{
	return active_;
}

bool NetWorkState::Update(void)
{
	if (!CheckNetWork())
	{
		return false;
	}
	return true;
}

int NetWorkState::GetNetHandle(void)
{
	return netHandle_;
}

bool NetWorkState::ConnectHost(IPDATA hostIP)
{
	return false;
}
