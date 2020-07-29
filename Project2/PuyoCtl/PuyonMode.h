#pragma once
#include "../Field.h"

struct PuyonMode
{
	void operator()(Field& field) {
		bool rensaFlag = true;

		// �Ղ�񂪏I����Ă�����true, �I����Ă��Ȃ�������false

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