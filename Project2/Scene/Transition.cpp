#include <Dxlib.h>
#include "Transition.h"
#include "../SceneMng.h"

Transition::Transition(unique_Base befor, unique_Base after)
{
	drawScene_ = std::move(befor);
	afterScene_ = std::move(after);
	Init();
}

Transition::~Transition()
{
}

unique_Base Transition::Update(unique_Base own)
{
	// Ìª°ÄÞ²ÝEÌª°ÄÞ±³Ä‚³‚¹‚é
	if (fadeCnt_)
	{
		if (fadeCnt_ == 256)
		{
			drawScene_ = std::move(afterScene_);
		}
		lpSceneMng.AddDrawQue({
		black_,
		lpSceneMng.GetScreenCenter().x, lpSceneMng.GetScreenCenter().y,
		0.0, 1.0, 0.0f, Layer::Ex,
		DX_BLENDMODE_ALPHA, 255 - abs(fadeCnt_ - 255), DrawType::Image });
		fadeCnt_ -= 2;
	}
	else
	{
		return std::move(drawScene_);
	}
	return std::move(own);
}

void Transition::Draw(void)
{
	drawScene_->Draw();
}

bool Transition::Init(void)
{
	black_ = LoadGraph("image/BlackOut.png");
	fadeCnt_ = 510;
	return true;
}
