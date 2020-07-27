#include <DxLib.h>
#include "Puyo.h"
#include "SceneMng.h"

Puyo::Puyo(Vector2&& pos, Puyo_Type id) :_puyoSize{40, 40}
{
	_pos = pos;
	_puyoID = id;
	_dirParmit = { 0, 0, 0, 0 };
	alive_ = true;
	dropCnt_ = 0;
	dropInt_ = 10;
	dropSpeed_ = 5;
	colorMap_.try_emplace(Puyo_Type::RED,  0xff0000);
	colorMap_.try_emplace(Puyo_Type::BRUE, 0x0000ff);
	colorMap_.try_emplace(Puyo_Type::GREEN, 0x00ff00);
	colorMap_.try_emplace(Puyo_Type::YELLOW, 0xffff00);
	colorMap_.try_emplace(Puyo_Type::PURPLE, 0x800080);
	color_ = colorMap_[id];
	puyonCnt_ = 0;
	puyonCntMax_ = 10;
}

Puyo::~Puyo()
{
}

bool Puyo::Update(void)
{
	if (dropCnt_ < dropInt_)
	{
		dropCnt_++;
		return true;
	}

	if (_dirParmit.bit.down)
	{
		_pos.y += dropSpeed_;
		dropCnt_ = 0;
		return true;
	}
	else
	{
		return false;
	}
}

void Puyo::Move(INPUT_ID id)
{
	switch (id)
	{
	case INPUT_ID::RIGHT:
		if (_dirParmit.bit.right)
		{
			_pos.x += _puyoSize.x;
		}
		break;
	case INPUT_ID::LEFT:
		if (_dirParmit.bit.left)
		{
			_pos.x -= _puyoSize.x;
		}
		break;
	case INPUT_ID::UP:
		if (_dirParmit.bit.up)
		{
			_pos.y -= _puyoSize.y;
		}
		break;
	case INPUT_ID::DOWN:
		SoftDrop();
		break;
	}
}

void Puyo::Draw(void)
{
	//DrawCircle(_pos.x, _pos.y, _puyoSize.x / 2, color_, true);
	DrawOval(_pos.x, _pos.y, _puyoSize.x / 2, _puyoSize.y / 2, color_, true);
}

const Vector2& Puyo::Pos(void)
{
	return _pos;
}

const Vector2& Puyo::Size(void)
{
	return _puyoSize;
}

const Vector2 Puyo::Grid(int size)
{
	return Vector2(_pos.x / size, (_pos.y - (size / 2)) / size );
}

const Puyo_Type& Puyo::Type(void)
{
	return _puyoID;
}

void Puyo::SetDirPermit(DirPermit dirParmit)
{
	_dirParmit = dirParmit;
}

void Puyo::SoftDrop(void)
{
	dropCnt_ = dropInt_;
}

bool Puyo::Alive(void)
{
	return alive_;
}

void Puyo::Alive(bool flag)
{
	alive_ = flag;
}

bool Puyo::AddPuyonCnt(void)
{
	if (puyonCnt_ >= puyonCntMax_)
	{
		puyonCnt_ = 0;
		return true;
	}
	puyonCnt_++;
	return false;
}
