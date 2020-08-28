#include<Dxlib.h>
#include<algorithm>
#include"_debug/_DebugDispOut.h"
#include"SceneMng.h"
#include"Scene/GameScene.h"
#include"EffectCtl.h"

void SceneMng::Run()
{
	activeScene_ = std::make_unique<GameScene>();
	while (!ProcessMessage() || CheckHitKey(KEY_INPUT_ESCAPE))
	{
		_DebugDispOut::GetInstance().WaitMode();
		activeScene_ = (*activeScene_).Update(std::move(activeScene_));
		activeScene_->Draw();
		lpEffectCtl.Update();
		Draw();
		drawList_.clear();
		frame_++;
	}
}

void SceneMng::Draw()
{
	std::sort(drawList_.begin(), drawList_.end(), [](const DrawQueT& x, const DrawQueT& y) {
		return std::tie(std::get<static_cast<int>(DrawQue::Layer)>(x), std::get<static_cast<int>(DrawQue::ZOrder)>(x))
			< std::tie(std::get<static_cast<int>(DrawQue::Layer)>(y), std::get<static_cast<int>(DrawQue::ZOrder)>(y));
		});


	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();

	int blendMode = DX_BLENDMODE_NOBLEND;
	int blendModeNum = 255;
	SetDrawBlendMode(blendMode, blendModeNum);
	for (auto dque : drawList_)
	{
		int x, y, handle;
		double rad;
		DrawType type;

		std::tie(handle, x, y, rad, std::ignore, std::ignore, blendMode, blendModeNum, type) = dque;

		// image‚©Effect‚©”»’f‚µ‚Ä‚»‚ê‚¼‚êŒÄ‚ÔDraw‚ð•Ï‚¦‚é
		// ‚Ð‚Æ‚Ü‚¸image‚¾‚¯
		SetDrawBlendMode(blendMode, blendModeNum);
		DrawRotaGraph(x, y, 1.0, rad, handle, true, false);
	}
	//lpEffectCtl.Draw();

	ScreenFlip();
}

bool SceneMng::AddDrawQue(DrawQueT que)
{
	drawList_.emplace_back(que);
	return true;
}

const Vector2 SceneMng::GetScreenCenter(void)
{
	return screenCenter_;
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

void SceneMng::DrawImage(int handle, const Vector2 pos, double rad)
{
	DrawRotaGraph(pos.x, pos.y, 1.0, rad, handle, true, false);
}

void SceneMng::DrawEffect(int handle, const Vector2 pos, double rad)
{
}

SceneMng::SceneMng() : screenSize_{ 800, 600 }, screenCenter_{ screenSize_.x / 2, screenSize_.y / 2 }
{
	SysInit();
	//drawSet_.try_emplace(DrawType::Image, DrawImage());
	frame_ = 0;
}

SceneMng::~SceneMng()
{
}
