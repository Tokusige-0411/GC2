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
	blockSize_ = 40;
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

	if (fieldState_ == FieldState::Fall)
	{
		for (auto&& puyo : puyoVec_)
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
	for (auto&& puyo : puyoVec_)
	{
		puyo->Draw();
	}
	DrawBox(0, 0, stgGridSize_.x * blockSize_, stgGridSize_.y * blockSize_, 0xffffff, false);
}

bool Field::Init(void)
{
	_screenID = MakeScreen(stgGridSize_.x * blockSize_, stgGridSize_.y * blockSize_, true);
	//_controller = std::make_unique<PadInput>();
	_controller = std::make_unique<KeyInput>();
	_controller->SetUp(_player);

	_dataBase.resize(stgGridSize_.x * stgGridSize_.y);
	for (int no = 0; no < stgGridSize_.y; no++)
	{
		data_.emplace_back(&_dataBase[no * stgGridSize_.x]);
	}
	for (int y = 0; y < stgGridSize_.y; y++)
	{
		data_[y][0] = std::make_shared<Puyo>(Vector2(blockSize_ / 2, blockSize_ / 2 + y * blockSize_), Puyo_Type::WALL);
		data_[y][stgGridSize_.x - 1] = std::make_shared<Puyo>(Vector2(stgGridSize_.x * blockSize_ - blockSize_ / 2, blockSize_ / 2 + y * blockSize_), Puyo_Type::WALL);
	}
	for (int x = 0; x < stgGridSize_.x; x++)
	{
		data_[stgGridSize_.y - 1][x] = std::make_shared<Puyo>(Vector2(blockSize_ / 2 + x * blockSize_, stgGridSize_.y * blockSize_ - blockSize_ / 2), Puyo_Type::WALL);
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
	puyoVec_.emplace(puyoVec_.begin(), std::make_shared<Puyo>(std::move(Vector2(stgGridSize_.x / 2 * blockSize_ - 20, 100)), static_cast<Puyo_Type>(rand() % 5 + 1)));
	puyoVec_.emplace(puyoVec_.begin(), std::make_shared<Puyo>(std::move(Vector2(stgGridSize_.x / 2 * blockSize_ - 20, 140)), static_cast<Puyo_Type>(rand() % 5 + 1)));
	return false;
}

bool Field::SetEraseData(SharedPuyo& puyo)
{
	auto grid = puyo->Grid(blockSize_);
	int count = 0;

	std::function<void(Puyo_Type, Vector2)> chPuyo = [&](Puyo_Type type, Vector2 grid) {
		if (!eraseData_[grid.y][grid.x])
		{
			if (data_[grid.y][grid.x])
			{
				if (data_[grid.y][grid.x]->Type() == type)
				{
					count++;
					eraseData_[grid.y][grid.x] = data_[grid.y][grid.x];
					chPuyo(type, { grid.x, grid.y + 1 });
					chPuyo(type, { grid.x, grid.y - 1 });
					chPuyo(type, { grid.x - 1, grid.y });
					chPuyo(type, { grid.x + 1, grid.y });
				}
			}
		}
	};

	chPuyo(puyo->Type(), puyo->Grid(blockSize_));

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
		for (auto&& puyo : puyoVec_)
		{
			auto grid = puyo->Grid(blockSize_);
			if (eraseData_[grid.y][grid.x] == puyo)
			{
				puyo->Alive(false);
				data_[grid.y][grid.x].reset();
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
	Vector2 grid = puyo->Grid(blockSize_);

	if (!data_[grid.y][grid.x - 1])
	{
		moveChack.bit.left = true;
	}
	if (!data_[grid.y][grid.x + 1])
	{
		moveChack.bit.right = true;
	}
	if (!data_[grid.y - 1][grid.x])
	{
		moveChack.bit.up = true;
	}

	if (!data_[grid.y + 1][grid.x])
	{
		moveChack.bit.down = true;
		data_[grid.y][grid.x].reset();
	}
	else
	{
		data_[grid.y][grid.x] = puyo;
	}

	puyo->SetDirPermit(moveChack);

	if (moveChack.bit.down)
	{
		return false;
	}
	return true;
}

bool Field::SetMunyon(SharedPuyo& puyo)
{
	auto CheckMunyon = [&](Puyo_Type id , Vector2 grid) {
		if (data_[grid.y][grid.x])
		{
			if (data_[grid.y][grid.x]->Type() == id)
			{
				return true;
			}
		}
		return false;
	};
	auto grid = puyo->Grid(blockSize_);
	DirPermit tmpPermit;
	tmpPermit.bit.up = CheckMunyon(puyo->Type(), { grid.x, grid.y - 1 });
	tmpPermit.bit.right = CheckMunyon(puyo->Type(), { grid.x + 1, grid.y });
	tmpPermit.bit.down = CheckMunyon(puyo->Type(), { grid.x, grid.y + 1 });
	tmpPermit.bit.left = CheckMunyon(puyo->Type(), { grid.x - 1, grid.y });
	puyo->SetDrawPermit(tmpPermit);
	return true;
}
