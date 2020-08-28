#include<Dxlib.h>
#include "GameScene.h"
#include "MenuScene.h"
#include "../SceneMng.h"

GameScene::GameScene()
{
	playerField_.emplace_back(std::make_unique<Field>(std::move(Vector2(32, 64)), std::move(Vector2(192, 416))));
	playerField_.emplace_back(std::make_unique<Field>(std::move(Vector2(432, 64)), std::move(Vector2(192, 416))));
	ojamaCnt_ = 0;
	gameSceneBG_ = LoadGraph("image/GameSceneBG.png");
}

GameScene::~GameScene()
{
}

unique_Base GameScene::Update(unique_Base own)
{
	lpSceneMng.AddDrawQue({ gameSceneBG_, lpSceneMng.GetScreenCenter().x, lpSceneMng.GetScreenCenter().y, 0.0, 0.0f, Layer::Bg,  DX_BLENDMODE_NOBLEND, 255, DrawType::Image });
	// ただの参照は書き換えれてしまう
	// →const参照でする(notおすすめ)
	// そのまま右辺値参照で受け取る
	for (auto&& data : playerField_)
	{
		ojamaCnt_ = data->Update(ojamaCnt_);
	}

	// ｹﾞｰﾑ終了判定
	//for (auto&& data : playerField_)
	//{
	//	data->SetResult
	//}

	if (CheckHitKey(KEY_INPUT_F2))
	{
		own = std::make_unique<MenuScene>(std::move(own), true, false);
	}

	return std::move(own);
}

void GameScene::Draw(void)
{
	for (auto&& data : playerField_)
	{
		data->DrawField();
	}
}
