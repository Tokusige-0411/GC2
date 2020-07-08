#include<Dxlib.h>
#include"Field.h"
#include"SceneMng.h"
#include"KeyState.h"

Field::Field()
{
	Init();
}

Field::Field(PLAYER_NUM player, Vector2&& offset, Vector2&& size) :_puyoSize{ 40, 40 }
{
	_offset = std::move(offset);
	_fieldSize = std::move(size);
	_player = player;
	Init();
}

Field::~Field()
{
}

void Field::Update(void)
{
	_input->Update();

	auto move = [](std::weak_ptr<InputState> keyData, const INPUT_ID id, int& pNum, const int size) {
		if (!keyData.expired())
		{
			if ((*keyData.lock()).State(id).first && !(*keyData.lock()).State(id).second)
			{
				pNum += size;
			}
		}
	};

	move(_input, INPUT_ID::LEFT, _puyoPos1.x, -_puyoSize.x);
	move(_input, INPUT_ID::RIGHT, _puyoPos1.x, _puyoSize.x);
	move(_input, INPUT_ID::LEFT, _puyoPos2.x, -_puyoSize.x);
	move(_input, INPUT_ID::RIGHT, _puyoPos2.x, _puyoSize.x);

	if (!(lpSceneMng.GetFrameCount() % 60))
	{
		_puyoPos1.y += _puyoSize.y;
		_puyoPos2.y += _puyoSize.y;
	}
	Draw();
}

void Field::Draw()
{
	SetDrawScreen(_screenID);
	ClsDrawScreen();

	DrawBox(0, _puyoSize.y, _fieldSize.x, _fieldSize.y, 0xffffff, false);
	DrawCircle(_puyoPos1.x, _puyoPos1.y, _puyoSize.x / 2 - 2, 0xffffff, true);
	DrawCircle(_puyoPos2.x, _puyoPos2.y, _puyoSize.x / 2 - 2, 0xffffff, true);
	DrawCircle(_puyoSize.y / 2, _puyoSize.y / 2, _puyoSize.x / 2 - 2, 0xffffff, true);
}

bool Field::Init(void)
{
	_screenID = MakeScreen(_fieldSize.x, _fieldSize.y, true);
	_puyoPos1 = { _puyoSize.x * 2 + 20, 20 };
	_puyoPos2 = { _puyoSize.x * 2 + 20, 60 };
	_input = std::make_shared<KeyState>(_player);
	return true;
}

int Field::GetScreenID(void)
{
	return _screenID;
}

Vector2 Field::GetOffset(void)
{
	return _offset;
}
