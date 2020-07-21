#include<Dxlib.h>
#include<functional>
#include<random>
#include<algorithm>
#include"Field.h"
#include"SceneMng.h"
#include"input/KeyInput.h"
#include"input/Mouse.h"
#include"input/PadInput.h"
#include"PlayerUnit.h"

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
	InstancePuyo();
}

Field::~Field()
{
}

void Field::Update(void)
{
	(*_controller)();

	bool nextFlag = true;
	std::for_each(_puyoVec.rbegin(), _puyoVec.rend(), [&](auto& puyo)
		{
			nextFlag &= SetParmit(puyo);
		});

	playerUnit_->Update();

	bool rensaFlag = true;
	std::for_each(_puyoVec.rbegin(), _puyoVec.rend(), [&](auto& puyo)
		{
			if (puyo->Update())
			{
				rensaFlag = false;
			}
		});

	if (rensaFlag)
	{
		fieldState_ = FieldState::Rensa;
		bool delFlag = false;
		for (auto&& puyo : _puyoVec)
		{
			delFlag |= SetEraseData(puyo);
		}
		
		if (delFlag)
		{
			auto itl = std::remove_if(_puyoVec.begin(), _puyoVec.end(), [](auto&& puyo) {return !(puyo->Alive()); });
			_puyoVec.erase(itl, _puyoVec.end());
		}
		else
		{
			if (nextFlag)
			{
				InstancePuyo();
				SetParmit(_puyoVec[0]);
			}
			fieldState_ = FieldState::Drop;
		}
	}
	
	Draw();
}

void Field::Draw()
{
	SetDrawScreen(_screenID);
	ClsDrawScreen();
	for (auto&& puyo : _puyoVec)
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

	// íœ‚Õ‚æŠi”[”z—ñ‰Šú‰»
	eraseDataBase_.resize(stgGridSize_.x * stgGridSize_.y);
	for (int no = 0; no < stgGridSize_.y; no++)
	{
		eraseData_.emplace_back(&eraseDataBase_[no * stgGridSize_.x]);
	}

	playerUnit_ = std::make_unique<PlayerUnit>(*this);
	fieldState_ = FieldState::Drop;

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

bool Field::InstancePuyo(void)
{
	_puyoVec.emplace(_puyoVec.begin(), std::make_unique<Puyo>(std::move(Vector2(stgGridSize_.x / 2 * _blockSize - 20, 60)), static_cast<Puyo_Type>(rand() % 5 + 1)));
	return false;
}

bool Field::SetEraseData(std::unique_ptr<Puyo>& puyo)
{
	memset(eraseDataBase_.data(), 0, eraseDataBase_.size() * sizeof(Puyo_Type));
	auto grid = puyo->Grid(_blockSize);
	int count = 0;

	std::function<void(Puyo_Type, Vector2)> chPuyo = [&](Puyo_Type type, Vector2 grid) {
		if (eraseData_[grid.y][grid.x] == Puyo_Type::NON)
		{
			if (_data[grid.y][grid.x] == type)
			{
				count++;
				eraseData_[grid.y][grid.x] = _data[grid.y][grid.x];
				chPuyo(type, { grid.x, grid.y + 1 });
				chPuyo(type, { grid.x, grid.y - 1 });
				chPuyo(type, { grid.x - 1, grid.y });
				chPuyo(type, { grid.x + 1, grid.y });
			}

		}
	};

	chPuyo(puyo->Type(), puyo->Grid(_blockSize));

	if (count < 4)
	{
		memset(eraseDataBase_.data(), 0, eraseDataBase_.size() * sizeof(Puyo_Type));
	}
	else
	{
		for (auto&& puyo : _puyoVec)
		{
			auto grid = puyo->Grid(_blockSize);
			if (eraseData_[grid.y][grid.x] == puyo->Type())
			{
				puyo->Alive(false);
				_data[grid.y][grid.x] = Puyo_Type::NON;
			}
		}
		return true;
	}
	return false;
}

bool Field::SetParmit(std::unique_ptr<Puyo>& puyo)
{
	// _dataBase‚Ì’†g‚Æ½Ã°¼Þ“à‚©Œ©‚ÄˆÚ“®‚Å‚«‚é‚©‚Ç‚¤‚©
	DirPermit moveChack = { 0, 0, 0, 0 };
	Vector2 grid = puyo->Grid(_blockSize);

	if (_data[grid.y][grid.x - 1] == Puyo_Type::NON)
	{
		moveChack.bit.left = true;
	}
	if (_data[grid.y][grid.x + 1] == Puyo_Type::NON)
	{
		moveChack.bit.right = true;
	}
	if (_data[grid.y - 1][grid.x] == Puyo_Type::NON)
	{
		moveChack.bit.up = true;
	}

	if (_data[grid.y + 1][grid.x] == Puyo_Type::NON)
	{
		moveChack.bit.down = true;
		_data[grid.y][grid.x] = Puyo_Type::NON;
	}
	else
	{
		_data[grid.y][grid.x] = puyo->Type();
	}

	puyo->SetDirPermit(moveChack);

	if (moveChack.bit.down)
	{
		return false;
	}
	return true;
}
