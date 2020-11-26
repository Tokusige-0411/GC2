#pragma once
#include <map>
#include <functional>
#include <list>
#include "Object.h"
#include "../input/Controller.h"

#define UNIT_ID_NUM 5

class BaseScene;

using InputFunc = std::function<bool(ContData&, bool)>;

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
	void BombReload(int self);

private:
	void Init(void)override;
	bool UpdateDef(void);
	bool UpdateNet(void);
	bool UpdateAuto(void);

	void SetInputMoveList(void);

	bool CheckWall(Dir dir);

	int UseBomb(void);

	int speed_;
	int blastLength_;

	std::unique_ptr<Controller> input_;					// �ݯ��ď��
	std::map<Dir, bool> dirPermit_;						// �e�����s���邩

	std::list<int> bombList_;

	std::list<INPUT_ID> pushKeyList_;
	std::list<InputFunc> inputMoveList_;
	BaseScene& scene_;
};

