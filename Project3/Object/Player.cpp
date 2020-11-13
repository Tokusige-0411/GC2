#include <DxLib.h>
#include <ImageMng.h>
#include "Player.h"
#include "../NetWark/NetWark.h"

int Player::playerCnt_ = 0;

Player::Player(Vector2 pos)
{
	pos_ = pos;
	dir_ = Dir::Right;
	dirPermit_[Dir::Up] = true;
	dirPermit_[Dir::Right] = true;
	dirPermit_[Dir::Down] = true;
	dirPermit_[Dir::Left] = true;
	animCnt_ = 0;
	Init();
}

Player::~Player()
{
}

void Player::Update(MapData& mapData)
{
	// かべﾁｪｯｸ
	auto CheckWall = [&](Vector2 checkPos) {
		if (mapData["Obj"][checkPos.y * 21 + checkPos.x])
		{
			return false;
		}
		return true;
	};

	// ﾏｽ目ちょうどになったら4方向調べる
	auto chipPos = (pos_ + 16) / 32;
	if ((pos_.x % 32 == 0) && (pos_.y % 32 == 0))
	{
		dirPermit_[Dir::Up] =  CheckWall({ chipPos.x, chipPos.y - 1 });
		dirPermit_[Dir::Down] = CheckWall({ chipPos.x, chipPos.y + 1 });
		dirPermit_[Dir::Right] = CheckWall({ chipPos.x + 1, chipPos.y });
		dirPermit_[Dir::Left] = CheckWall({ chipPos.x - 1, chipPos.y });
	}

	// 移動処理と方向を調べる
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
	//lpImageMng.GetID("player", "image/bomberman.png", { 32, 51 }, { 5, 4 });
	lpImageMng.GetID("player", "image/Player_Anim.png", { 32, 51 }, { 4, 4 });

	playerID_ = playerCnt_;
	if (lpNetWork.GetNetWorkMode() == NetWorkMode::Host)
	{
		if (!(playerID_ % 2))
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
		if (!(playerID_ % 2))
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
	playerCnt_++;
}

void Player::UpdateMyself(void)
{
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

	// 座標情報の送信
	MesPacket plData;
	plData.resize(3);
	plData[0].iData = playerID_;
	plData[1].iData = pos_.x;
	plData[2].iData = pos_.y;
	lpNetWork.SendMes(plData, MesType::Pos);
}

void Player::UpdateNet(void)
{
	auto data = lpNetWork.PickMes();
	if (data.first.type == MesType::Pos)
	{
		pos_ = { data.second[1].iData, data.second[2].iData };
	}
}
