#include<Dxlib.h>
#include"Field.h"
#include"SceneMng.h"

Field::Field()
{
}

Field::Field(Vector2 offset) :_chip{ 6, 13 }, _puyoSize{ 40, 40 }, _fieldSize{ _chip.x * _puyoSize.x, _chip.y * _puyoSize.y }
{
	_offset = offset;
	_screenID = MakeScreen(_fieldSize.x, _fieldSize.y, true);
	_puyoPos1 = { _puyoSize.x * 2 + 20, 20 };
	_puyoPos2 = { _puyoSize.x * 2 + 20, 60 };
}

Field::~Field()
{
}

void Field::Update(void)
{
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

int Field::GetScreenID(void)
{
	return _screenID;
}

Vector2 Field::GetOffset(void)
{
	return _offset;
}
