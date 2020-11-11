#pragma once
#include <map>
#include "Object.h"
#include "../input/Controller.h"

class Player :
	public Object
{
public:
	Player(Vector2 pos);
	~Player();

	void Update(MapData& mapData)override;
	void Draw(void)override;

private:
	void Init(void)override;

	std::unique_ptr<Controller> input_;         // ²İ¯ÌßÄî•ñ
	std::map<Dir, bool> dirPermit_;				// Še•ûŒüs‚¯‚é‚©

	static int playerCnt_;
	int playerID_;
};

