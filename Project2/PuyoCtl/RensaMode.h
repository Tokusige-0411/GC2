#pragma once
#include "../Field.h"

struct RensaMode
{
	void operator()(Field& field) {
		bool delFlag = false;
		for (auto&& puyo : field.puyoVec_)
		{
			delFlag |= field.SetEraseData(puyo);
		}

		if (delFlag)
		{
			auto itl = std::remove_if(field.puyoVec_.begin(), field.puyoVec_.end(), [](auto&& puyo) {return !(puyo->Alive()); });
			field.puyoVec_.erase(itl, field.puyoVec_.end());
			field.fieldState_ = FieldState::Fall;
		}
		else
		{
			field.InstancePuyo();
			field.SetParmit(field.puyoVec_[0]);
			field.fieldState_ = FieldState::Drop;
		}
	}
};