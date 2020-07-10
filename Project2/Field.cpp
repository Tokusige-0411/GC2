#include<Dxlib.h>
#include"Field.h"
#include"SceneMng.h"
//#include"input/KeyState.h"
//#include"input/PadState.h"
#include"input/KeyInput.h"

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
	//_input->Update();
	(*_controller)();

	for (auto data : _controller->GetContData())
	{
		if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
		{
			_puyo->Move(data.first);
		}
	}

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
	//_input = std::make_unique<KeyState>(_player);
	//_input = std::make_shared<PadState>();
	_controller = std::make_unique<KeyInput>();
	_controller->SetUp(static_cast<int>(_player));
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
