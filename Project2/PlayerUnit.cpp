#include "_debug/_DebugConOut.h"
#include "PlayerUnit.h"

PlayerUnit::PlayerUnit(Field& field) : field_(field)
{
	targetID_ = 0;
}

PlayerUnit::~PlayerUnit()
{
}

void PlayerUnit::Update()
{
	auto RatoPuyo = [&](Vector2 puyoPos1, Vector2 puyoPos2, bool rightRota) {
		auto rotaVec = field_.blockSize_;
		if (!rightRota)
		{
			rotaVec = -field_.blockSize_;
		}

		if (puyoPos1.y > puyoPos2.y)
		{
			field_.puyoVec_[targetID_ ^ 1]->Pos({puyoPos1.x + rotaVec, puyoPos1.y});
		}
		if (puyoPos1.y < puyoPos2.y)
		{
			field_.puyoVec_[targetID_ ^ 1]->Pos({ puyoPos1.x - rotaVec, puyoPos1.y });
		}
		if (puyoPos1.x > puyoPos2.x)
		{
			field_.puyoVec_[targetID_ ^ 1]->Pos({ puyoPos1.x, puyoPos1.y - rotaVec });
		}
		if (puyoPos1.x < puyoPos2.x)
		{
			field_.puyoVec_[targetID_ ^ 1]->Pos({ puyoPos1.x, puyoPos1.y + rotaVec });
		}
	};

	if (!field_.puyoVec_[targetID_]->GetDirParmit().bit.down || !field_.puyoVec_[targetID_ ^ 1]->GetDirParmit().bit.down)
	{
		field_.fieldState_ = FieldState::Fall;
		targetID_ = 0;
		return;
	}

	if (field_.puyoVec_[0]->Pos().y > field_.puyoVec_[1]->Pos().y)
	{
		std::swap(field_.puyoVec_[0], field_.puyoVec_[1]);
		targetID_ ^= 1;
		TRACE("%d", targetID_);
	}

	auto data = field_._controller->GetContData();
	if ((data[INPUT_ID::L_ROTA][static_cast<int>(Trg::Now)] && !data[INPUT_ID::L_ROTA][static_cast<int>(Trg::Old)]) || 
		(data[INPUT_ID::R_ROTA][static_cast<int>(Trg::Now)] && !data[INPUT_ID::R_ROTA][static_cast<int>(Trg::Old)]))
	{
		RatoPuyo(field_.puyoVec_[targetID_]->Pos(), field_.puyoVec_[targetID_ ^ 1]->Pos(), data[INPUT_ID::R_ROTA][static_cast<int>(Trg::Now)]);
	}

	if (data[INPUT_ID::DOWN][static_cast<int>(Trg::Now)] && data[INPUT_ID::DOWN][static_cast<int>(Trg::Old)])
	{
		field_.puyoVec_[targetID_]->SoftDrop();
		field_.puyoVec_[targetID_ ^ 1]->SoftDrop();
	}

	for (auto data : field_._controller->GetContData())
	{
		if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
		{
			field_.puyoVec_[targetID_]->Move(data.first);
			field_.puyoVec_[targetID_ ^ 1]->Move(data.first);
		}
	}
}
