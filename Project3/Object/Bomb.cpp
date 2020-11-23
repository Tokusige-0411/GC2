#include "Bomb.h"
#include <imageMng.h>
#include "../Scene/GameScene.h"
#include "Player.h"

Bomb::Bomb(int owner, int self, Vector2 pos, BaseScene& scene) : scene_(scene)
{
	ownerID_ = owner;
	objectID_ = self;
	pos_ = pos;
	startTime_ = std::chrono::system_clock::now();
	Init();
}

Bomb::~Bomb()
{
}

bool Bomb::Update(void)
{
	return update_();
}

bool Bomb::UpdateDef(void)
{
	endTime_ = std::chrono::system_clock::now();
	if (std::chrono::duration_cast<std::chrono::milliseconds>(endTime_ - startTime_).count() >= 3000)
	{
		alive_ = false;
		auto& player = dynamic_cast<GameScene&>(scene_).GetPlayerObj(ownerID_);
		dynamic_cast<Player&>(*player).BombReload(objectID_);
	}
	return true;
}

void Bomb::Draw(void)
{
	DrawRotaGraph(pos_.x, pos_.y, 1.0, 0.0, IMAGE_ID("bomb")[0], true);
}

void Bomb::Init(void)
{
	lpImageMng.GetID("bomb", "image/bomb.png", { 32, 32 }, { 2, 7 });
}
