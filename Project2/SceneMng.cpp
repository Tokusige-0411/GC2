#include<Dxlib.h>
#include"SceneMng.h"

void SceneMng::Run()
{
	while (!ProcessMessage() || CheckHitKey(KEY_INPUT_ESCAPE))
	{
		Draw();
	}
}

void SceneMng::Draw()
{
	ClsDrawScreen();

	ScreenFlip();
}

bool SceneMng::SysInit()
{
	ChangeWindowMode(true);
	SetGraphMode(840, 600, 16);

	if (DxLib_Init() == -1)
	{
		return false;
	}

	SetDrawScreen(DX_SCREEN_BACK);
	return true;
}

SceneMng::SceneMng()
{
	SysInit();
}

SceneMng::~SceneMng()
{
}
