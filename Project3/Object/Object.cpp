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

void Object::Update(MapData& mapData)
{
}

void Object::Draw(void)
{
}

Vector2 Object::GetPos(void)
{
	return pos_;
}

void Object::Init(void)
{
}
