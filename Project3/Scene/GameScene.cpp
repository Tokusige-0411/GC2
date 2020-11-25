#include <DxLib.h>
#include "GameScene.h"
#include "../input/PadInput.h"
#include "../NetWark/NetWark.h"
#include "../TileLoader.h"
#include "../common/imageMng.h"
#include "../Object/Player.h"
#include "../Object/Bomb.h"

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

	// Ï¯ÌßÁ¯Ìß“Ç‚Ýž‚Ý
	auto chipData = mapObj_->GetTsxInfo();
	lpImageMng.GetID("map", chipData.imageName, Vector2{ chipData.tileWidth, chipData.tileHeight }, Vector2{ chipData.width, chipData.height });
	mapData_ = mapObj_->GetMapData();
	mapInfo_ = mapObj_->GetTmxInfo();

	// map‚Ìchar‚ÌˆÊ’u‚Éplayer‚ð²Ý½ÀÝ½
	int instanceCnt = 0;
	for (int y = 0; y < mapInfo_.height; y++)
	{
		for (int x = 0; x < mapInfo_.width; x++)
		{
			if (mapData_["Char"][y * mapInfo_.width + x])
			{
				objList_.emplace_back(std::make_unique<Player>(instanceCnt, Vector2{x * mapInfo_.tileWidth, y * mapInfo_.tileHeight}, mapObj_, *this));
				instanceCnt += UNIT_ID_NUM;
			}
		}
	}

	lpImageMng.GetID("fire", "image/fire.png", { 32, 32 }, { 3, 4 });

	return true;
}

unique_Base GameScene::Update(unique_Base own)
{
	mapObj_->FireUpdate();

	objList_.sort([](uniqueObj& a, uniqueObj& b) {
		return a->IsPickUp() > b->IsPickUp();
		});

	for (auto& data : objList_)
	{
		if (!data->Update())
		{
			Player::fallCnt++;
		}
	}

	objList_.remove_if([](uniqueObj& obj) {return !(obj->GetAlive()); });

	DrawOwnScreen();

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
	mapObj_->Draw();
	for (auto& data : objList_)
	{
		data->Draw();
	}
}

void GameScene::SetBombObj(int owner, int self, Vector2 pos, bool sendFlag)
{
	if (sendFlag)
	{
		objList_.emplace_back(std::make_unique<Bomb>(owner, self, pos, *this));
	}
}

uniqueObj& GameScene::GetPlayerObj(int id)
{
	for (auto& obj : objList_)
	{
		if (obj->GetObjectID() == id)
		{
			return obj;
		}
	}
}

void GameScene::SetFire(const Vector2& pos, int length)
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
	Draw();
}
