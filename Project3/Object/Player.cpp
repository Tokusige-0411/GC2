#include <DxLib.h>
#include <ImageMng.h>
#include "Player.h"
#include "../NetWark/NetWark.h"
#include "../input/KeyInput.h"
#include "../input/PadInput.h"
#include "../input/INPUT_ID.h"
#include "../Scene/GameScene.h"

#include "../_debug/_DebugConOut.h"

int Player::fallCnt = 0;

Player::Player(int id, Vector2 pos, std::shared_ptr<TileLoader>& mapObj, BaseScene& scene) : scene_(scene)
{
	pos_ = pos;
	playerID_ = id;
	mapObj_ = mapObj;
	Init();
}

Player::~Player()
{
}

void Player::Init(void)
{
	lpImageMng.GetID("player", "image/Player_Anim.png", { 32, 51 }, { 4, 4 });

	if (lpNetWork.GetNetWorkMode() == NetWorkMode::Host)
	{
		input_ = std::make_unique<KeyInput>();
		input_->SetUp(0);
		if (playerID_ / UNIT_ID_NUM == 1)
		{
			update_ = std::bind(&Player::UpdateNet, this);
		}
	}
	if (lpNetWork.GetNetWorkMode() == NetWorkMode::Guest)
	{
		input_ = std::make_unique<PadInput>();
		input_->SetUp(0);
		if (playerID_ / UNIT_ID_NUM == 0)
		{
			update_ = std::bind(&Player::UpdateNet, this);
		}
	}
	if (lpNetWork.GetNetWorkMode() == NetWorkMode::Offline)
	{
		input_ = std::make_unique<KeyInput>();
		input_->SetUp(0);
	}

	if ((playerID_ / UNIT_ID_NUM) >= 2)
	{
		update_ = std::bind(&Player::UpdateAuto, this);
	}

	dir_ = Dir::Right;
	dirPermit_[Dir::Up] = true;
	dirPermit_[Dir::Right] = true;
	dirPermit_[Dir::Down] = true;
	dirPermit_[Dir::Left] = true;
	animCnt_ = 0;
	speed_ = 4;

	lpNetWork.AddMesList(playerID_ / UNIT_ID_NUM, mesList_, mtx_);
}

bool Player::Update(void)
{
	// かべﾁｪｯｸ
	auto mapData = mapObj_->GetMapData();
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
		dirPermit_[Dir::Up] = CheckWall({ chipPos.x, chipPos.y - 1 });
		dirPermit_[Dir::Down] = CheckWall({ chipPos.x, chipPos.y + 1 });
		dirPermit_[Dir::Right] = CheckWall({ chipPos.x + 1, chipPos.y });
		dirPermit_[Dir::Left] = CheckWall({ chipPos.x - 1, chipPos.y });
	}
	return update_();
}

void Player::Draw(void)
{
	DrawRotaGraph(pos_.x + 16, pos_.y - 6, 1.0, 0.0, IMAGE_ID("player")[(2 + (animCnt_ / 15 % 2)) * 4 + static_cast<int>(dir_)], true);
	animCnt_++;
}

int Player::GetPlayerID(void)
{
	return playerID_;
}

bool Player::UpdateDef(void)
{
	(*input_)();

	auto data = input_->GetContData();

	if (data[INPUT_ID::UP][static_cast<int>(Trg::Now)])
	{
		pos_.y -= speed_;
	}
	if (data[INPUT_ID::RIGHT][static_cast<int>(Trg::Now)])
	{
		pos_.x += speed_;
	}
	if (data[INPUT_ID::DOWN][static_cast<int>(Trg::Now)])
	{
		pos_.y += speed_;
	}
	if (data[INPUT_ID::LEFT][static_cast<int>(Trg::Now)])
	{
		pos_.x -= speed_;
	}

	// ボム設置
	if ((data[INPUT_ID::SET_BOMB][static_cast<int>(Trg::Now)]) && (!data[INPUT_ID::SET_BOMB][static_cast<int>(Trg::Old)]))
	{
		MesPacket bombData;
		bombData.resize(4);
		bombData[0].iData = playerID_;
		bombData[1].iData = 0;
		bombData[2].iData = pos_.x;
		bombData[3].iData = pos_.y;
		lpNetWork.SendMes(bombData, MesType::Set_Bomb);
		dynamic_cast<GameScene&>(scene_).SetBombObj(playerID_, 0, pos_, true);
	}

	// 座標情報の送信
	MesPacket plData;
	plData.resize(4);
	plData[0].iData = playerID_;
	plData[1].iData = pos_.x;
	plData[2].iData = pos_.y;
	plData[3].iData = static_cast<int>(dir_);
	lpNetWork.SendMes(plData, MesType::Pos);

	return true;
}

bool Player::UpdateNet(void)
{
	std::lock_guard<std::mutex> lock(mtx_);
	if (Object::IsPickUp())
	{
		while (Object::IsPickUp())
		{
			auto data = Object::PickUp();
			if (data.first == MesType::Pos)
			{
				pos_ = Vector2{ data.second[1].iData, data.second[2].iData };
				dir_ = static_cast<Dir>(data.second[3].iData);
			}
			if (data.first == MesType::Set_Bomb)
			{
				dynamic_cast<GameScene&>(scene_).SetBombObj(playerID_, 0, pos_, true);
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool Player::UpdateAuto(void)
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

	// 座標情報の送信
	MesPacket plData;
	plData.resize(4);
	plData[0].iData = playerID_;
	plData[1].iData = pos_.x;
	plData[2].iData = pos_.y;
	plData[3].iData = static_cast<int>(dir_);
	lpNetWork.SendMes(plData, MesType::Pos);
	return true;
}

bool Player::UseBomb(int bombID)
{
	return true;
}
