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
			// 負け判定
			if (field.data_[1][4])
			{
				TRACE("ゲームオーバー");
				return false;
			}
			// おじゃまの計算および連鎖などの初期化
			field.ojamaCnt_ = static_cast<int>(field.rensaMax_ / 2 * field.rensaCnt_ * std::exp(field.erasePuyoCnt_ / 8));
			if (field.rensaCnt_ > field.rensaMax_)
			{
				field.rensaMax_ = field.rensaCnt_;
			}
			field.rensaCnt_ = 0;
			field.erasePuyoCnt_ = 0;

			// おじゃまの相殺
			if (field.ojamaCnt_ >= static_cast<int>(field.ojamaList_.size()))
			{
				field.ojamaCnt_ -= field.ojamaList_.size();
				field.ojamaList_.clear();
			}
			else
			{
				field.ojamaList_.erase(field.ojamaList_.begin(), std::next(field.ojamaList_.begin(), field.ojamaCnt_));
			}

			// おじゃまﾘｽﾄに要素があり尚且つおじゃまが降っていい状態だったら
			int ojamaBeginPos = -1;
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
					if (field.ojamaList_.size() >= 6)	
					{
						ojama->Pos({ 48 + ((count % 6) * 32), 16 });
					}
					else
					{
						if (ojamaBeginPos == -1)
						{
							ojamaBeginPos = rand() % 6;
						}
						ojama->Pos({ 48 + ((ojamaBeginPos % 6) * 32), 16 });
						ojamaBeginPos++;
					}
					//ojama->Pos({ 48 + ((count % 6) * 32), 16 });
					ojama->SetDropSpeed(8, 1);
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