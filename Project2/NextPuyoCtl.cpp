#include <Dxlib.h>
#include "NextPuyoCtl.h"
#include "SceneMng.h"

NextPuyoCtl::NextPuyoCtl(Vector2& nextPos, int haveCnt, int drawCnt) : nextScreenSize_{48, 80}
{
	screenID_ = MakeScreen(nextScreenSize_.x, nextScreenSize_.y, false);
	nextPuyoList_.clear();
	nextPos_ = nextPos;
	drawCnt_ = drawCnt;

	Add(haveCnt);
}

NextPuyoCtl::~NextPuyoCtl()
{
}

void NextPuyoCtl::Draw(void)
{
	int count = drawCnt_;
	auto beforID = GetDrawScreen();
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	for (auto puyo : nextPuyoList_)
	{
		puyo.first->Draw();
		puyo.second->Draw();
		if (count <= 0)
		{
			break;
		}
		count--;
	}

	SetDrawScreen(beforID);
	//DrawGraph(nextPos_.x, nextPos_.y, screenID_, true);
	lpSceneMng.AddDrawQue({ screenID_, 
		nextPos_.x + nextScreenSize_.x / 2,  
		nextPos_.y + nextScreenSize_.y / 2 , 0.0, 1.0,
		0.0f, Layer::Char, 
		DX_BLENDMODE_NOBLEND, 255, DrawType::Image});
}

PairPuyo NextPuyoCtl::Pickup(void)
{
	auto puyo = *(nextPuyoList_.begin());
	nextPuyoList_.erase(nextPuyoList_.begin());
	Add(1);
	return puyo;
}

void NextPuyoCtl::Add(int no)
{
	while (no > 0)
	{
		auto id1 = rand() % 5 + 1;
		auto id2 = rand() % 5 + 1;
		nextPuyoList_.push_back(
			{
				std::make_shared<Puyo>(Vector2{ 16, 16 }, static_cast<Puyo_ID>(id1)),
				std::make_shared<Puyo>(Vector2{ 16, 16 }, static_cast<Puyo_ID>(id2))
			});
		no--;
	}

	Vector2 pos1 = {16, 16};
	Vector2 pos2 = { 16, 48 };
	Vector2 addPos = { 32, 16 };

	for (auto puyoPair : nextPuyoList_)
	{
		puyoPair.first->Pos(std::move(pos1));
		puyoPair.second->Pos(std::move(pos2));
		pos1 += addPos;
		pos2 += addPos;
	}
}
