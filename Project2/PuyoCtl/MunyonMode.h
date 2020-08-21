#pragma once
#include"../_debug/_DebugConOut.h"
#include "../Field.h"

struct MunyonMode
{
	bool operator()(Field& field) {
		bool nextFlag = true;
		for (auto puyo : field.puyoVec_)
		{
			puyo->Update(0);
			if (puyo->CheckMunyon())
			{
				nextFlag = false;
			}
		}

		if (nextFlag)
		{
			// ��������
			if (field.data_[1][4])
			{
				TRACE("�Q�[���I�[�o�[");
				return false;
			}
			// ������܂̌v�Z����јA���Ȃǂ̏�����
			field.ojamaCnt_ = field.rensaCnt_ * std::exp(field.erasePuyoCnt_ / 8);
			if (field.rensaCnt_ > field.rensaMax_)
			{
				field.rensaMax_ = field.rensaCnt_;
			}
			field.rensaCnt_ = 0;
			field.erasePuyoCnt_ = 0;

			// ������܂̑��E
			if (field.ojamaCnt_ >= field.ojamaList_.size())
			{
				field.ojamaCnt_ -= field.ojamaList_.size();
				field.ojamaList_.clear();
			}
			else
			{
				field.ojamaList_.erase(field.ojamaList_.begin(), std::next(field.ojamaList_.begin(), field.ojamaCnt_));
			}

			// �������ؽĂɗv�f�����菮��������܂��~���Ă�����Ԃ�������
			if (field.ojamaList_.size() && field.ojamaFlag_)
			{
				int count = 0;
				for (auto ojama : field.ojamaList_)
				{
					if (count >= 30)
					{
						break;
					}
					ojama->SetStayInterval(count);
					ojama->Pos({ 48 + ((count % 6) * 32), 16 });
					field.puyoVec_.emplace(field.puyoVec_.begin(), ojama);
					count++;
				}
				field.ojamaList_.erase(field.ojamaList_.begin(), std::next(field.ojamaList_.begin(), count));
				field.ojamaFlag_ = false;
				field.fieldState_ = FieldState::Fall;
			}
			else
			{
				field.InstancePuyo();
				field.SetParmit(field.puyoVec_[0]);
				field.SetParmit(field.puyoVec_[1]);
				field.ojamaFlag_ = true;
				field.fieldState_ = FieldState::Drop;
			}
		}
		return true;
	}
};