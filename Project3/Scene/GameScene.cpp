#include <DxLib.h>
#include "GameScene.h"
#include "../input/PadInput.h"
#include "../NetWark/NetWark.h"
#include "../TileLoader.h"
#include "../common/imageMng.h"
#include "../Object/Player.h"

bool GameScene::Init(void)
{
	if (lpNetWork.GetNetWorkMode() == NetWorkMode::Guest)
	{
		lpTileLoader.TMXLoader("cash/RevData.tmx");
	}
	else
	{
		lpTileLoader.TMXLoader("TestMap.tmx");
		//lpTileLoader.TMXLoader("MapData.tmx");
	}

	// œØÃﬂ¡ØÃﬂì«Ç›çûÇ›
	auto chipData = lpTileLoader.GetTsxInfo();
	lpImageMng.GetID("map", chipData.imageName, Vector2{ chipData.tileWidth, chipData.tileHeight }, Vector2{ chipData.width, chipData.height });
	mapData_ = lpTileLoader.GetMapData();
	mapInfo_ = lpTileLoader.GetTmxInfo();

	// mapÇÃcharÇÃà íuÇ…playerÇ≤›Ω¿›Ω
	int instanceCnt = 0;
	for (int y = 0; y < mapInfo_.height; y++)
	{
		for (int x = 0; x < mapInfo_.width; x++)
		{
			if (mapData_["Char"][y * mapInfo_.width + x])
			{
				objList_.emplace_back(std::make_unique<Player>(instanceCnt, Vector2{x * mapInfo_.tileWidth, y * mapInfo_.tileHeight}));
				instanceCnt++;
			}
		}
	}

	return true;
}

unique_Base GameScene::Update(unique_Base own)
{
	std::sort(objList_.begin(), objList_.end(), [](uniqueObj& a, uniqueObj& b) {
		return a->IsPickUp() > b->IsPickUp();
		});

	for (auto& data : objList_)
	{
		if (!data->Update(mapData_))
		{
			Player::fallCnt++;
		}
	}

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
