#pragma once
#include "../Field.h"

struct FallMode
{
	void operator()(Field& field) 
	{
		bool nextFlag = true;
		std::for_each(field._puyoVec.rbegin(), field._puyoVec.rend(), [&](auto& puyo)
			{
				nextFlag &= field.SetParmit(puyo);
			});

		bool puyonFlag = true;
		std::for_each(field._puyoVec.rbegin(), field._puyoVec.rend(), [&](auto& puyo)
			{
				if (puyo->Update())
				{
					puyonFlag = false;
				}
			});

		if (puyonFlag)
		{
			field.fieldState_ = FieldState::Puyon;
		}
	}
};