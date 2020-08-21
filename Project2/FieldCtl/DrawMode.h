#pragma once
#include "../Field.h"

struct DrawMode
{
	bool operator()(Field& field)
	{
		return true;
	}
};