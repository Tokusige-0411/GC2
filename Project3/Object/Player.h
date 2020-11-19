#pragma once
#include <map>
#include <functional>
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

	bool UseBomb(int bombID);

	int speed_;
	std::unique_ptr<Controller> input_;					// ²İ¯ÌßÄî•ñ
	std::map<Dir, bool> dirPermit_;						// Še•ûŒüs‚¯‚é‚©

	int playerID_;										// ÌßÚ²Ô°¯•Ê”Ô†

	std::list<int> bombList;
	BaseScene& scene_;
};

