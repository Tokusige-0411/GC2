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

	std::unique_ptr<Controller> input_;         // ｲﾝｯﾌﾟﾄ情報
	std::map<Dir, bool> dirPermit_;				// 各方向行けるか

	static int playerCnt_;
	int playerID_;
};

