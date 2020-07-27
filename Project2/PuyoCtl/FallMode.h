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

		bool rensaFlag = true;
		std::for_each(field._puyoVec.rbegin(), field._puyoVec.rend(), [&](auto& puyo)
			{
				if (puyo->Update())
				{
					rensaFlag = false;
				}
			});

		if (rensaFlag)
		{
			field.fieldState_ = FieldState::Rensa;
		}
	}
};