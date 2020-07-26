#include<Dxlib.h>
#include "GameScene.h"

GameScene::GameScene()
{
	_playerField.emplace_back(std::make_unique<Field>(std::move(Vector2(0, 0)), std::move(Vector2(240, 520))));
	//_playerField.emplace_back(std::make_unique<Field>(std::move(Vector2(400, 0)), std::move(Vector2(240, 520))));
}

GameScene::~GameScene()
{
}

unique_Base GameScene::Update(unique_Base own)
{
	// �����̎Q�Ƃ͏���������Ă��܂�
	// ��const�Q�Ƃł���(not��������)
	// ���̂܂܉E�Ӓl�Q�ƂŎ󂯎��
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
