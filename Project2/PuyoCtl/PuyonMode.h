#pragma once
#include "../Field.h"

struct PuyonMode
{
	bool operator()(Field& field) {
		bool rensaFlag = true;

		// �Ղ�񂪏I����Ă�����true, �I����Ă��Ȃ�������false
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