#include <DxLib.h>
#include <ImageMng.h>
#include "Player.h"
#include "../NetWark/NetWark.h"
#include "../input/KeyInput.h"
#include "../input/PadInput.h"
#include "../input/INPUT_ID.h"
#include "../Scene/GameScene.h"

#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

int Player::fallCnt = 0;

Player::Player(int id, Vector2 pos, std::shared_ptr<TileLoader>& mapObj, BaseScene& scene) : scene_(scene)
{
	pos_ = pos;
	objectID_ = id;
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
		if (objectID_ / UNIT_ID_NUM == 0)
		{
			input_ = std::make_unique<KeyInput>();
			input_->SetUp(0);
			SetInputMoveList();
		}
		else if (objectID_ / UNIT_ID_NUM == 1)
		{
			update_ = std::bind(&Player::UpdateNet, this);
		}
		else
		{
			update_ = std::bind(&Player::UpdateAuto, this);
		}
	}

	if (lpNetWork.GetNetWorkMode() == NetWorkMode::Guest)
	{
		if (objectID_ / UNIT_ID_NUM == 0)
		{
			update_ = std::bind(&Player::UpdateNet, this);
		}
		else if (objectID_ / UNIT_ID_NUM == 1)
		{
			input_ = std::make_unique<PadInput>();
			input_->SetUp(0);
			SetInputMoveList();
		}
		else
		{
			update_ = std::bind(&Player::UpdateAuto, this);
		}
	}
	if (lpNetWork.GetNetWorkMode() == NetWorkMode::Offline)
	{
		if (objectID_ / UNIT_ID_NUM == 0)
		{
			input_ = std::make_unique<KeyInput>();
			input_->SetUp(0);
			SetInputMoveList();
		}
		else
		{
			update_ = std::bind(&Player::UpdateAuto, this);
		}
	}

	if ((objectID_ / UNIT_ID_NUM) >= 2)
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

	bombList_.push_back(1);
	bombList_.push_back(2);
	bombList_.push_back(3);
	bombList_.push_back(4);

	lpNetWork.AddMesList(objectID_ / UNIT_ID_NUM, mesList_, mtx_);
}

bool Player::Update(void)
{
	return update_();
}

void Player::Draw(void)
{
	DrawRotaGraph(pos_.x + 16, pos_.y, 1.0, 0.0, IMAGE_ID("player")[(2 + (animCnt_ / 15 % 2)) * 4 + static_cast<int>(dir_)], true);
	_dbgDrawBox(pos_.x, pos_.y, pos_.x + 32, pos_.y + 32, 0xffffff, false);
	animCnt_++;
}

int Player::GetPlayerID(void)
{
	return objectID_;
}

bool Player::UpdateDef(void)
{
	(*input_)();

	auto data = input_->GetContData();
	for (auto inputMove = inputMoveList_.begin(); inputMove != inputMoveList_.end(); inputMove++)
	{
		if ((*inputMove)(data, true))
		{
			inputMoveList_.splice(inputMoveList_.begin(), inputMoveList_, inputMove);
			inputMoveList_.sort([&](InputFunc funcA, InputFunc funcB) {return (funcA(data, false) < funcB(data, false)); });
			break;
		}
	}

	// ボム設置
	if ((data[INPUT_ID::SET_BOMB][static_cast<int>(Trg::Now)]) && (!data[INPUT_ID::SET_BOMB][static_cast<int>(Trg::Old)]))
	{
		int bombFlag = UseBomb();
		if (bombFlag != -1)
		{
			MesPacket bombData;
			bombData.resize(6);
			bombData[0].iData = objectID_;
			bombData[1].iData = bombFlag;
			bombData[2].iData = pos_.x;
			bombData[3].iData = pos_.y;
			auto now = TimeUnion{ std::chrono::system_clock::now() };
			bombData[4].iData = now.data[0];
			bombData[5].iData = now.data[1];
			lpNetWork.SendMes(bombData, MesType::Set_Bomb);
			dynamic_cast<GameScene&>(scene_).SetBombObj(objectID_, bombFlag, pos_, true);
		}
	}

	// 座標情報の送信
	MesPacket plData;
	plData.resize(4);
	plData[0].iData = objectID_;
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
				auto now = TimeUnion{ std::chrono::system_clock::now() };
				dynamic_cast<GameScene&>(scene_).SetBombObj(objectID_, 0, pos_, true);
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
	dirPermit_[Dir::Up] = CheckWall({ chipPos.x, chipPos.y - 1 });
	dirPermit_[Dir::Down] = CheckWall({ chipPos.x, chipPos.y + 1 });
	dirPermit_[Dir::Right] = CheckWall({ chipPos.x + 1, chipPos.y });
	dirPermit_[Dir::Left] = CheckWall({ chipPos.x - 1, chipPos.y });
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

	// 座標情報の送信
	MesPacket plData;
	plData.resize(4);
	plData[0].iData = objectID_;
	plData[1].iData = pos_.x;
	plData[2].iData = pos_.y;
	plData[3].iData = static_cast<int>(dir_);
	lpNetWork.SendMes(plData, MesType::Pos);
	return true;
}

void Player::SetInputMoveList(void)
{
	inputMoveList_.push_back([&](ContData& cont, bool move) {
		if (cont[INPUT_ID::UP][static_cast<int>(Trg::Now)])
		{
			dir_ = Dir::Up;
			if (move)
			{
				// 動けるかどうか
				if (CheckWall(dir_))
				{
					pos_.y -= speed_;
					auto tmpPos = pos_.x % 32;
					if (tmpPos)
					{
						if (tmpPos < 16)
						{
							pos_.x = ((pos_.x / 32)) * 32;
						}
						else
						{
							pos_.x = ((pos_.x / 32) + 1) * 32;
						}
					}
					return true;
				}
				return false;
			}
			return true;
		}
		return false;
		});
	inputMoveList_.push_back([&](ContData& cont, bool move) {
		if (cont[INPUT_ID::RIGHT][static_cast<int>(Trg::Now)])
		{
			dir_ = Dir::Right;
			if (move)
			{
				if (CheckWall(dir_))
				{
					pos_.x += speed_;
					auto tmpPos = pos_.y % 32;
					if (tmpPos)
					{
						if (tmpPos < 16)
						{
							pos_.y = ((pos_.y / 32)) * 32;
						}
						else
						{
							pos_.y = ((pos_.y / 32) + 1) * 32;
						}
					}
					return true;
				}
				return false;
			}
			return true;
		}
		return false;
		});
	inputMoveList_.push_back([&](ContData& cont, bool move) {
		if (cont[INPUT_ID::DOWN][static_cast<int>(Trg::Now)])
		{
			dir_ = Dir::Down;
			if (move)
			{
				if (CheckWall(dir_))
				{
					pos_.y += speed_;
					auto tmpPos = pos_.x % 32;
					if (tmpPos)
					{
						if (tmpPos < 16)
						{
							pos_.x = ((pos_.x / 32)) * 32;
						}
						else
						{
							pos_.x = ((pos_.x / 32) + 1) * 32;
						}
					}
					return true;
				}
				return false;
			}
			return true;
		}
		return false;
		});

	inputMoveList_.push_back([&](ContData& cont, bool move) {
		if (cont[INPUT_ID::LEFT][static_cast<int>(Trg::Now)])
		{
			dir_ = Dir::Left;
			if (move)
			{
				if (CheckWall(dir_))
				{
					pos_.x -= speed_;
					auto tmpPos = pos_.y % 32;
					if (tmpPos)
					{
						if (tmpPos < 16)
						{
							pos_.y = ((pos_.y / 32)) * 32;
						}
						else
						{
							pos_.y = ((pos_.y / 32) + 1) * 32;
						}
					}
					return true;
				}
				return false;
			}
			return true;
		}
		return false;
		});
}

bool Player::CheckWall(Dir dir)
{
	Vector2 checkPos = pos_;
	switch (dir)
	{
	case Dir::Up:
		checkPos.x += 16;
		checkPos.y -= 1;
		break;
	case Dir::Right:
		checkPos.x += 33;
		checkPos.y += 16;
		break;
	case Dir::Down:
		checkPos.x += 16;
		checkPos.y += 33;
		break;
	case Dir::Left:
		checkPos.x -= 1;
		checkPos.y += 16;
		break;
	default:
		break;
	}

	if (mapObj_->GetMapData("Obj", checkPos))
	{
		return false;
	}
	return true;
}

int Player::UseBomb(void)
{
	int useBomb = -1;

	if (bombList_.size())
	{
		useBomb = bombList_.front();
		bombList_.pop_front();
	}

	return useBomb;
}

void Player::BombReload(int self)
{
	bombList_.push_back(self);
}
