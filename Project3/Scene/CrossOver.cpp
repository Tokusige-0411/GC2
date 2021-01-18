#include <DxLib.h>
#include "CrossOver.h"

CrossOver::CrossOver(unique_Base next, unique_Base old)
{
	nextScene_ = std::move(next);
	oldScene_ = std::move(old);
	Init();
}

CrossOver::~CrossOver()
{
}

bool CrossOver::Init()
{
	fadeCnt_ = 0;
	return true;
}

unique_Base CrossOver::Update(unique_Base own, double delta)
{
	Draw();
	if (fadeCnt_ > 255)
	{
		own = std::move(nextScene_);
	}
	fadeCnt_++;
	return own;
}

void CrossOver::Draw(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeCnt_);
	nextScene_->Draw();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - fadeCnt_);
	oldScene_->Draw();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}
