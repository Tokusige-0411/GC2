#pragma once
#include "../Field.h"

struct WinMode
{
	bool operator()(Field& field)
	{
		return true;
	}
};