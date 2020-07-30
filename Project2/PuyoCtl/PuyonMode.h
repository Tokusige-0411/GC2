#pragma once
#include "../Field.h"

struct PuyonMode
{
	void operator()(Field& field) {
		bool rensaFlag = true;

		// �Ղ�񂪏I����Ă�����true, �I����Ă��Ȃ�������false
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