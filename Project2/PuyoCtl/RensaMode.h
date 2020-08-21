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
			for (auto& puyo : field.puyoVec_)
			{
				puyo->SetMunyon();
				auto CheckMunyon = [&](Puyo_ID id, Vector2 grid) {
					if ((grid.x >= 0) && (grid.y >= 0))
					{
						if (field.data_[grid.y][grid.x])
						{
							if (field.data_[grid.y][grid.x]->Type() == id)
							{
								return true;
							}
						}
					}
					return false;
				};
				auto grid = puyo->Grid(field.blockSize_);
				auto type = puyo->Type();
				DirPermit tmpPermit;
				tmpPermit.bit.up = CheckMunyon(type, { grid.x, grid.y - 1 });
				tmpPermit.bit.right = CheckMunyon(type, { grid.x + 1, grid.y });
				tmpPermit.bit.down = CheckMunyon(type, { grid.x, grid.y + 1 });
				tmpPermit.bit.left = CheckMunyon(type, { grid.x - 1, grid.y });
				puyo->SetDrawPermit(tmpPermit);
			}
			field.fieldState_ = FieldState::Munyon;
		}
		return true;
	}
};