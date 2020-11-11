#include <DxLib.h>
#include <ImageMng.h>
#include "Player.h"

int Player::playerCnt = 0;

Player::Player(Vector2 pos)
{
	pos_ = pos;
	dir_ = 0;
	Init();
}

Player::~Player()
{
}

void Player::Update(void)
{
}

void Player::Draw(void)
{
	DrawGraph(pos_.x, pos_.y, IMAGE_ID("player")[0], true);
}

void Player::Init(void)
{
	playerID = playerCnt;
	playerCnt++;

	lpImageMng.GetID("player", "image/Player.png");
}
