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

	std::unique_ptr<Controller> input_;         // �ݯ��ď��
	std::map<Dir, bool> dirPermit_;				// �e�����s���邩

	static int playerCnt_;
	int playerID_;
};

