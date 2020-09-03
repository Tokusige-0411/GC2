#include<Dxlib.h>
#include<functional>
#include<random>
#include<algorithm>
#include<cmath>
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
#include"FieldCtl/WinMode.h"
#include"FieldCtl/LoseMode.h"
#include"FieldCtl/DrawMode.h"
#include"NextPuyoCtl.h"
#include"EffectCtl.h"

int Field::plCount_ = 0;
bool Field::gameEnd_ = false;
std::array<int, 2> Field::changeKey_ = { KEY_INPUT_PGUP, KEY_INPUT_PGDN };

Field::Field()
{
	Init();
}

Field::Field(Vector2&& offset, Vector2&& size) : stgGridSize_{8, 14}
{
	offset_ = offset;
	fieldSize_ = size;
	blockSize_ = 32;
	player_ = plCount_ % 2;
	plCount_++;
	gameEnd_ = false;
	Init();
	InstancePuyo();
}

Field::~Field()
{
	plCount_ = 0;
	gameEnd_ = false;
}

int Field::Update(int ojamaCnt)
{
	ChangeCont();
	(*contMap_[contType_])();
	ojamaCnt_ = 0;

	// Ç®Ç∂Ç·Ç‹Ç’ÇÊÇÃ≤›Ω¿›Ω
	while (ojamaCnt)
	{
		InstanceOjama();
		ojamaCnt--;
	}

	if (!gameEnd_)
	{
		if (!fieldMode_[fieldState_](*this))
		{
			result_ = ResultF::Lose;
			gameEnd_ = true;
		}
	}
	else
	{
		switch (result_)
		{
		case ResultF::Win:
			ojamaCnt_ = fieldMode_[FieldState::Win](*this);
			break;
		case ResultF::Lose:
			ojamaCnt_ = fieldMode_[FieldState::Lose](*this);
			break;
		case ResultF::Draw:
			ojamaCnt_ = fieldMode_[FieldState::Draw](*this);
			break;
		default:
			break;
		}
	}

	if (fieldState_ == FieldState::Fall)
	{
		for (auto&& puyo : puyoVec_)
		{
			puyo->SoftDrop();
		}
	}

	return ojamaCnt_;
}

void Field::Draw()
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	DrawGraph(0, 0, fieldBG_[player_], true);
	GuideDraw();
	for (auto&& puyo : puyoVec_)
	{
		puyo->Draw();
	}
	DrawGraph(0, 0, fieldFrame_, true);
	OjamaDraw();
	SetLogo();
	//LogoDraw();
}

void Field::OjamaDraw(void)
{
	int count = 0;
	for (auto ojama : ojamaList_)
	{
		DrawCircle(48 + count % 6 * blockSize_, 16, blockSize_ / 2, 0x888888, true);
		count++;
	}
}

void Field::GuideDraw(void)
{
	if (fieldState_ != FieldState::Drop)
	{
		return;
	}
	auto pos = puyoVec_[targetID_ ^ 1]->Pos();
	DrawCircle(pos.x, pos.y, blockSize_ / 2 + 2, 0xffffff, true);
	DrawCircle(guidePuyo.first->Pos().x, guidePuyo.first->Pos().y, 5, guidePuyo.first->GetColor(), true);
	DrawCircle(guidePuyo.second->Pos().x, guidePuyo.second->Pos().y, 5, guidePuyo.second->GetColor(), true);
}

void Field::LogoDraw(int handle, Vector2 pos, double rad)
{
	lpSceneMng.AddDrawQue({ handle, pos.x, pos.y, rad, 1.0, 0.0f, Layer::Ui, DX_BLENDMODE_NOBLEND, 255, DrawType::Image });
}

void Field::SetLogo(void)
{
	if (rensaLogoDrawCnt_ > 0)
	{
		LogoDraw(rensaLogo_[rensaDrawCnt_], rensaLogoPos_, 0.0);
		rensaLogoPos_.y--;
		rensaLogoDrawCnt_--;
	}
	else
	{
		rensaDrawCnt_ = 0;
	}
}

void Field::DrawField(void)
{
	Draw();
	lpSceneMng.AddDrawQue({ screenID_, offset_.x + fieldSize_.x / 2 + blockSize_, offset_.y + fieldSize_.y / 2 + blockSize_, rad_, 1.0, 0.0f, Layer::Char, DX_BLENDMODE_NOBLEND, 255, DrawType::Image });
	nextCtl_->Draw();
}

bool Field::Init(void)
{
	screenID_ = MakeScreen(stgGridSize_.x * blockSize_, stgGridSize_.y * blockSize_, false);

	dataBase_.resize(stgGridSize_.x * stgGridSize_.y);
	for (int no = 0; no < stgGridSize_.y; no++)
	{
		data_.emplace_back(&dataBase_[no * stgGridSize_.x]);
	}
	for (int y = 0; y < stgGridSize_.y; y++)
	{
		data_[y][0] = std::make_shared<Puyo>(Vector2(blockSize_ / 2, blockSize_ / 2 + y * blockSize_), Puyo_ID::WALL);
		data_[y][stgGridSize_.x - 1] = std::make_shared<Puyo>(Vector2(stgGridSize_.x * blockSize_ - blockSize_ / 2, blockSize_ / 2 + y * blockSize_), Puyo_ID::WALL);
	}
	for (int x = 0; x < stgGridSize_.x; x++)
	{
		data_[stgGridSize_.y - 1][x] = std::make_shared<Puyo>(Vector2(blockSize_ / 2 + x * blockSize_, stgGridSize_.y * blockSize_ - blockSize_ / 2), Puyo_ID::WALL);
	}

	// çÌèúÇ’ÇÊäiî[îzóÒèâä˙âª
	eraseDataBase_.resize(stgGridSize_.x * stgGridSize_.y);
	for (int no = 0; no < stgGridSize_.y; no++)
	{
		eraseData_.emplace_back(&eraseDataBase_[no * stgGridSize_.x]);
	}

	playerUnit_ = std::make_unique<PlayerUnit>(*this);

	Vector2 nextOffset = { offset_.x + blockSize_ * (stgGridSize_.x + 1), offset_.y + blockSize_ * 2 };
	nextCtl_ = std::make_unique<NextPuyoCtl>(nextOffset, 3, 2);

	fieldMode_.try_emplace(FieldState::Drop, DropMode());
	fieldMode_.try_emplace(FieldState::Puyon, PuyonMode());
	fieldMode_.try_emplace(FieldState::Rensa, RensaMode());
	fieldMode_.try_emplace(FieldState::Fall, FallMode());
	fieldMode_.try_emplace(FieldState::Munyon, MunyonMode());
	fieldMode_.try_emplace(FieldState::Win, WinMode());
	fieldMode_.try_emplace(FieldState::Lose, LoseMode());
	fieldMode_.try_emplace(FieldState::Draw, LoseMode());
	fieldState_ = FieldState::Drop;

	rensaCnt_ = 0;
	rensaMax_ = 2;
	ojamaCnt_ = 0;
	ojamaFlag_ = true;
	erasePuyoCnt_ = 0;
	result_ = ResultF::Win;

	changeTrg_ = { 0, 1 };
	contMap_.clear();
	contType_ = ContType::Pad;
	contMap_.emplace(ContType::KeyBoard, std::make_unique<KeyInput>());
	contMap_.emplace(ContType::Mouse, std::make_unique<Mouse>());
	contMap_.emplace(ContType::Pad, std::make_unique<PadInput>());
	for (auto&& data : contMap_)
	{
		data.second->SetUp(player_);
	}

	targetID_ = 0;
	rad_ = 0.0;
	fallSpeed_ = 5;
	resultEndCnt_ = 360;
	rensaDrawCnt_ = 0;
	rensaLogoPos_ = { 0, 0 };
	rensaLogoDrawCnt_ = 0;
	resultLogoPos_ = {offset_.x + 128, offset_.y + 150};

	// ï`âÊ ›ƒﬁŸ
	LoadDivGraph("image/FieldBG.png", 2, 2, 1, 256, 448, fieldBG_.data());
	fieldFrame_ = LoadGraph("image/FieldFrame.png");
	LoadDivGraph("image/rensa.png", 5, 1, 5, 96, 24, rensaLogo_.data());
	resultLogo_.try_emplace(ResultF::Win, LoadGraph("image/Win.png"));
	resultLogo_.try_emplace(ResultF::Lose, LoadGraph("image/Lose.png"));
	resultLogo_.try_emplace(ResultF::Draw, LoadGraph("image/Lose.png"));

	return true;
}

int Field::GetScreenID(void)
{
	return screenID_;
}

Vector2 Field::GetOffset(void)
{
	return offset_;
}

bool Field::InstancePuyo(void)
{
	auto pairPuyo = nextCtl_->Pickup();
	pairPuyo.first->Pos(Vector2((stgGridSize_.x / 2 + 1) * blockSize_ - 16, 0));
	pairPuyo.second->Pos(Vector2((stgGridSize_.x / 2 + 1) * blockSize_ - 16, 32));
	puyoVec_.emplace(puyoVec_.begin(), pairPuyo.first);
	puyoVec_.emplace(puyoVec_.begin(), pairPuyo.second);
	auto type = puyoVec_[0]->Type();
	auto pos = puyoVec_[0]->Pos();
	guidePuyo.first = std::make_shared<Puyo>(Vector2(pos.x, pos.y), type);
	type = puyoVec_[1]->Type();
	pos = puyoVec_[1]->Pos();
	guidePuyo.second = std::make_shared<Puyo>(Vector2(pos.x, pos.y), type);

	return true;
}

bool Field::SetEraseData(SharedPuyo& puyo)
{
	auto grid = puyo->Grid(blockSize_);
	int count = 0;

	std::function<void(Puyo_ID, Vector2)> chPuyo = [&](Puyo_ID type, Vector2 grid) {
		if (type != Puyo_ID::OJAMA)
		{
			if ((grid.y >= 0) &&
				(grid.y < stgGridSize_.y) &&
				(grid.x >= 0) &&
				(grid.x < stgGridSize_.x))
			{
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
			}
		}
	};

	chPuyo(puyo->Type(), puyo->Grid(blockSize_));

	auto chOjama = [&](Vector2 grid) {
		if (data_[grid.y][grid.x])
		{
			if (data_[grid.y][grid.x]->Type() == Puyo_ID::OJAMA)
			{
				eraseData_[grid.y][grid.x] = data_[grid.y][grid.x];
			}
		}
	};

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
		// Ç®Ç∂Ç·Ç‹Ç’ÇÊÇeraseDataÇ…í«â¡
		for (int y = 0; y < stgGridSize_.y; y++)
		{
			for (int x = 0; x < stgGridSize_.x; x++)
			{
				if (eraseData_[y][x])
				{
					if (eraseData_[y][x]->Type() < Puyo_ID::OJAMA)
					{
						chOjama({ x + 1, y });
						chOjama({ x - 1, y });
						chOjama({ x, y + 1 });
						chOjama({ x, y - 1 });
					}
				}
			}
		}

		// Ç®é◊ñÇÇ’ÇÊÇÃí«â¡
		rensaDrawCnt_ = rensaCnt_;
		rensaCnt_++;
		erasePuyoCnt_ += count;

		for (auto&& puyo : puyoVec_)
		{
			auto grid = puyo->Grid(blockSize_);
			if (eraseData_[grid.y][grid.x] == puyo)
			{
				if (puyo->Type() < Puyo_ID::OJAMA)
				{
					int effectHndle = lpEffectCtl.Play("erase");
					lpSceneMng.AddDrawQue({
						effectHndle,
						offset_.x + puyo->Pos().x,
						offset_.y + puyo->Pos().y + blockSize_ / 2,
						0.0, 1.0, 1.0f, Layer::Char, DX_BLENDMODE_NOBLEND, 255, DrawType::Effect });
					rensaLogoPos_ = puyo->Pos();
					rensaLogoDrawCnt_ = 60;
				}
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
	DirPermit moveChack = { 1, 1, 1, 1 };
	Vector2 grid = puyo->Grid(blockSize_);

	if ((grid.y >= 0) &&
		(grid.y < stgGridSize_.y) &&
		(grid.x >= 0) &&
		(grid.x < stgGridSize_.x))
	{
		if (data_[grid.y][grid.x - 1])
		{
			moveChack.bit.left = false;
		}
		if (data_[grid.y][grid.x + 1])
		{
			moveChack.bit.right = false;
		}
		if (data_[grid.y + 1][grid.x])
		{
			moveChack.bit.down = false;
			data_[grid.y][grid.x] = puyo;
		}
		else
		{
			data_[grid.y][grid.x].reset();
		}
	}

	puyo->SetDirPermit(moveChack);

	if (moveChack.bit.down)
	{
		return false;
	}
	return true;
}

void Field::InstanceOjama(void)
{
	ojamaList_.emplace_back(std::make_shared<OjamaPuyo>(std::move(Vector2{48, 16}), Puyo_ID::OJAMA));
}

void Field::SetResult(ResultF result)
{
	result_ = result;
}

const ResultF Field::GetResult(void)
{
	return result_;
}

Vector2 Field::ConvertGrid(Vector2 grid)
{
	return Vector2(grid.x * blockSize_ + blockSize_ / 2, grid.y * blockSize_ + blockSize_ / 2);
}

void Field::ChangeCont(void)
{
	changeTrg_.second = changeTrg_.first;
	changeTrg_.first = CheckHitKey(changeKey_[player_]);
	if (changeTrg_.first && !changeTrg_.second)
	{
		++contType_;
		if (contType_ >= ContType::Max)
		{
			contType_ = ContType::KeyBoard;
		}
	}
}
