#pragma once
#include "Object.h"

class Player :
	public Object
{
public:
	Player(Vector2 pos);
	~Player();

	void Update(void)override;
	void Draw(void)override;

private:
	void Init(void)override;

	static int playerCnt;
	int playerID;
};

