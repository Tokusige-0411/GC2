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
	gameSceneBG_ = LoadGraph("image/GameSceneBG.png");
	gameEnd_ = false;
}

GameScene::~GameScene()
{
}

unique_Base GameScene::Update(unique_Base own)
{
	if (!gameEnd_)
	{
		// �|�[�Y��ʈڍs
		if (CheckHitKey(KEY_INPUT_F2))
		{
			own = std::make_unique<MenuScene>(std::move(own), true, false);
		}
		//������ܑ���
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
			own = std::make_unique<ResultScene>(std::move(own), true, false);
		}
	}

	// ������������
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
	// �ްяI������
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
	0.0, 0.0f, Layer::Bg,  DX_BLENDMODE_NOBLEND, 255, DrawType::Image }
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
