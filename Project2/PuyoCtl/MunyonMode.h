#pragma once
#include "../Field.h"

struct MunyonMode
{
	void operator()(Field& field) {
		bool nextFlag = true;
		for (auto puyo : field.puyoVec_)
		{
			puyo->Update();
			if (puyo->CheckMunyon())
			{
				nextFlag = false;
			}
		}

		if (nextFlag)
		{
			field.InstancePuyo();
			field.SetParmit(field.puyoVec_[0]);
			field.fieldState_ = FieldState::Drop;
		}
	}
};