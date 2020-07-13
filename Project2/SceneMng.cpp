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
