#include "Object.h"

Object::Object()
{
	Init();
}

Object::Object(int id, Vector2 pos)
{
	Init();
}

Object::~Object()
{
}

bool Object::Update(void)
{
	return true;
}

bool Object::UpdateDef(void)
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
	std::lock_guard<std::mutex> lock(mtx_);
	return mesList_.size();
}

MesPair Object::PickUp(void)
{
	std::lock_guard<std::mutex> lock(mtx_);
	auto data = mesList_.front();
	mesList_.erase(mesList_.begin());
	return data;
}

void Object::Init(void)
{
	dir_ = Dir::Up;
	update_ = std::bind(&Object::UpdateDef, this);
	alive_ = true;
	deth_ = false;
	lost_ = true;
	objectID_ = 0;
	animCnt_ = 0;
}
