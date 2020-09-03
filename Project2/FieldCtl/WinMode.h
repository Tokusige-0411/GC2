#pragma once
#include "../Field.h"

struct WinMode
{
	bool operator()(Field& field)
	{
		if (field.resultEndCnt_ < 240)
		{
			field.LogoDraw(field.resultLogo_[ResultF::Win], field.resultLogoPos_, 0.0);
			if (!field.resultEndCnt_)
			{
				return true;
			}
		}
		field.resultEndCnt_--;
		return false;
		//return true;
	}
};