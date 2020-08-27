#include<Dxlib.h>
#include"_debug/_DebugDispOut.h"
#include"SceneMng.h"
#include"Scene/GameScene.h"
#include"EffectCtl.h"

void SceneMng::Run()
{
	_activeScene = std::make_unique<GameScene>();
	while (!ProcessMessage() || CheckHitKey(KEY_INPUT_ESCAPE))
	{
		_DebugDispOut::GetInstance().WaitMode();
		_activeScene = (*_activeScene).Update(std::move(_activeScene));
		lpEffectCtl.Update();
		Draw();
		_frame++;
	}
}

void SceneMng::Draw()
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();

	_activeScene->Draw();
	lpEffectCtl.Draw();

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
	SetGraphMode(screenSize_.x, screenSize_.y, 16);

	if (DxLib_Init() == -1)
	{
		return false;
	}

	SetDrawScreen(DX_SCREEN_BACK);
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	lpEffectCtl.Init(Vector2(screenSize_.x, screenSize_.y));
	return true;
}

SceneMng::SceneMng() : screenSize_{800, 600}
{
	SysInit();
	_frame = 0;
}

SceneMng::~SceneMng()
{
}
