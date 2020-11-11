#include <DxLib.h>
#include <ImageMng.h>
#include "Player.h"

int Player::playerCnt_ = 0;

Player::Player(Vector2 pos)
{
	pos_ = pos;
	dir_ = Dir::Right;
	dirPermit_[Dir::Up] = true;
	dirPermit_[Dir::Right] = true;
	dirPermit_[Dir::Down] = true;
	dirPermit_[Dir::Left] = true;
	Init();
}

Player::~Player()
{
}

void Player::Update(MapData& mapData)
{
	// ‚©‚×Áª¯¸
	auto CheckWall = [&](Vector2 checkPos) {
		if (mapData["Obj"][checkPos.y * 21 + checkPos.x])
		{
			return false;
		}
		return true;
	};

	// Ï½–Ú‚¿‚å‚¤‚Ç‚É‚È‚Á‚½‚ç4•ûŒü’²‚×‚é
	auto chipPos = (pos_ + 16) / 32;
	if ((pos_.x % 32 == 0) && (pos_.y % 32 == 0))
	{
		dirPermit_[Dir::Up] =  CheckWall({ chipPos.x, chipPos.y - 1 });
		dirPermit_[Dir::Down] = CheckWall({ chipPos.x, chipPos.y + 1 });
		dirPermit_[Dir::Right] = CheckWall({ chipPos.x + 1, chipPos.y });
		dirPermit_[Dir::Left] = CheckWall({ chipPos.x - 1, chipPos.y });
	}

	do
	{
		if (!dirPermit_[dir_])
		{
			++dir_;
		}
		if (dir_ == Dir::Max)
		{
			dir_ = Dir::Up;
		}
	} while (!dirPermit_[dir_]);

	if (dir_ == Dir::Right)
	{
		pos_.x += 2;
	}
	if (dir_ == Dir::Down)
	{
		pos_.y += 2;
	}
	if (dir_ == Dir::Up)
	{
		pos_.y -= 2;
	}
	if (dir_ == Dir::Left)
	{
		pos_.x -= 2;
	}
}

void Player::Draw(void)
{
	DrawRotaGraph(pos_.x + 16, pos_.y + 16, 1.0, 0.0, IMAGE_ID("player")[0], true);
}

void Player::Init(void)
{
	playerID_ = playerCnt_;
	playerCnt_++;

	lpImageMng.GetID("player", "image/Player.png");
}
