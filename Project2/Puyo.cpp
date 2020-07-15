#include <DxLib.h>
#include "Puyo.h"
#include "SceneMng.h"

Puyo::Puyo(Vector2&& pos, Puyo_Type id) :_puyoSize{40, 40}
{
	_pos = pos;
	_puyoID = id;
	_dirParmit = { 0, 0, 0, 0 };
}

Puyo::~Puyo()
{
}

void Puyo::Update(void)
{
	//if (!(lpSceneMng.GetFrameCount() % 60))
	//{
	//	_pos.y += _puyoSize.y / 2;
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

Vector2 Puyo::Pos(void)
{
	return _pos;
}

Vector2 Puyo::Size(void)
{
	return _puyoSize;
}

Vector2 Puyo::Chip(void)
{
	return Vector2(_pos.x / _puyoSize.x, _pos.y / _puyoSize.y);
}

void Puyo::SetDirPermit(DirPermit dirParmit)
{
	_dirParmit = dirParmit;
}
