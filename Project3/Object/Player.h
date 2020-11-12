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

	std::unique_ptr<Controller> input_;						// �ݯ��ď��
	std::map<Dir, bool> dirPermit_;							// �e�����s���邩

	std::function<void(void)> update_;			// �޽Ă�νĂű����ްĂ𕪂���

	static int playerCnt_;
	int playerID_;
};

