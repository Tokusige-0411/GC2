#include <DxLib.h>
#include "Puyo.h"
#include "SceneMng.h"

Puyo::Puyo(Vector2&& pos, Puyo_Type id) :_puyoSize{40, 40}
{
	pos_ = pos;
	puyoID_ = id;
	dirParmit_ = { 0, 0, 0, 0 };
	drawParmit_ = { 0, 0, 0, 0 };
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
	puyonCntMax_ = 20;
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

	if (dirParmit_.bit.down)
	{
		pos_.y += dropSpeed_;
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
		if (dirParmit_.bit.right)
		{
			pos_.x += _puyoSize.x;
		}
		break;
	case INPUT_ID::LEFT:
		if (dirParmit_.bit.left)
		{
			pos_.x -= _puyoSize.x;
		}
		break;
	case INPUT_ID::UP:
		if (dirParmit_.bit.up)
		{
			pos_.y -= _puyoSize.y;
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
	DrawOval(
		pos_.x, 
		pos_.y + (puyonCnt_ - ((puyonCnt_ % (puyonCntMax_ / 2 + 1)) * 2) * (puyonCnt_ > (puyonCntMax_ / 2 + 1))) * 2,
		_puyoSize.x / 2 + (puyonCnt_ - ((puyonCnt_ % (puyonCntMax_ / 2 + 1)) * 2) * (puyonCnt_ > (puyonCntMax_ / 2 + 1))) / 2,
		_puyoSize.y / 2 - (puyonCnt_ - ((puyonCnt_ % (puyonCntMax_ / 2 + 1)) * 2) * (puyonCnt_ > (puyonCntMax_ / 2 + 1))) / 3,
		color_, true);
	if (drawParmit_.bit.up)
	{
		DrawBox(pos_.x - _puyoSize.x / 2, pos_.y - _puyoSize.y / 2, pos_.x + _puyoSize.x / 2 + 1, pos_.y + 1, color_, true);
	}
	if (drawParmit_.bit.right)
	{
		DrawBox(pos_.x, pos_.y - _puyoSize.y / 2, pos_.x + _puyoSize.x / 2 + 1, pos_.y + _puyoSize.y / 2 + 1, color_, true);
	}
	if (drawParmit_.bit.down)
	{
		DrawBox(pos_.x - _puyoSize.x / 2, pos_.y, pos_.x + _puyoSize.x / 2 + 1, pos_.y + _puyoSize.y / 2 + 1, color_, true);
	}
	if (drawParmit_.bit.left)
	{
		DrawBox(pos_.x - _puyoSize.x / 2, pos_.y - _puyoSize.y / 2, pos_.x + 1, pos_.y + _puyoSize.y / 2 + 1, color_, true);
	}
}

const Vector2& Puyo::Pos(void)
{
	return pos_;
}

void Puyo::Pos(Vector2&& pos)
{
	pos_ = pos;
}

const Vector2& Puyo::Size(void)
{
	return _puyoSize;
}

const Vector2 Puyo::Grid(int size)
{
	return Vector2(pos_.x / size, (pos_.y - (size / 2)) / size );
}

const Puyo_Type& Puyo::Type(void)
{
	return puyoID_;
}

void Puyo::SetDirPermit(DirPermit dirParmit)
{
	dirParmit_ = dirParmit;
}

const DirPermit& Puyo::GetDirParmit(void)
{
	return dirParmit_;
}

void Puyo::SetDrawPermit(DirPermit drawPermit)
{
	drawParmit_ = drawPermit;
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
