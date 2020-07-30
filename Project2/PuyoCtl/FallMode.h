#pragma once
#include "../Field.h"

struct FallMode
{
	void operator()(Field& field) 
	{
		std::for_each(field.puyoVec_.rbegin(), field.puyoVec_.rend(), [&](auto& puyo)
			{
				if (field.SetParmit(puyo))
				{
					puyo->SetPuyon();
				}
			});

		bool puyonFlag = true;
		std::for_each(field.puyoVec_.rbegin(), field.puyoVec_.rend(), [&](auto& puyo)
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