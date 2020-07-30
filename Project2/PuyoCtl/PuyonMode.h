#pragma once
#include "../Field.h"

struct PuyonMode
{
	void operator()(Field& field) {
		bool rensaFlag = true;

		// ‚Õ‚æ‚ñ‚ªI‚í‚Á‚Ä‚¢‚½‚çtrue, I‚í‚Á‚Ä‚¢‚È‚©‚Á‚½‚çfalse
		for (auto& puyo : field.puyoVec_)
		{
			puyo->Update();
			if (puyo->CheckPuyon())
			{
				rensaFlag = false;
			}
		}

		if (rensaFlag)
		{
			field.fieldState_ = FieldState::Rensa;
		}
	}
};