#pragma once
#include "../Field.h"

struct PuyonMode
{
	void operator()(Field& field) {
		bool rensaFlag = true;

		// ‚Õ‚æ‚ñ‚ªI‚í‚Á‚Ä‚¢‚½‚çtrue, I‚í‚Á‚Ä‚¢‚È‚©‚Á‚½‚çfalse

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