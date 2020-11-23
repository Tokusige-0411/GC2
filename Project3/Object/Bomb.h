#pragma once
#include <chrono>
#include "Object.h"
#include <Vector2.h>

class Bomb :
	public Object
{
public:
	Bomb(int owner, int self, Vector2 pos, BaseScene& scene);
	~Bomb();

	bool Update(void)override;
	bool UpdateDef(void)override;
	void Draw(void)override;

private:
	void Init(void)override;

	int ownerID_;
	std::chrono::system_clock::time_point startTime_;
	std::chrono::system_clock::time_point endTime_;
	BaseScene& scene_;
};

