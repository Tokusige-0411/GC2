#include<Dxlib.h>
#include "GameScene.h"

GameScene::GameScene()
{
	playerField_.emplace_back(std::make_unique<Field>(std::move(Vector2(32, 64)), std::move(Vector2(192, 416))));
	playerField_.emplace_back(std::make_unique<Field>(std::move(Vector2(432, 64)), std::move(Vector2(192, 416))));
	ojamaCnt_ = 0;
}

GameScene::~GameScene()
{
}

unique_Base GameScene::Update(unique_Base own)
{
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

	return std::move(own);
}

void GameScene::Draw(void)
{
	Vector2 offset;
	for (auto&& data : playerField_)
	{
		data->DrawField();
	}
}
