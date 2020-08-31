#pragma once
#include "../Field.h"
#include "FallMode.h"

struct DropMode
{
	bool operator()(Field& field) 
	{
		field.targetID_ = field.playerUnit_->Update(field.targetID_);

		// ¶Ş²ÄŞ‚Õ‚æ‚Ìİ’è
		std::function<void(Vector2, SharedPuyo&)> guideSet = [&](Vector2 grid, SharedPuyo& guidePuyo) {
		
			if ((grid.y >= 0) && 
				(grid.y < field.stgGridSize_.y) &&
				(grid.x >= 0) &&
				(grid.x < field.stgGridSize_.x)
				)
			{
				if (field.data_[grid.y][grid.x])
				{
					auto pos = field.ConvertGrid({ grid.x, grid.y - 1 });
					guidePuyo->Pos({ pos.x, pos.y });
				}
				else
				{
					guideSet({ grid.x, grid.y + 1 }, guidePuyo);
				}
			}
		};

		//auto grid = field.puyoVec_[field.targetID_]->Grid(field.blockSize_);
		//guideSet(grid, field.guidePuyo.first);
		//grid = field.puyoVec_[field.targetID_ ^ 1]->Grid(field.blockSize_);
		//guideSet(grid, field.guidePuyo.second);

		//if (field.puyoVec_[field.targetID_ ^ 1]->Pos().y > field.puyoVec_[field.targetID_]->Pos().y)
		//{
		//	grid = field.guidePuyo.first->Grid(field.blockSize_);
		//	field.guidePuyo.first->Pos(field.ConvertGrid({ grid.x, grid.y - 1 }));
		//}

		auto pos = field.puyoVec_[field.targetID_]->Pos();
		field.guidePuyo.first->Pos(std::move(pos));
		pos = field.puyoVec_[field.targetID_ ^ 1]->Pos();
		field.guidePuyo.second->Pos(std::move(pos));

		guideSet(field.guidePuyo.first->Grid(field.blockSize_), field.guidePuyo.first);
		guideSet(field.guidePuyo.second->Grid(field.blockSize_), field.guidePuyo.second);

		// â‘Îl‚¦’¼‚·
		Vector2 grid;
		if (field.puyoVec_[field.targetID_]->Pos().y > field.puyoVec_[field.targetID_ ^ 1]->Pos().y)
		{
			grid = field.guidePuyo.second->Grid(field.blockSize_);
			field.guidePuyo.second->Pos(field.ConvertGrid({ grid.x, grid.y - 1 }));
		}
		else if(field.puyoVec_[field.targetID_]->Pos().y < field.puyoVec_[field.targetID_ ^ 1]->Pos().y)
		{
			grid = field.guidePuyo.first->Grid(field.blockSize_);
			field.guidePuyo.first->Pos(field.ConvertGrid({ grid.x, grid.y - 1 }));
		}

		return FallMode()(field);
	}
};

