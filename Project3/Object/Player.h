#pragma once
#include <map>
#include <functional>
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
	void UpdateMyself(void);
	void UpdateNet(void);

	std::unique_ptr<Controller> input_;						// ｲﾝｯﾌﾟﾄ情報
	std::map<Dir, bool> dirPermit_;							// 各方向行けるか

	std::function<void(void)> update_;			// ｹﾞｽﾄとﾎｽﾄでｱｯﾌﾟﾃﾞｰﾄを分ける

	static int playerCnt_;
	int playerID_;
};

