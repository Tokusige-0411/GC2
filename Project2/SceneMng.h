#pragma once
#include <memory>
#include <vector>
#include <map>
#include "BaseScene.h"
#include "Field.h"
#include "InputState.h"
#include "Vector2.h"
#include "INPUT_ID.h"
#include "PLAYER_NUM.h"

#define lpSceneMng SceneMng::GetInstance()

class SceneMng
{
public:
	static SceneMng& GetInstance()
	{
		static SceneMng s_Instance;
		return s_Instance;
	}

	void Run();
	void Draw();

	int GetFrameCount();
	const std::map<INPUT_ID, int>& GetKeyList(PLAYER_NUM player);

private:
	bool SysInit();
	SceneMng();
	~SceneMng();

	const Vector2 _screenSize;

	unique_Base _activeScene;

	std::map<PLAYER_NUM, std::map<INPUT_ID, int>> _keyList;

	int _frame;
};

