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
#include"PuyoCtl/DropMode.h"
#include"PuyoCtl/FallMode.h"
#include"PuyoCtl/MunyonMode.h"
#include"PuyoCtl/PuyonMode.h"
#include"PuyoCtl/RensaMode.h"

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
	fieldMode_[fieldState_](*this);


	/*bool nextFlag = true;
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
				fieldState_ = FieldState::Drop;
			}
		}
	}*/

	if (fieldState_ == FieldState::Rensa)
	{
		for (auto&& puyo : _puyoVec)
		{
			puyo->SoftDrop();
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
		_data[y][0] = std::make_shared<Puyo>(Vector2(_blockSize / 2, _blockSize / 2 + y * _blockSize), Puyo_Type::WALL);
		_data[y][stgGridSize_.x - 1] = std::make_shared<Puyo>(Vector2(stgGridSize_.x * _blockSize - _blockSize / 2, _blockSize / 2 + y * _blockSize), Puyo_Type::WALL);
	}
	for (int x = 0; x < stgGridSize_.x; x++)
	{
		_data[stgGridSize_.y - 1][x] = std::make_shared<Puyo>(Vector2(_blockSize / 2 + x * _blockSize, stgGridSize_.y * _blockSize - _blockSize / 2), Puyo_Type::WALL);
	}

	// çÌèúÇ’ÇÊäiî[îzóÒèâä˙âª
	eraseDataBase_.resize(stgGridSize_.x * stgGridSize_.y);
	for (int no = 0; no < stgGridSize_.y; no++)
	{
		eraseData_.emplace_back(&eraseDataBase_[no * stgGridSize_.x]);
	}

	playerUnit_ = std::make_unique<PlayerUnit>(*this);

	fieldMode_.try_emplace(FieldState::Drop, DropMode());
	fieldMode_.try_emplace(FieldState::Puyon, PuyonMode());
	fieldMode_.try_emplace(FieldState::Rensa, RensaMode());
	fieldMode_.try_emplace(FieldState::Fall, FallMode());
	fieldMode_.try_emplace(FieldState::Munyon, MunyonMode());
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
	_puyoVec.emplace(_puyoVec.begin(), std::make_shared<Puyo>(std::move(Vector2(stgGridSize_.x / 2 * _blockSize - 20, 60)), static_cast<Puyo_Type>(rand() % 5 + 1)));
	return false;
}

bool Field::SetEraseData(SharedPuyo& puyo)
{
	auto grid = puyo->Grid(_blockSize);
	int count = 0;

	std::function<void(Puyo_Type, Vector2)> chPuyo = [&](Puyo_Type type, Vector2 grid) {
		if (!eraseData_[grid.y][grid.x])
		{
			if (_data[grid.y][grid.x])
			{
				if (_data[grid.y][grid.x]->Type() == type)
				{
					count++;
					eraseData_[grid.y][grid.x] = _data[grid.y][grid.x];
					chPuyo(type, { grid.x, grid.y + 1 });
					chPuyo(type, { grid.x, grid.y - 1 });
					chPuyo(type, { grid.x - 1, grid.y });
					chPuyo(type, { grid.x + 1, grid.y });
				}
			}
		}
	};

	chPuyo(puyo->Type(), puyo->Grid(_blockSize));

	if (count < 4)
	{
		for (int y = 0; y < stgGridSize_.y; y++)
		{
			for (int x = 0; x < stgGridSize_.x; x++)
			{
				if (eraseData_[y][x])
				{
					eraseData_[y][x].reset();
				}
			}
		}
	}
	else
	{
		for (auto&& puyo : _puyoVec)
		{
			auto grid = puyo->Grid(_blockSize);
			if (eraseData_[grid.y][grid.x] == puyo)
			{
				puyo->Alive(false);
				_data[grid.y][grid.x].reset();
			}
		}
		return true;
	}
	return false;
}

bool Field::SetParmit(SharedPuyo& puyo)
{
	// _dataBaseÇÃíÜêgÇ∆Ω√∞ºﬁì‡Ç©å©Çƒà⁄ìÆÇ≈Ç´ÇÈÇ©Ç«Ç§Ç©
	DirPermit moveChack = { 0, 0, 0, 0 };
	Vector2 grid = puyo->Grid(_blockSize);

	if (!_data[grid.y][grid.x - 1])
	{
		moveChack.bit.left = true;
	}
	if (!_data[grid.y][grid.x + 1])
	{
		moveChack.bit.right = true;
	}
	if (!_data[grid.y - 1][grid.x])
	{
		moveChack.bit.up = true;
	}

	if (!_data[grid.y + 1][grid.x])
	{
		moveChack.bit.down = true;
		_data[grid.y][grid.x].reset();
	}
	else
	{
		_data[grid.y][grid.x] = puyo;
	}

	puyo->SetDirPermit(moveChack);

	if (moveChack.bit.down)
	{
		return false;
	}
	return true;
}
