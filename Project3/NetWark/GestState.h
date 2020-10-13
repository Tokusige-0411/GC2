#pragma once
#include "NetWorkState.h"
class GestState :
	public NetWorkState
{
public:
	GestState();
	~GestState() override;
	NetWorkMode GetMode(void) override { return NetWorkMode::Gest; };
};

