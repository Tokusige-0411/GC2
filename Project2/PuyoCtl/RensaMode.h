#pragma once
#include "../Field.h"

struct RensaMode
{
	void operator()(Field& field) {
		bool delFlag = false;
		for (auto&& puyo : field._puyoVec)
		{
			delFlag |= field.SetEraseData(puyo);
		}

		if (delFlag)
		{
			auto itl = std::remove_if(field._puyoVec.begin(), field._puyoVec.end(), [](auto&& puyo) {return !(puyo->Alive()); });
			field._puyoVec.erase(itl, field._puyoVec.end());
			field.fieldState_ = FieldState::Fall;
		}
		else
		{
			field.InstancePuyo();
			field.SetParmit(field._puyoVec[0]);
			field.fieldState_ = FieldState::Drop;
		}
	}
};