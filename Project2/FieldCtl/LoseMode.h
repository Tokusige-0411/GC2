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
			if (field.resultEndCnt_ < 240)
			{
				field.LogoDraw(field.resultLogo_[ResultF::Lose], field.resultLogoPos_, 0.0);
				if (!field.resultEndCnt_)
				{
					return true;
				}
			}
		}
		field.resultEndCnt_--;
		return false;
	}
};
