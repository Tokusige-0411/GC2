#pragma once
#include "../Field.h"
#include "FallMode.h"

struct DropMode
{
	bool operator()(Field& field) 
	{
		field.playerUnit_->Update();
		return FallMode()(field);
	}
};

