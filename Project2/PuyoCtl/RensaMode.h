#pragma once
#include "../Field.h"

struct RensaMode
{
	bool operator()(Field& field) {
		bool delFlag = false;
		for (auto&& puyo : field.puyoVec_)
		{
			delFlag |= field.SetEraseData(puyo);
		}

		if (delFlag)
		{
			auto itl = std::remove_if(field.puyoVec_.begin(), field.puyoVec_.end(), [](auto&& puyo) {return !(puyo->Alive()); });
			field.puyoVec_.erase(itl, field.puyoVec_.end());
			field.fieldState_ = FieldState::Fall;
		}
		else
		{
			// ��������
			if (field.data_[1][4])
			{
				TRACE("�Q�[���I�[�o�[");
				return false;
			}
			// ������܂̌v�Z����јA���Ȃǂ̏�����
			field.ojamaCnt_ = field.rensaMax_ / 2 * field.rensaCnt_ * std::exp(field.erasePuyoCnt_ / 8);
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
				field.targetID_ = 0;
				field.fieldState_ = FieldState::Drop;
			}
		}
		return true;
	}
};