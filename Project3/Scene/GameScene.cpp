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

	if (lpNetWork.GetNetWorkMode() == NetWorkMode::Host)
	{
		objList_.emplace_back(std::make_shared<Player>(Vector2{ 32, 32 }));
		objList_.emplace_back(std::make_shared<Player>(Vector2{ 32, 64 }));

		// ≤›Ω¿›ΩèÓïÒÇÃëóêM
		MesPacket plPos;
		for (auto& data : objList_)
		{
			unionData x = { data->GetPos().x };
			unionData y = { data->GetPos().y };
			plPos.emplace_back(x);
			plPos.emplace_back(y);
		}
		lpNetWork.SendMes(plPos, MesType::Instance);
	}

	if (lpNetWork.GetNetWorkMode() == NetWorkMode::Guest)
	{
		auto recvData = lpNetWork.PickMes();
		while (recvData.first.type != MesType::Instance)
		{
			recvData = lpNetWork.PickMes();
		}

		while (recvData.second.size())
		{
			objList_.emplace_back(std::make_shared<Player>(Vector2{ recvData.second[0].iData, recvData.second[1].iData }));
			recvData.second.erase(recvData.second.begin(), recvData.second.begin() + 2);
		}
	}

	if (lpNetWork.GetNetWorkMode() == NetWorkMode::Offline)
	{
		objList_.emplace_back(std::make_shared<Player>(Vector2{ 32, 32 }));
		objList_.emplace_back(std::make_shared<Player>(Vector2{ 32, 64 }));
	}

	return true;
}

unique_Base GameScene::Update(unique_Base own)
{
	for (auto& data : objList_)
	{
		data->Update(mapData_);
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
