#include <DxLib.h>
#include <ImageMng.h>
#include "Player.h"
#include "../NetWark/NetWark.h"

Player::Player(int id, Vector2 pos)
{
	pos_ = pos;
	playerID_ = id;
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

	// ˆÚ“®ˆ—‚Æ•ûŒü‚ğ’²‚×‚é
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

	update_();

	animCnt_++;
}

void Player::Draw(void)
{
	DrawRotaGraph(pos_.x + 16, pos_.y - 6, 1.0, 0.0, IMAGE_ID("player")[(2 + (animCnt_ / 15 % 2)) * 4 + static_cast<int>(dir_)], true);
}

int Player::GetPlayerID(void)
{
	return playerID_;
}

void Player::Init(void)
{
	lpImageMng.GetID("player", "image/Player_Anim.png", { 32, 51 }, { 4, 4 });

	if (lpNetWork.GetNetWorkMode() == NetWorkMode::Host)
	{
		if (!(playerID_ % BIT_NUM))
		{
			update_ = std::bind(&Player::UpdateMyself, this);
		}
		else
		{
			update_ = std::bind(&Player::UpdateNet, this);
		}
	}

	if (lpNetWork.GetNetWorkMode() == NetWorkMode::Guest)
	{
		if (!(playerID_ % BIT_NUM))
		{
			update_ = std::bind(&Player::UpdateNet, this);
		}
		else
		{
			update_ = std::bind(&Player::UpdateMyself, this);
		}
	}

	if (lpNetWork.GetNetWorkMode() == NetWorkMode::Offline)
	{
		update_ = std::bind(&Player::UpdateMyself, this);
	}

	dir_ = Dir::Right;
	dirPermit_[Dir::Up] = true;
	dirPermit_[Dir::Right] = true;
	dirPermit_[Dir::Down] = true;
	dirPermit_[Dir::Left] = true;
	animCnt_ = 0;
	speed_ = 4;

	lpNetWork.AddMesList(playerID_, mesList_, mtx_);
}

void Player::UpdateMyself(void)
{
	if (dir_ == Dir::Right)
	{
		pos_.x += speed_;
	}
	if (dir_ == Dir::Down)
	{
		pos_.y += speed_;
	}
	if (dir_ == Dir::Up)
	{
		pos_.y -= speed_;
	}
	if (dir_ == Dir::Left)
	{
		pos_.x -= speed_;
	}

	// À•Wî•ñ‚Ì‘—M
	MesPacket plData;
	plData.resize(4);
	plData[0].iData = playerID_;
	plData[1].iData = pos_.x;
	plData[2].iData = pos_.y;
	plData[3].iData = static_cast<int>(dir_);
	lpNetWork.SendMes(plData, MesType::Pos);
}

void Player::UpdateNet(void)
{
	std::lock_guard<std::mutex> mut(mtx_);
	if (mesList_.size())
	{
		auto data = mesList_.front();
		pos_ = Vector2{ data[1].iData, data[2].iData };
		dir_ = static_cast<Dir>(data[3].iData);
		mesList_.erase(mesList_.begin());
	}
}
