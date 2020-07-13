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
	//if (!(lpSceneMng.GetFrameCount() % 60))
	//{
	//	_pos.y += _puyoSize.y;
	//}
}

void Puyo::Move(INPUT_ID id)
{
	switch (id)
	{
	case INPUT_ID::RIGHT:
		_pos.x += _puyoSize.x;
		break;
	case INPUT_ID::LEFT:
		_pos.x -= _puyoSize.x;
		break;
	case INPUT_ID::UP:
		_pos.y -= _puyoSize.y;
		break;
	case INPUT_ID::DOWN:
		_pos.y += _puyoSize.y;
		break;
	}
}

void Puyo::Draw(void)
{
	DrawCircle(_pos.x, _pos.y, _puyoSize.x / 2, 0xffffff, true);
}
