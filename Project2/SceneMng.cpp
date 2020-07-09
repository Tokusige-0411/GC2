#include<Dxlib.h>
#include"SceneMng.h"
#include"GameScene.h"

void SceneMng::Run()
{
	_activeScene = std::make_unique<GameScene>();
	while (!ProcessMessage() || CheckHitKey(KEY_INPUT_ESCAPE))
	{
		_activeScene = (*_activeScene).Update(std::move(_activeScene));
		Draw();
		_frame++;
	}
}

void SceneMng::Draw()
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();

	_activeScene->Draw();

	DrawBox(320, 120, 360, 200, 0xffffff, false);
	DrawBox(440, 120, 480, 200, 0xffffff, false);

	ScreenFlip();
}

int SceneMng::GetFrameCount()
{
	return _frame;
}

const std::map<INPUT_ID, int>& SceneMng::GetKeyList(PLAYER_ID player)
{
	return _keyList.at(player);
}

bool SceneMng::SysInit()
{
	ChangeWindowMode(true);
	SetGraphMode(_screenSize.x, _screenSize.y, 16);

	if (DxLib_Init() == -1)
	{
		return false;
	}

	SetDrawScreen(DX_SCREEN_BACK);
	return true;
}

SceneMng::SceneMng() : _screenSize{800, 600}
{
	SysInit();

	_keyList[PLAYER_ID::ONE].try_emplace(INPUT_ID::UP, KEY_INPUT_W);
	_keyList[PLAYER_ID::ONE].try_emplace(INPUT_ID::DOWN, KEY_INPUT_S);
	_keyList[PLAYER_ID::ONE].try_emplace(INPUT_ID::LEFT, KEY_INPUT_A);
	_keyList[PLAYER_ID::ONE].try_emplace(INPUT_ID::RIGHT, KEY_INPUT_D);
	_keyList[PLAYER_ID::ONE].try_emplace(INPUT_ID::R_ROTA, KEY_INPUT_V);
	_keyList[PLAYER_ID::ONE].try_emplace(INPUT_ID::L_LOTA, KEY_INPUT_B);

	_keyList[PLAYER_ID::TWO].try_emplace(INPUT_ID::UP, KEY_INPUT_UP);
	_keyList[PLAYER_ID::TWO].try_emplace(INPUT_ID::DOWN, KEY_INPUT_DOWN);
	_keyList[PLAYER_ID::TWO].try_emplace(INPUT_ID::LEFT, KEY_INPUT_LEFT);
	_keyList[PLAYER_ID::TWO].try_emplace(INPUT_ID::RIGHT, KEY_INPUT_RIGHT);
	_keyList[PLAYER_ID::TWO].try_emplace(INPUT_ID::R_ROTA, KEY_INPUT_NUMPAD1);
	_keyList[PLAYER_ID::TWO].try_emplace(INPUT_ID::L_LOTA, KEY_INPUT_NUMPAD2);

	_frame = 0;
}

SceneMng::~SceneMng()
{
}
