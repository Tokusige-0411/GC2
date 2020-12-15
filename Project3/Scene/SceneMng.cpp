#include<Dxlib.h>
#include<algorithm>
#include"../_debug/_DebugDispOut.h"
#include"../_debug/_DebugConOut.h"
#include"SceneMng.h"
#include"LoginScene.h"
#include"GameScene.h"

void SceneMng::Run()
{
	activeScene_ = std::make_unique<LoginScene>();
	befor_ = std::chrono::system_clock::now();
	_dbgSetDrawPosFps(FPS_SIDE::LEFT, FPS_VER::TOP);
	while (!ProcessMessage())
	{
		now_ = std::chrono::system_clock::now();
		double delta = std::chrono::duration_cast<std::chrono::milliseconds>(now_ - befor_).count();
		befor_ = now_;
		_dbgStartDraw();
		activeScene_ = (*activeScene_).Update(std::move(activeScene_), delta);
		Draw();
		frame_++;
	}
}

void SceneMng::Draw()
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();

	activeScene_->BaseScene::Draw();
	_dbgDrawFPS();
	_dbgAddDraw();

	ScreenFlip();
}

const Vector2 SceneMng::GetScreenSize(void)
{
	return screenSize_;
}

const Vector2 SceneMng::GetScreenCenter(void)
{
	return screenCenter_;
}

const int& SceneMng::GetFrameCnt(void)
{
	return frame_;
}

const time_point& SceneMng::GetTime(void)
{
	return now_;
}

void SceneMng::AddDrawQue(DrawQueT dQue)
{
	drawList_.emplace_back(dQue);
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
	SetAlwaysRunFlag(true);
	return true;
}

SceneMng::SceneMng() : screenSize_{ 800, 600 }, screenCenter_{ screenSize_.x / 2, screenSize_.y / 2 }
{
	SysInit();
	frame_ = 0;
}

SceneMng::~SceneMng()
{
}
