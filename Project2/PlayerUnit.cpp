#include "PlayerUnit.h"

PlayerUnit::PlayerUnit(Field& field) : field_(field)
{
}

PlayerUnit::~PlayerUnit()
{
}

void PlayerUnit::Update()
{
	if (field_.fieldState_ == FieldState::Rensa)
	{
		return;
	}

	for (auto data : field_._controller->GetContData())
	{
		if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
		{
			field_._puyoVec.front()->Move(data.first);
		}
	}

	auto data = field_._controller->GetContData();
	if (data[INPUT_ID::DOWN][static_cast<int>(Trg::Now)] && data[INPUT_ID::DOWN][static_cast<int>(Trg::Old)])
	{
		field_._puyoVec[0]->SoftDrop();
	}
}
