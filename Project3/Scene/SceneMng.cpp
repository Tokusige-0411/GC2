#include<Dxlib.h>
#include<algorithm>
#include"../_debug/_DebugDispOut.h"
#include"SceneMng.h"
#include"TitleScene.h"

void SceneMng::Run()
{
	activeScene_ = std::make_unique<TitleScene>();
	while (!ProcessMessage())
	{
		activeScene_ = (*activeScene_).Update(std::move(activeScene_));
		Draw();
		frame_++;
	}
}

void SceneMng::Draw()
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();

	activeScene_->Draw();

	ScreenFlip();
}

const Vector2 SceneMng::GetScreenCenter(void)
{
	return screenCenter_;
}

int SceneMng::GetFrameCnt(void)
{
	return frame_;
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
	return true;
}

SceneMng::SceneMng() : screenSize_{ 672, 544 }, screenCenter_{ screenSize_.x / 2, screenSize_.y / 2 }
{
	SysInit();
	frame_ = 0;
}

SceneMng::~SceneMng()
{
}
