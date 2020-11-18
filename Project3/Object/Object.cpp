#include "Object.h"

Object::Object()
{
	animCnt_ = 0;
	dir_ = Dir::Up;
}

Object::Object(int id, Vector2 pos)
{
	animCnt_ = 0;
	dir_ = Dir::Up;
}

Object::~Object()
{
}

bool Object::Update(MapData& mapData)
{
	return true;
}

void Object::Draw(void)
{
}

Vector2 Object::GetPos(void)
{
	return pos_;
}

bool Object::IsPickUp(void)
{
	return mesList_.size();
}

MesPacket Object::PickUp(void)
{
	auto data = mesList_.front();
	mesList_.erase(mesList_.begin());
	return data;
}

void Object::Init(void)
{
}
