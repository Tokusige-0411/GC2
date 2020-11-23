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

	// ﾏｯﾌﾟﾁｯﾌﾟ読み込み
	auto chipData = mapObj_->GetTsxInfo();
	lpImageMng.GetID("map", chipData.imageName, Vector2{ chipData.tileWidth, chipData.tileHeight }, Vector2{ chipData.width, chipData.height });
	mapData_ = mapObj_->GetMapData();
	mapInfo_ = mapObj_->GetTmxInfo();

	// mapのcharの位置にplayerをｲﾝｽﾀﾝｽ
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

	return true;
}

unique_Base GameScene::Update(unique_Base own)
{
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
	auto draw = [&](std::string key) {
		for (int y = 0; y < mapInfo_.height; y++)
		{
			for (int x = 0; x < mapInfo_.width; x++)
			{
				if (mapData_[key][y * mapInfo_.width + x])
				{
					DrawGraph(x * mapInfo_.tileWidth, y * mapInfo_.tileHeight, lpImageMng.GetID("map")[mapData_[key][y * mapInfo_.width + x] - 1], true);
				}
			}
		}
	};
	draw("Bg");
	draw("Item");
	draw("Obj");
	draw("Char");
	
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
