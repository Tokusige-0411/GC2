#pragma once
#include "Object.h"
#include <Vector2.h>

class Bomb :
	public Object
{
public:
	Bomb(int owner, int self, Vector2 pos);
	~Bomb();

	bool Update(void)override;
	bool UpdateDef(void)override;
	void Draw(void)override;

private:
	void Init(void)override;

	int ownerID_;
	int selfID_;
};

