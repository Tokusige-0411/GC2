#pragma once
#include "../Field.h"

struct LoseMode
{
	bool operator()(Field& field)
	{
		return true;
	}
};
