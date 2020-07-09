#include<Dxlib.h>
#include "GameScene.h"

GameScene::GameScene()
{
	_playerField.emplace_back(std::make_unique<Field>(std::move(Vector2(40, 40)), std::move(Vector2(240, 520))));
	_playerField.emplace_back(std::make_unique<Field>(std::move(Vector2(520, 40)), std::move(Vector2(240, 520))));
}

GameScene::~GameScene()
{
}

unique_Base GameScene::Update(unique_Base own)
{
	// ただの参照は書き換えれてしまう
	// →const参照でする(notおすすめ)
	// そのまま右辺値参照で受け取る
	for (auto&& data : _playerField)
	{
		data->Update();
	}
	return std::move(own);
}

void GameScene::Draw(void)
{
	Vector2 offset;
	for (auto&& data : _playerField)
	{
		offset = data->GetOffset();
		DrawGraph(offset.x, offset.y, data->GetScreenID(), true);
	}
}
