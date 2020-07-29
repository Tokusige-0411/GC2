#pragma once
#include "../Field.h"

struct MunyonMode
{
	void operator()(Field& field) {
		for (auto puyo : field.puyoVec_)
		{
			field.SetMunyon(puyo);
		}
		field.fieldState_ = FieldState::Rensa;
	}
};