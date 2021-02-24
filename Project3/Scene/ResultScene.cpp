#include <DxLib.h>
#include "ResultScene.h"
#include "LoginScene.h"
#include "CrossOver.h"

ResultScene::ResultScene()
{
	Init();
}

ResultScene::~ResultScene()
{
}

bool ResultScene::Init()
{
	result_ = lpNetWork.GetResult();
	return true;
}

unique_Base ResultScene::Update(unique_Base own, double delta)
{
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		own = std::make_unique<CrossOver>(std::make_unique<LoginScene>(), std::move(own));
	}
	DrawOwnScreen();
	return std::move(own);
}

void ResultScene::Draw(void)
{
	DrawString(100, 100, "Œ‹‰Ê”­•\", 0xffffff);
	for (int i = 0; i < result_.size(); i++)
	{
		if (result_[i].iData == -1)
		{
			DrawFormatString(100, 150 + (i * 40), 0xffffff, "%dˆÊ:-", i + 1);
		}
		else
		{
			DrawFormatString(100, 150 + (i * 40), 0xffffff, "%dˆÊ:%d", i + 1, result_[i].iData / UNIT_ID_NUM + 1);
		}
	}
}

void ResultScene::DrawOwnScreen(void)
{
	SetDrawScreen(screenID_);
	Draw();
}
