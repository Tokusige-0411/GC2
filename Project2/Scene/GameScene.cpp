#include<Dxlib.h>
#include "GameScene.h"
#include "MenuScene.h"
#include "../SceneMng.h"
#include "ResultScene.h"

GameScene::GameScene()
{
	playerField_.emplace_back(std::make_unique<Field>(std::move(Vector2(32, 64)), std::move(Vector2(192, 416))));
	playerField_.emplace_back(std::make_unique<Field>(std::move(Vector2(432, 64)), std::move(Vector2(192, 416))));
	ojamaCnt_ = 0;
	gameEnd_ = false;
	ownFlag_ = false;
	poseKey_ = 0;
	poseKeyOld_ = 1;
	startCnt_ = 0;
	logoPos_ = { lpSceneMng.GetScreenCenter().x, -80 };
	logoVec_ = 900;
	fade_ = 255;
	ex_ = 1.0;
	gameSceneBG_ = LoadGraph("image/GameSceneBG.png");
	readyGoH_[0] = LoadGraph("image/Ready.png");
	readyGoH_[1] = LoadGraph("image/Go.png");
}

GameScene::~GameScene()
{
}

unique_Base GameScene::Update(unique_Base own)
{
	if (Start())
	{
		return std::move(own);
	}
	poseKeyOld_ = poseKey_;
	poseKey_ = CheckHitKey(KEY_INPUT_F2);
	if (!gameEnd_)
	{
		// ポーズ画面移行
		if ((poseKey_) && (!poseKeyOld_))
		{
			own = std::make_unique<MenuScene>(std::move(own), true, false);
		}
		//おじゃま送る
		for (auto&& data : playerField_)
		{
			ojamaCnt_ = data->Update(ojamaCnt_);
		}
	}
	else
	{
		bool resultFlag = true;
		for (auto&& data : playerField_)
		{
			resultFlag &= static_cast<bool>(data->Update(0));
		}
		if (resultFlag)
		{
			if (!ownFlag_)
			{
				ownFlag_ = true;
				own = std::make_unique<ResultScene>(std::move(own), true, true);
			}
		}
	}

	// 引き分け判定
	bool drawFlag = true;
	for (auto&& data : playerField_)
	{
		drawFlag &= (data->GetResult() == ResultF::Lose);
	}
	if (drawFlag)
	{
		for (auto&& data : playerField_)
		{
			data->SetResult(ResultF::Draw);
		}
	}
	// ｹﾞｰﾑ終了判定
	for (auto&& data : playerField_)
	{
		if (data->GetResult() != ResultF::Win)
		{
			gameEnd_ = true;
		}
	}

	return std::move(own);
}

void GameScene::Draw(void)
{
	lpSceneMng.AddDrawQue({
	gameSceneBG_,
	lpSceneMng.GetScreenCenter().x,
	lpSceneMng.GetScreenCenter().y,
	0.0, 1.0, 0.0f, Layer::Bg,  DX_BLENDMODE_NOBLEND, 255, DrawType::Image }
	);
	for (auto&& data : playerField_)
	{
		data->DrawField();
	}
}

void GameScene::SetGameEnd(bool flag)
{
	gameEnd_ = flag;
}

const bool GameScene::GetGameEnd(void)
{
	return gameEnd_;
}

bool GameScene::Start(void)
{
	if (startCnt_ >= 240)
	{
		return false;
	}
	if (startCnt_ < 120)
	{
		logoPos_.y += logoVec_ / 120;
		lpSceneMng.AddDrawQue({ readyGoH_[startCnt_ / 120], logoPos_.x, logoPos_.y,  0.0, ex_, 1.0f, Layer::Ui, DX_BLENDMODE_NOBLEND, fade_, DrawType::Image});
	}
	else
	{
		ex_ += 0.05;
		fade_ -= 255 / 120;
		lpSceneMng.AddDrawQue({ readyGoH_[startCnt_ / 120], 
			lpSceneMng.GetScreenCenter().x, lpSceneMng.GetScreenCenter().y, 0.0, ex_, 1.0f, Layer::Ui, DX_BLENDMODE_ALPHA, fade_, DrawType::Image });
	}
	startCnt_++;
	return (startCnt_ < 240);
}
