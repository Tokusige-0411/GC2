#include "Bomb.h"
#include <imageMng.h>
#include "../Scene/GameScene.h"
#include "Player.h"

Bomb::Bomb(int owner, int self, int blastLength, Vector2 pos, sharedMap& mapObj,BaseScene& scene) : scene_(scene)
{
	ownerID_ = owner;
	objectID_ = self;
	pos_ = pos;
	startTime_ = std::chrono::system_clock::now();
	blastLength_ = blastLength;
	mapObj_ = mapObj;
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
	auto Set = [&]() {
		alive_ = false;
		deth_ = true;
		mapObj_->SetFireGenerator(pos_, blastLength_);
		auto player = dynamic_cast<GameScene&>(scene_).GetPlayerObj(ownerID_);
		if (player)
		{
			dynamic_cast<Player&>(*player).BombReload(objectID_);
		}
	};
	if (std::chrono::duration_cast<std::chrono::milliseconds>(endTime_ - startTime_).count() >= 3000 ||
		mapObj_->GetFireMap(pos_) > 0.0)
	{
		Set();
	}

	mapObj_->SetBombMap(pos_, alive_);

	return true;
}

void Bomb::Draw(void)
{
	DrawRotaGraph(pos_.x + 16, pos_.y + 16, 1.0, 0.0, IMAGE_ID(ObjectID::Bomb)[0], true);
}

void Bomb::Init(void)
{
	lpImageMng.GetID(ObjectID::Bomb, "image/bomb.png", { 32, 32 }, { 2, 7 });
	AnimVector data;
	data.emplace_back(IMAGE_ID(ObjectID::Bomb)[0], 10);
	data.emplace_back(IMAGE_ID(ObjectID::Bomb)[2], 20);
	animMap_.emplace(AnimState::Normal, data);
	data.clear();

	data.emplace_back(IMAGE_ID(ObjectID::Bomb)[1], 3);
	data.emplace_back(IMAGE_ID(ObjectID::Bomb)[3], 6);
	data.emplace_back(IMAGE_ID(ObjectID::Bomb)[5], 9);
	data.emplace_back(IMAGE_ID(ObjectID::Bomb)[7], 12);
	data.emplace_back(IMAGE_ID(ObjectID::Bomb)[9], 15);
	data.emplace_back(IMAGE_ID(ObjectID::Bomb)[11], 18);
	data.emplace_back(IMAGE_ID(ObjectID::Bomb)[13], 21);
	data.emplace_back(-1, 24);
	animMap_.emplace(AnimState::Deth, data);
}
