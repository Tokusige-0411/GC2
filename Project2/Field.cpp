#include<Dxlib.h>
#include"Field.h"
#include"SceneMng.h"
#include"input/KeyInput.h"
#include"input/Mouse.h"
#include"input/PadInput.h"

int Field::_plCount = 0;

Field::Field()
{
	Init();
}

Field::Field(Vector2&& offset, Vector2&& size) : stgGridSize_{8, 14}
{
	_offset = offset;
	_fieldSize = size;
	_blockSize = 40;
	_player = _plCount;
	_plCount++;
	Init();
	_puyo.emplace_back(std::make_unique<Puyo>(std::move(Vector2(stgGridSize_.x / 2 * _blockSize - 20, 60 )), Puyo_Type::RED));
}

Field::~Field()
{
}

void Field::Update(void)
{
	(*_controller)();

	// _dataBase‚Ì’†g‚Æ½Ã°¼Þ“à‚©Œ©‚ÄˆÚ“®‚Å‚«‚é‚©‚Ç‚¤‚©
	DirPermit moveChack = { 0, 1, 1, 1 };
	Vector2 grid = _puyo.front()->Grid(_blockSize);
	moveChack.bit.left = (_data[grid.y][grid.x - 1] == Puyo_Type::NON);
	moveChack.bit.right = (_data[grid.y][grid.x + 1] == Puyo_Type::NON);
	moveChack.bit.down = (_data[grid.y + 1][grid.x] == Puyo_Type::NON);
	_puyo.front()->SetDirPermit(moveChack);
	for (auto data : _controller->GetContData())
	{
		if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
		{
			if (data.first == INPUT_ID::DOWN && !moveChack.bit.down)
			{
				_data[grid.y][grid.x] = _puyo.front()->Type();
				_puyo.emplace(_puyo.begin(),std::make_unique<Puyo>(std::move(Vector2(stgGridSize_.x / 2 * _blockSize - 20, 60)), Puyo_Type::RED));
			}
			else 
			{
				_puyo.front()->Move(data.first);
			}
		}
	}

	moveChack = { 0, 0, 0, 0 };
	for (int i = 1; i < _puyo.size(); i++)
	{
		//Vector2 grid = puyo->Grid(_blockSize);
		//moveChack.bit.left = (_data[grid.y][grid.x - 1] == Puyo_Type::NON);
		//moveChack.bit.right = (_data[grid.y][grid.x + 1] == Puyo_Type::NON);
		//moveChack.bit.down = (_data[grid.y + 1][grid.x] == Puyo_Type::NON);
		_puyo[i]->SetDirPermit(moveChack);
		//puyo->Update();
	}
	
	Draw();
}

void Field::Draw()
{
	SetDrawScreen(_screenID);
	ClsDrawScreen();
	for (auto&& puyo : _puyo)
	{
		puyo->Draw();
	}
	DrawBox(0, 0, stgGridSize_.x * _blockSize, stgGridSize_.y * _blockSize, 0xffffff, false);
}

bool Field::Init(void)
{
	_screenID = MakeScreen(stgGridSize_.x * _blockSize, stgGridSize_.y * _blockSize, true);
	//_controller = std::make_unique<PadInput>();
	_controller = std::make_unique<KeyInput>();
	_controller->SetUp(_player);

	_dataBase.resize(stgGridSize_.x * stgGridSize_.y);
	for (int no = 0; no < stgGridSize_.y; no++)
	{
		_data.emplace_back(&_dataBase[no * stgGridSize_.x]);
	}
	for (int y = 0; y < stgGridSize_.y; y++)
	{
		_data[y][0] = Puyo_Type::WALL;
		_data[y][stgGridSize_.x - 1] = Puyo_Type::WALL;
	}
	for (int x = 0; x < stgGridSize_.x; x++)
	{
		_data[stgGridSize_.y - 1][x] = Puyo_Type::WALL;
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
