#include <DxLib.h>
#include "GameScene.h"
#include "../input/PadInput.h"
#include "../NetWark/NetWark.h"
#include "../TileLoader.h"
#include "../common/imageMng.h"

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
		lpTileLoader.TMXLoader("MapData.tmx");
	}

	// œØÃﬂ¡ØÃﬂì«Ç›çûÇ›
	auto chipData = lpTileLoader.GetTsxInfo();
	lpImageMng.GetID("map", chipData.imageName, Vector2{ chipData.tileWidth, chipData.tileHeight }, Vector2{ chipData.width, chipData.height });

	return true;
}

unique_Base GameScene::Update(unique_Base own)
{
	(*input_)();
	SetDrawScreen(screenID_);
	Draw();
	return std::move(own);
}

void GameScene::Draw(void)
{
	auto mapData = lpTileLoader.GetMapData();
	auto mapInfo = lpTileLoader.GetTmxInfo();

	auto draw = [&](std::string key) {
		for (int y = 0; y < mapInfo.height; y++)
		{
			for (int x = 0; x < mapInfo.width; x++)
			{
				if (mapData[key][y * mapInfo.width + x])
				{
					DrawGraph(x * mapInfo.tileWidth, y * mapInfo.tileHeight, lpImageMng.GetID("map")[mapData[key][y * mapInfo.width + x] - 1], true);
				}
			}
		}
	};
	draw("Bg");
	draw("Item");
	draw("Obj");
	draw("Char");
}

GameScene::GameScene()
{
	Init();
}

GameScene::~GameScene()
{
}
