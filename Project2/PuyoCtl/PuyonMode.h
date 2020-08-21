#pragma once
#include "../Field.h"

struct PuyonMode
{
	bool operator()(Field& field) {
		bool rensaFlag = true;

		// ぷよんが終わっていたらtrue, 終わっていなかったらfalse
		for (auto& puyo : field.puyoVec_)
		{
			puyo->Update(0);
			if (puyo->CheckPuyon())
			{
				rensaFlag = false;
			}
		}

		if (rensaFlag)
		{
			field.fieldState_ = FieldState::Rensa;
		}
		return true;
	}
};