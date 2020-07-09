#include<Dxlib.h>
#include"Field.h"
#include"SceneMng.h"
#include"KeyState.h"

int Field::_plCount = 0;

Field::Field()
{
	Init();
}

Field::Field(Vector2&& offset, Vector2&& size)
{
	_offset = offset;
	_fieldSize = size;
	_player = static_cast<PLAYER_ID>(_plCount);
	Init();
	_puyo = std::make_unique<Puyo>(std::move(Vector2( 100, 20 )), Puyo_Type::RED);
	_plCount++;
}

Field::~Field()
{
}

void Field::Update(void)
{
	_input->Update();
	_puyo->Update();
	Draw();
}

void Field::Draw()
{
	SetDrawScreen(_screenID);
	ClsDrawScreen();
	_puyo->Draw();
	DrawBox(0, 40, _fieldSize.x, _fieldSize.y, 0xffffff, false);
}

bool Field::Init(void)
{
	_screenID = MakeScreen(_fieldSize.x, _fieldSize.y, true);
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
