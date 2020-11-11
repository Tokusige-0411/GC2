#include <DxLib.h>
#include "GameScene.h"
#include "../input/PadInput.h"
#include "../NetWark/NetWark.h"
#include "../TileLoader.h"
#include "../common/imageMng.h"
#include "../Object/Player.h"

bool GameScene::Init(void)
{
	input_ = std::make_unique<PadInput>();
	input_->SetUp(0);

	if (lpNetWork.GetNetWorkMode() == NetWorkMode::Gest)
	{
		lpTileLoader.TMXLoader("cash/RevData.tmx");
	}
	else
	{
		lpTileLoader.TMXLoader("TestMap.tmx");
	}

	// œØÃﬂ¡ØÃﬂì«Ç›çûÇ›
	auto chipData = lpTileLoader.GetTsxInfo();
	lpImageMng.GetID("map", chipData.imageName, Vector2{ chipData.tileWidth, chipData.tileHeight }, Vector2{ chipData.width, chipData.height });
	mapData_ = lpTileLoader.GetMapData();
	mapInfo_ = lpTileLoader.GetTmxInfo();

	objList_.emplace_back(std::make_shared<Player>(Vector2{ 32, 32 }));

	return true;
}

unique_Base GameScene::Update(unique_Base own)
{
	(*input_)();

	for (auto& data : objList_)
	{
		data->Update();
	}

	SetDrawScreen(screenID_);
	Draw();

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
