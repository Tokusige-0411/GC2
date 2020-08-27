#pragma once
#include "../Field.h"

struct PuyonMode
{
	bool operator()(Field& field) {
		bool rensaFlag = true;

		// ‚Õ‚æ‚ñ‚ªI‚í‚Á‚Ä‚¢‚½‚çtrue, I‚í‚Á‚Ä‚¢‚È‚©‚Á‚½‚çfalse
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
			for (auto& puyo : field.puyoVec_)
			{
				puyo->SetMunyon();
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
			//field.fieldState_ = FieldState::Rensa;
		}
		return true;
	}
};