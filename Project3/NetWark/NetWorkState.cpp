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

NetHandleList NetWorkState::GetNetHandle(void)
{
	{
		std::lock_guard<std::mutex> lock(handleMtx_);
		return netHandleList_;
	}
}

std::mutex& NetWorkState::GetHandleMtx(void)
{
	return handleMtx_;
}

void NetWorkState::ResetNetHandle(void)
{
	{
		std::lock_guard<std::mutex> lock(handleMtx_);
		netHandleList_.clear();
	}
}

ActiveState NetWorkState::ConnectHost(IPDATA hostIP)
{
	return ActiveState::Non;
}
