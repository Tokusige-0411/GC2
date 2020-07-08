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
	_playerField.emplace_back(std::make_shared<Field>(std::move(Vector2(40, 40)), std::move(Vector2(240, 520))));
	_playerField.emplace_back(std::make_shared<Field>(std::move(Vector2(520, 40)), std::move(Vector2(240, 520))));

	_keyList[PLAYER_NUM::ONE].try_emplace(INPUT_ID::UP, KEY_INPUT_W);
	_keyList[PLAYER_NUM::ONE].try_emplace(INPUT_ID::DOWN, KEY_INPUT_S);
	_keyList[PLAYER_NUM::ONE].try_emplace(INPUT_ID::LEFT, KEY_INPUT_A);
	_keyList[PLAYER_NUM::ONE].try_emplace(INPUT_ID::RIGHT, KEY_INPUT_D);
	_keyList[PLAYER_NUM::ONE].try_emplace(INPUT_ID::R_ROTA, KEY_INPUT_Q);
	_keyList[PLAYER_NUM::ONE].try_emplace(INPUT_ID::L_LOTA, KEY_INPUT_E);

	_keyList[PLAYER_NUM::TWO].try_emplace(INPUT_ID::UP, KEY_INPUT_UP);
	_keyList[PLAYER_NUM::TWO].try_emplace(INPUT_ID::DOWN, KEY_INPUT_DOWN);
	_keyList[PLAYER_NUM::TWO].try_emplace(INPUT_ID::LEFT, KEY_INPUT_LEFT);
	_keyList[PLAYER_NUM::TWO].try_emplace(INPUT_ID::RIGHT, KEY_INPUT_RIGHT);
	_keyList[PLAYER_NUM::TWO].try_emplace(INPUT_ID::R_ROTA, KEY_INPUT_NUMPAD1);
	_keyList[PLAYER_NUM::TWO].try_emplace(INPUT_ID::L_LOTA, KEY_INPUT_NUMPAD2);

	//for (auto list : _keyData)
	//{
	//	for (auto id : INPUT_ID())
	//	{
	//		list.second.try_emplace.
	//	}
	//}

	_frame = 0;
}

SceneMng::~SceneMng()
{
}
