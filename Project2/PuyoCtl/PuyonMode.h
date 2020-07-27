#pragma once
#include "../Field.h"

struct PuyonMode
{
	void operator()(Field& field) {
		bool rensaFlag = true;

		// �Ղ�񂪏I����Ă�����true, �I����Ă��Ȃ�������false
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