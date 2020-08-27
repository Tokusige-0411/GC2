#include<Dxlib.h>
#include "GameScene.h"
#include "MenuScene.h"

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
	// �����̎Q�Ƃ͏���������Ă��܂�
	// ��const�Q�Ƃł���(not��������)
	// ���̂܂܉E�Ӓl�Q�ƂŎ󂯎��
	for (auto&& data : playerField_)
	{
		ojamaCnt_ = data->Update(ojamaCnt_);
	}

	// �ްяI������
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
	DrawGraph(0, 0, gameSceneBG_, true);
	for (auto&& data : playerField_)
	{
		data->DrawField();
	}
}
