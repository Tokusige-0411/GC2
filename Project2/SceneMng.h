#pragma once
#include <memory>
#include <vector>
#include <map>
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
private:
	bool SysInit();
	SceneMng();
	~SceneMng();

	const Vector2 _screenSize;

	unique_Base _activeScene;									// 動いているｼｰﾝ

	int _frame;
};

