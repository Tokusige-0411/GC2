#pragma once
#include "NetWorkState.h"
class GestState :
	public NetWorkState
{
public:
	GestState();
	~GestState() override;
	NetWorkMode GetMode(void) override { return NetWorkMode::Gest; };
	bool ConnectHost(IPDATA hostIP) override;

private:
	bool CheckNetWork(void) override;
};

