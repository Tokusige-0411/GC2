#pragma once
#include <memory>
#include <vector>
#include <map>
#include <random>
#include "BaseScene.h"
#include "Field.h"
#include "input/InputState.h"
#include "Vector2.h"
#include "input/INPUT_ID.h"
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
	std::mt19937 GetMt();

private:
	bool SysInit();
	SceneMng();
	~SceneMng();

	const Vector2 _screenSize;

	unique_Base _activeScene;									// “®‚¢‚Ä‚¢‚é¼°Ý

	int _frame;

	std::mt19937 _mt;
};

