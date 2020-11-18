#pragma once
#include <map>
#include <functional>
#include "Object.h"
#include "../input/Controller.h"

class Player :
	public Object
{
public:
	Player(int id, Vector2 pos);
	~Player();

	static int fallCnt;

	bool Update(MapData& mapData)override;
	void Draw(void)override;
	int GetPlayerID(void);

private:
	void Init(void)override;
	bool UpdateMyself(void);
	bool UpdateNet(void);

	int speed_;
	std::unique_ptr<Controller> input_;					// ²İ¯ÌßÄî•ñ
	std::map<Dir, bool> dirPermit_;						// Še•ûŒüs‚¯‚é‚©

	std::function<bool(void)> update_;					// ¹Ş½Ä‚ÆÎ½Ä‚Å±¯ÌßÃŞ°Ä‚ğ•ª‚¯‚é
	MesPacket plData_;
	int playerID_;										// ÌßÚ²Ô°¯•Ê”Ô†
};

