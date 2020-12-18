#include <DxLib.h>
#include "GameScene.h"
#include "CrossOver.h"
#include "ResultScene.h"
#include "../input/PadInput.h"
#include "../NetWark/NetWark.h"
#include "../TileLoader.h"
#include "../common/imageMng.h"
#include "../Object/Player.h"
#include "../Object/Bomb.h"
#include "../_debug/_DebugConOut.h"

bool GameScene::Init(void)
{
	mapObj_ = std::make_shared<TileLoader>();
	if (lpNetWork.GetNetWorkMode() == NetWorkMode::Guest)
	{
		mapObj_->TMXLoader("cash/RevData.tmx");
	}
	else
	{
		mapObj_->TMXLoader("TestMap.tmx");
		//lpTileLoader.TMXLoader("MapData.tmx");
	}

	// ﾏｯﾌﾟﾁｯﾌﾟ読み込み
	auto chipData = mapObj_->GetTsxInfo();
	lpImageMng.GetID(ObjectID::Map, chipData.imageName, Vector2{ chipData.tileWidth, chipData.tileHeight }, Vector2{ chipData.width, chipData.height });
	mapData_ = mapObj_->GetMapData();
	mapInfo_ = mapObj_->GetTmxInfo();

	// mapのcharの位置にplayerをｲﾝｽﾀﾝｽ
	int instanceCnt = 0;
	for (int i = 0; i < mapInfo_.height * mapInfo_.width; i++)
	{
		if (mapData_["Char"][i])
		{
			objList_.emplace_back(std::make_shared<Player>(instanceCnt, Vector2{ (i % mapInfo_.width) * mapInfo_.tileWidth, (i / mapInfo_.width) * mapInfo_.tileHeight }, mapObj_, *this));
			instanceCnt += UNIT_ID_NUM;
			if (lpNetWork.GetNetWorkMode() != NetWorkMode::Offline)
			{
				if (instanceCnt / UNIT_ID_NUM >= lpNetWork.GetPlayerInf().second)
				{
					break;
				}
			}
		}
	}

	lpImageMng.GetID(ObjectID::Fire, "image/fire.png", { 32, 32 }, { 3, 4 });

	return true;
}

unique_Base GameScene::Update(unique_Base own, double delta)
{
	if (lpNetWork.GetStartState() == StartState::GameStart)
	{
		mapObj_->FireUpdate(delta);

		objList_.sort([](sharedObj& a, sharedObj& b) {
			return a->IsPickUp() > b->IsPickUp();
			});

		int aliveCnt = 0;
		for (auto& data : objList_)
		{
			if (!data->Update())
			{
				Player::fallCnt++;
			}
			if (!data->GetDeth())
			{
				aliveCnt++;
			}
		}
		objList_.remove_if([](sharedObj& obj) { return (obj->GetDeth() && obj->GetLost()); });

		// ｱｸﾃｨﾌﾞなｵﾌﾞｼﾞｪｸﾄが1つになったらﾘｻﾞﾙﾄへ
		if (aliveCnt <= 1)
		{
			for (auto& data : objList_)
			{
				if (!data->GetDeth())
				{
					lpNetWork.SetResult(data->GetObjectID());
				}
			}
			lpNetWork.SendResult();
			own = std::make_unique<CrossOver>(std::make_unique<ResultScene>(), std::move(own));
		}
	}

	DrawOwnScreen();

	// 何回情報を落としたか
	end_ = std::chrono::system_clock::now();
	if (std::chrono::duration_cast<std::chrono::seconds>(end_ - start_).count() >= 1)
	{
		timeCnt_++;
		start_ = end_;
	}
	DrawBox(190, 0, 220, 15, 0x000000, true);
	DrawFormatString(200, 0, 0xffffff, "%d", Player::fallCnt / timeCnt_);

	return std::move(own);
}

void GameScene::Draw(void)
{
	if (lpNetWork.GetStartState() == StartState::Countdown)
	{
		auto now = lpSceneMng.GetTime();
		auto time = GAME_START_TIME - (std::chrono::duration_cast<std::chrono::milliseconds>(now - lpNetWork.GetStartTime()).count());
		if (time < 0)
		{
			lpNetWork.SetStartState(StartState::GameStart);
			TRACE("ゲームスタート\n");
		}
	}
	mapObj_->Draw();
	for (auto& data : objList_)
	{
		if (!(data->GetDeth()))
		{
			data->Draw();
		}
	}
}

void GameScene::SetBombObj(int owner, int self, Vector2 pos, int blastLength, bool sendFlag)
{
	if (sendFlag)
	{
		objList_.emplace_back(std::make_shared<Bomb>(owner, self, blastLength, pos, mapObj_, *this));
	}
}

void GameScene::SetEnptyObj(void)
{
	objList_.emplace_back(std::make_shared<Object>());
}

sharedObj GameScene::GetPlayerObj(int id)
{
	for (auto& obj : objList_)
	{
		if (obj->GetObjectID() == id)
		{
			return obj;
		}
	}
	sharedObj obj = nullptr;
	return obj;
}

void GameScene::SetFireMap(const Vector2& pos, int length)
{
	mapObj_->SetFireGenerator(pos, length);
}

GameScene::GameScene()
{
	Init();
}

GameScene::~GameScene()
{
}

void GameScene::DrawOwnScreen(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	Draw();
}
