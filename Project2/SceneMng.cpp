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

	ScreenFlip();
}

int SceneMng::GetFrameCount()
{
	return _frame;
}

std::mt19937 SceneMng::GetMt()
{
	return _mt;
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
	_frame = 0;
}

SceneMng::~SceneMng()
{
}
