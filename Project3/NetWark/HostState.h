#pragma once
#include "NetWorkState.h"
class HostState :
	public NetWorkState
{
public:
	HostState();
	~HostState() override;
	NetWorkMode GetMode(void) override { return NetWorkMode::Host; };

private:
	bool CheckNetWork(void) override;
};

