#pragma once
#include "../Field.h"

struct FallMode
{
	bool operator()(Field& field) 
	{
		std::for_each(field.puyoVec_.rbegin(), field.puyoVec_.rend(), [&](auto& puyo)
			{
				if (!field.SetParmit(puyo))
				{
					puyo->ResetMunyon();
				}
			});

		bool puyonFlag = true;
		std::for_each(field.puyoVec_.rbegin(), field.puyoVec_.rend(), [&](auto& puyo)
			{
				if (puyo->Update(0))
				{
					puyonFlag = false;
				}
				if ((!puyo->GetDirPermit().bit.down) && (puyo->GetOldDirPermit().bit.down))
				{
					int count = 1;
					puyo->SetPuyon(count);
					auto grid = puyo->Grid(field.blockSize_);
					while (count < 3)
					{
						if ((grid.y + count) < field.stgGridSize_.y)
						{
							if (field.data_[grid.y + count][grid.x])
							{
								field.data_[grid.y + count][grid.x]->SetPuyon(count + 1);
							}
						}
						else
						{
							break;
						}
						count++;
					}
				}
			});

		if (puyonFlag)
		{
			field.fieldState_ = FieldState::Puyon;
		}
		return true;
	}
};