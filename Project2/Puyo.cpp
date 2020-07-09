#include <DxLib.h>
#include "Puyo.h"
#include "SceneMng.h"

Puyo::Puyo(Vector2&& pos, Puyo_Type id) :_puyoSize{40, 40}
{
	_pos = pos;
	_puyoID = id;
}

Puyo::~Puyo()
{
}

void Puyo::Update(void)
{
	if (!(lpSceneMng.GetFrameCount() % 60))
	{
		_pos.y += _puyoSize.y;
	}
}

void Puyo::Move(void)
{
}

void Puyo::Draw(void)
{
	DrawCircle(_pos.x, _pos.y, _puyoSize.x / 2, 0xffffff, true);
}
