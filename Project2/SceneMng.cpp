#include<Dxlib.h>
#include<algorithm>
#include"EffekseerForDXLib.h"
#include"_debug/_DebugDispOut.h"
#include"SceneMng.h"
#include"Scene/GameScene.h"
#include"Scene/TitleScene.h"
#include"EffectCtl.h"

void SceneMng::Run()
{
	activeScene_ = std::make_unique<TitleScene>();
	while ((!ProcessMessage()) && !(gameExit_))
	{
		gameExit_ = CheckHitKey(KEY_INPUT_ESCAPE);
		_DebugDispOut::GetInstance().WaitMode();
		activeScene_ = (*activeScene_).Update(std::move(activeScene_));
		activeScene_->Draw();
		lpEffectCtl.Update();
		Draw();
		// drawListÇÃèâä˙âª
		auto itl = std::remove_if(drawList_.begin(), drawList_.end(), [&](auto dque) {
			DrawType type = std::get<static_cast<int>(DrawQue::Type)>(dque);
			if (type == DrawType::Effect)
			{
				int handle = std::get<static_cast<int>(DrawQue::Handle)>(dque);
				if (IsEffekseer2DEffectPlaying(handle) == 0)
				{
					return false;
				}
			}
			return true;
		});
		drawList_.erase(itl, drawList_.end());
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
	for (auto& dque : drawList_)
	{
		int x, y, handle;
		double rad ,ex;
		DrawType type;

		std::tie(handle, x, y, rad, ex, std::ignore, std::ignore, blendMode, blendModeNum, type) = dque;

		// imageÇ©EffectÇ©îªífÇµÇƒÇªÇÍÇºÇÍåƒÇ‘DrawÇïœÇ¶ÇÈ
		// Ç–Ç∆Ç‹Ç∏imageÇæÇØ
		SetDrawBlendMode(blendMode, blendModeNum);
		(this->*drawSet_[type])(handle, {x, y}, rad, ex);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
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

void SceneMng::SetGameExit(void)
{
	gameExit_ = true;
}

int SceneMng::GetFrameCnt(void)
{
	return frame_;
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

void SceneMng::DrawImage(int handle, const Vector2 pos, double rad, double ex)
{
	DrawRotaGraph(pos.x, pos.y, ex, rad, handle, true, false);
}

void SceneMng::DrawEffect(int handle, const Vector2 pos, double rad, double ex)
{
	lpEffectCtl.Draw(handle, pos);
}

SceneMng::SceneMng() : screenSize_{ 800, 600 }, screenCenter_{ screenSize_.x / 2, screenSize_.y / 2 }
{
	SysInit();
	drawSet_.try_emplace(DrawType::Image, &SceneMng::DrawImage);
	drawSet_.try_emplace(DrawType::Effect, &SceneMng::DrawEffect);
	frame_ = 0;
	gameExit_ = 0;
}

SceneMng::~SceneMng()
{
}
