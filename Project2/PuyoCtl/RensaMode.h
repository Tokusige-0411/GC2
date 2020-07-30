#pragma once
#include "../Field.h"

struct RensaMode
{
	void operator()(Field& field) {
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
			auto CheckMunyon = [&](Puyo_Type id, Vector2 grid) {
				if (field.data_[grid.y][grid.x])
				{
					if (field.data_[grid.y][grid.x]->Type() == id)
					{
						return true;
					}
				}
				return false;
			};

			for (auto& puyo : field.puyoVec_)
			{
				puyo->SetMunyon();
				auto grid = puyo->Grid(field.blockSize_);
				DirPermit tmpPermit;
				tmpPermit.bit.up = CheckMunyon(puyo->Type(), { grid.x, grid.y - 1 });
				tmpPermit.bit.right = CheckMunyon(puyo->Type(), { grid.x + 1, grid.y });
				tmpPermit.bit.down = CheckMunyon(puyo->Type(), { grid.x, grid.y + 1 });
				tmpPermit.bit.left = CheckMunyon(puyo->Type(), { grid.x - 1, grid.y });
				puyo->SetDrawPermit(tmpPermit);
			}
			field.fieldState_ = FieldState::Munyon;
		}
	}
};