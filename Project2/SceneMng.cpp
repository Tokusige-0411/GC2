#include<Dxlib.h>
#include"SceneMng.h"

void SceneMng::Run()
{
	while (!ProcessMessage() || CheckHitKey(KEY_INPUT_ESCAPE))
	{
		for (auto data : _playerField)
		{
			data->Update();
		}
		Draw();
		_frame++;
	}
}

void SceneMng::Draw()
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();

	Vector2 offset;
	for (auto data : _playerField)
	{
		offset = data->GetOffset();
		DrawGraph(offset.x, offset.y, data->GetScreenID(), true);
	}

	DrawBox(320, 120, 360, 200, 0xffffff, false);
	DrawBox(440, 120, 480, 200, 0xffffff, false);

	ScreenFlip();
}

int SceneMng::GetFrameCount()
{
	return _frame;
}

bool SceneMng::SysInit()
{
	ChangeWindowMode(true);
	SetGraphMode(_screenSize.x, _screenSize.y, 16);

	if (DxLib_Init() == -1)
	{
		return false;
	}

	SetDrawScreen(DX_SCREEN_BACK);
	return true;
}

SceneMng::SceneMng() : _screenSize{800, 600}
{
	SysInit();
	_playerField.emplace_back(std::make_shared<Field>(Vector2(40, 40), Vector2(240, 520)));
	_playerField.emplace_back(std::make_shared<Field>(Vector2(520, 40), Vector2(240, 520)));
	_frame = 0;
}

SceneMng::~SceneMng()
{
}
