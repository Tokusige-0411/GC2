#pragma once
#include "../Field.h"

struct PuyonMode
{
	void operator()(Field& field) {
		bool rensaFlag = true;

		// ぷよんが終わっていたらtrue, 終わっていなかったらfalse

		for (int i = 0; i < 2; i++)
		{
			if (!field.SetPuyon(field.puyoVec_[i]))
			{
				rensaFlag = false;
			}
		}

		if (rensaFlag)
		{
			field.fieldState_ = FieldState::Munyon;
		}
	}
};