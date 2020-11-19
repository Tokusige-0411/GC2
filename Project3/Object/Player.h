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
	std::unique_ptr<Controller> input_;					// �ݯ��ď��
	std::map<Dir, bool> dirPermit_;						// �e�����s���邩

	int playerID_;										// ��ڲ԰���ʔԍ�

	std::list<int> bombList;
	BaseScene& scene_;
};

