#include<Dxlib.h>
#include"Field.h"
#include"SceneMng.h"
#include"input/KeyInput.h"
#include"input/Mouse.h"
#include"input/PadInput.h"

#define STG_SIZE_X 6
#define STG_SIZE_Y 13

int Field::_plCount = 0;

Field::Field()
{
	Init();
}

Field::Field(Vector2&& offset, Vector2&& size)
{
	_offset = offset;
	_fieldSize = size;
	_player = _plCount;
	_plCount++;
	Init();
	_puyo.emplace_back(std::make_unique<Puyo>(std::move(Vector2( 100, 60 )), Puyo_Type::RED));
}

Field::~Field()
{
}

void Field::Update(void)
{
	(*_controller)();

	// _dataBase‚Ì’†g‚Æ½Ã°¼Þ“à‚©Œ©‚ÄˆÚ“®‚Å‚«‚é‚©‚Ç‚¤‚©
	DirPermit moveChack = { 1, 1, 1, 1 };

	
	for (auto data : _controller->GetContData())
	{
		if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
		{
		}
	}

	Draw();
}

void Field::Draw()
{
	SetDrawScreen(_screenID);
	ClsDrawScreen();
	for (auto& puyo : _puyo)
	{
		puyo->Draw();
	}
	DrawBox(0, 40, _fieldSize.x, _fieldSize.y, 0xffffff, false);
}

bool Field::Init(void)
{
	_screenID = MakeScreen(_fieldSize.x, _fieldSize.y, true);
	//_controller = std::make_unique<PadInput>();
	_controller = std::make_unique<KeyInput>();
	_controller->SetUp(_player);

	_dataBase.resize(STG_SIZE_X * STG_SIZE_Y);
	for (int no = 0; no < STG_SIZE_Y; no++)
	{
		_data.emplace_back(&_dataBase[no * STG_SIZE_X]);
	}

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
