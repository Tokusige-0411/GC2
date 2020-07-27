#pragma once
#include "../Field.h"

struct PuyonMode
{
	void operator()(Field& field) {
		bool rensaFlag = true;

		// ぷよんが終わっていたらtrue, 終わっていなかったらfalse
		if (!field.SetPuyon(field._puyoVec[0]))
		{
			rensaFlag = false;
		}

		if (rensaFlag)
		{
			field.fieldState_ = FieldState::Rensa;
		}
	}
};