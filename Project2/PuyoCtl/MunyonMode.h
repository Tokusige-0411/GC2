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
			// •‰‚¯”»’è
			if (field.data_[1][4])
			{
				TRACE("ƒQ[ƒ€ƒI[ƒo[");
				return false;
			}
			// ‚¨‚¶‚á‚Ü‚ÌŒvŽZ‚¨‚æ‚Ñ˜A½‚È‚Ç‚Ì‰Šú‰»
			field.ojamaCnt_ = field.rensaCnt_ * std::exp(field.erasePuyoCnt_ / 8);
			if (field.rensaCnt_ > field.rensaMax_)
			{
				field.rensaMax_ = field.rensaCnt_;
			}
			field.rensaCnt_ = 0;
			field.erasePuyoCnt_ = 0;

			// ‚¨‚¶‚á‚Ü‚Ì‘ŠŽE
			if (field.ojamaCnt_ >= field.ojamaList_.size())
			{
				field.ojamaCnt_ -= field.ojamaList_.size();
				field.ojamaList_.clear();
			}
			else
			{
				field.ojamaList_.erase(field.ojamaList_.begin(), std::next(field.ojamaList_.begin(), field.ojamaCnt_));
			}

			// ‚¨‚¶‚á‚ÜØ½Ä‚É—v‘f‚ª‚ ‚è®ŠŽ‚Â‚¨‚¶‚á‚Ü‚ª~‚Á‚Ä‚¢‚¢ó‘Ô‚¾‚Á‚½‚ç
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