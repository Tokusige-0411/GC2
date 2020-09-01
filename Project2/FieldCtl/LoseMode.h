#pragma once
#include "../Field.h"

struct LoseMode
{
	bool operator()(Field& field)
	{
		if (field.offset_.y < 900)
		{
			field.offset_.y += field.fallSpeed_;
			field.fallSpeed_ += 1;
			field.rad_ += 0.05;
		}
		else
		{
			return true;
		}
		return false;
	}
};
