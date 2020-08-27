#pragma once
#include"../_debug/_DebugConOut.h"
#include "../Field.h"

struct MunyonMode
{
	bool operator()(Field& field) {
		bool nextFlag = true;
		for (auto puyo : field.puyoVec_)
		{
			puyo->Update(0);
			if (puyo->CheckMunyon())
			{
				nextFlag = false;
			}
		}

		if (nextFlag)
		{
			field.fieldState_ = FieldState::Rensa;
		}
		return true;
	}
};