#include "Bomb.h"
#include <imageMng.h>

Bomb::Bomb(int owner, int self, Vector2 pos)
{
	ownerID_ = owner;
	selfID_ = self;
	pos_ = pos;
	Init();
}

Bomb::~Bomb()
{
}

bool Bomb::Update(void)
{
	return true;
}

bool Bomb::UpdateDef(void)
{
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
