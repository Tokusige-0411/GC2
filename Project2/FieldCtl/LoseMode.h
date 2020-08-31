#pragma once
#include "../Field.h"

struct LoseMode
{
	bool operator()(Field& field)
	{
		field.offset_.y += 10;
		if (field.offset_.y > 900)
		{
			return true;
		}
		return false;
	}
};
