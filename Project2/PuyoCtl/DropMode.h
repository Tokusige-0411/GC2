#pragma once
#include "../Field.h"
#include "FallMode.h"

struct DropMode
{
	void operator()(Field& field) 
	{
		FallMode()(field);
		field.playerUnit_->Update();
	}
};

