#include <DxLib.h>
#include "Puyo.h"
#include "SceneMng.h"

Puyo::Puyo() :puyoSize_{ 32, 32 }
{
	Init();
}

Puyo::Puyo(Vector2&& pos, Puyo_ID id) :puyoSize_{32, 32}
{
	pos_ = pos;
	puyoID_ = id;
	colorMap_.try_emplace(Puyo_ID::RED, 0xff0000);
	colorMap_.try_emplace(Puyo_ID::BRUE, 0x0000ff);
	colorMap_.try_emplace(Puyo_ID::GREEN, 0x00ff00);
	colorMap_.try_emplace(Puyo_ID::YELLOW, 0xffff00);
	colorMap_.try_emplace(Puyo_ID::PURPLE, 0xA222A2);
	color_ = colorMap_[id];
	Init();
}

Puyo::~Puyo()
{
}

bool Puyo::Update(int no)
{
	if (puyonCnt_)
	{
		puyonCnt_--;
	}

	if (munyonCnt_)
	{
		munyonCnt_--;
	}

	if (dropCnt_ < dropInt_)
	{
		dropCnt_++;
		return true;
	}

	if (dirPermit_.bit.down)
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
		if (dirPermit_.bit.right)
		{
			pos_.x += puyoSize_.x;
		}
		break;
	case INPUT_ID::LEFT:
		if (dirPermit_.bit.left)
		{
			pos_.x -= puyoSize_.x;
		}
		break;
	case INPUT_ID::UP:
		break;
	case INPUT_ID::DOWN:
		SoftDrop();
		break;
	}
}

void Puyo::Draw(void)
{
	DrawOval(
		pos_.x, 
		pos_.y + (-abs(puyonCnt_ - 6) + 6) * 2 / 3 * (4 - puyonNum_),
		puyoSize_.x / 2,
		puyoSize_.y / 2 - (-abs(puyonCnt_ - 6) + 6) / 3 * (4 - puyonNum_),
		color_, true
	);

	if (drawPermit_.bit.up)
	{
		DrawBox(pos_.x - puyoSize_.x / 2, pos_.y - puyoSize_.y / 2, pos_.x + puyoSize_.x / 2 + 1, pos_.y + 1, color_, true);
	}
	if (drawPermit_.bit.right)
	{
		DrawBox(pos_.x, pos_.y - puyoSize_.y / 2, pos_.x + puyoSize_.x / 2 + 1, pos_.y + puyoSize_.y / 2 + 1, color_, true);
	}
	if (drawPermit_.bit.down)
	{
		DrawBox(pos_.x - puyoSize_.x / 2, pos_.y, pos_.x + puyoSize_.x / 2 + 1, pos_.y + puyoSize_.y / 2 + 1, color_, true);
	}
	if (drawPermit_.bit.left)
	{
		DrawBox(pos_.x - puyoSize_.x / 2, pos_.y - puyoSize_.y / 2, pos_.x + 1, pos_.y + puyoSize_.y / 2 + 1, color_, true);
	}
}

void Puyo::Init(void)
{
	dirPermit_ = { 1, 1, 1, 1 };
	oldDirPermit_ = dirPermit_;
	drawPermit_ = { 0, 0, 0, 0 };
	alive_ = true;
	dropCnt_ = 0;
	dropInt_ = 15;
	dropSpeed_ = 8;
	puyonCnt_ = 0;
	puyonNum_ = 1;
	munyonCnt_ = 0;
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
	return puyoSize_;
}

const Vector2 Puyo::Grid(int size)
{
	return Vector2(pos_.x / size, (pos_.y - (size / 2)) / size );
}

const Puyo_ID& Puyo::Type(void)
{
	return puyoID_;
}

void Puyo::SetDirPermit(DirPermit dirParmit)
{
	oldDirPermit_ = dirPermit_;
	dirPermit_ = dirParmit;
}

const DirPermit& Puyo::GetDirPermit(void)
{
	return dirPermit_;
}

const DirPermit& Puyo::GetOldDirPermit(void)
{
	return oldDirPermit_;
}

void Puyo::SetDrawPermit(DirPermit drawPermit)
{
	drawPermit_ = drawPermit;
}

void Puyo::SetDropSpeed(int speed, int interval)
{
	dropSpeed_ = speed;
	dropInt_ = interval;
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

void Puyo::SetPuyon(int num)
{
	puyonCnt_ = 12;
	puyonNum_ = num;
}

bool Puyo::CheckPuyon(void)
{
	return (puyonCnt_ > 0);
}

void Puyo::SetMunyon(void)
{
	munyonCnt_ = 15;
}

bool Puyo::CheckMunyon(void)
{
	return (munyonCnt_ > 0);
}

void Puyo::ResetMunyon(void)
{
	drawPermit_ = { 0, 0, 0, 0 };
}

int Puyo::GetColor(void)
{
	return color_;
}
