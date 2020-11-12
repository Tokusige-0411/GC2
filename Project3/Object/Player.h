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

	std::unique_ptr<Controller> input_;						// ²İ¯ÌßÄî•ñ
	std::map<Dir, bool> dirPermit_;							// Še•ûŒüs‚¯‚é‚©

	std::function<void(void)> update_;			// ¹Ş½Ä‚ÆÎ½Ä‚Å±¯ÌßÃŞ°Ä‚ğ•ª‚¯‚é

	static int playerCnt_;
	int playerID_;
};

