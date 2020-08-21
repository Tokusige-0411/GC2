#pragma once
#include "../Field.h"
#include "FallMode.h"

struct DropMode
{
	bool operator()(Field& field) 
	{
		field.targetID_ = field.playerUnit_->Update();

		// ¶Ş²ÄŞ‚Õ‚æ‚Ìİ’è
		std::function<void(Vector2, SharedPuyo&)> guideSet = [&](Vector2 grid, SharedPuyo& guidePuyo) {
		
			if (field.data_[grid.y][grid.x])
			{
				auto pos = field.ConvertGrid({grid.x, grid.y - 1});
				guidePuyo->Pos({pos.x, pos.y});
			}
			else
			{
				guideSet({grid.x, grid.y + 1}, guidePuyo);
			}
		};

		auto grid = field.puyoVec_[0]->Grid(field.blockSize_);
		guideSet(grid, field.guidePuyo.first);

		return FallMode()(field);
	}
};

