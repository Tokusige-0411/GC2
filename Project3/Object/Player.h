#pragma once
#include <map>
#include <functional>
#include <list>
#include "Object.h"
#include "../input/Controller.h"

#define UNIT_ID_NUM 5

class Player :
	public Object
{
public:
	Player(int id, Vector2 pos, std::shared_ptr<TileLoader>& mapObj, BaseScene& scene);
	~Player();

	static int fallCnt;

	bool Update(void)override;
	void Draw(void)override;
	int GetPlayerID(void);

private:
	void Init(void)override;
	bool UpdateDef(void);
	bool UpdateNet(void);
	bool UpdateAuto(void);

	int UseBomb(void);

	int speed_;
	std::unique_ptr<Controller> input_;					// ｲﾝｯﾌﾟﾄ情報
	std::map<Dir, bool> dirPermit_;						// 各方向行けるか

	int playerID_;										// ﾌﾟﾚｲﾔｰ識別番号

	std::list<int> bombList_;
	BaseScene& scene_;
};

