#include <Dxlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include "../_debug/_DebugConOut.h"
#include "LoginScene.h"
#include "CrossOver.h"
#include "GameScene.h"
#include "../NetWark/NetWark.h"
#include "../NetWark/HostState.h"
#include "../NetWark/GestState.h"
#include "../TileLoader.h"
#include "../common/imageMng.h"

LoginScene::LoginScene()
{
	Init();

	func_.try_emplace(UpdateMode::SetNetworkMode, std::bind(&LoginScene::SetNetWorkMode, this));
	func_.try_emplace(UpdateMode::SethostIP, std::bind(&LoginScene::SetHostIP, this));
	func_.try_emplace(UpdateMode::StartInit, std::bind(&LoginScene::StartInit, this));

	updateMode_ = UpdateMode::SetNetworkMode;
}

LoginScene::~LoginScene()
{
}

bool LoginScene::Init()
{
	reConnect_ = false;
	gameStart_ = false;

	mapObj_ = std::make_shared<TileLoader>();

	return true;
}

unique_Base LoginScene::Update(unique_Base own, double delta)
{
	(func_[updateMode_])();
	DrawOwnScreen();
	if (gameStart_)
	{
		own = std::make_unique<CrossOver>(std::make_unique<GameScene>(), std::move(own));
	}
	return own;
}

void LoginScene::Draw(void)
{

}

void LoginScene::DrawOwnScreen(void)
{
	SetDrawScreen(screenID_);
	Draw();
}

void LoginScene::SetNetWorkMode(void)
{
	auto ipdata = lpNetWork.GetIP();

	//TRACE("自分のIPアドレス:%d.%d.%d.%d\n", ipdata.d1, ipdata.d2, ipdata.d3, ipdata.d4);
	for (int i = 0; i < static_cast<int>(ipdata.size()); i++)
	{
		if (ipdata[i].d1 == 192)
		{
			TRACE("自分のグローバルIPアドレス:%d.%d.%d.%d\n", ipdata[i].d1, ipdata[i].d2, ipdata[i].d3, ipdata[i].d4);
		}
		else
		{
			TRACE("自分のローカルIPアドレス:%d.%d.%d.%d\n", ipdata[i].d1, ipdata[i].d2, ipdata[i].d3, ipdata[i].d4);
		}
	}

	// 前回接続したIPｱﾄﾞﾚｽ
	TRACE("接続方法を選択してください\n");
	std::ifstream ifs("ini/hostIP.txt");
	if (!ifs)
	{
		TRACE("0:ホストになる\n1:ゲストになる\n\n3:オフライン\n");
	}
	else
	{
		std::string ip;
		ifs >> ip;
		TRACE("0:ホストになる\n1:ゲストになる\n2:ゲストになる(前回接続したIPアドレスへ再接続)%s\n3:オフライン\n", ip.c_str());
	}

	int mode;
	do 
	{
		std::cin >> mode;
		if (mode == 0)
		{
			lpNetWork.SetNetWorkMode(NetWorkMode::Host);
			updateMode_ = UpdateMode::StartInit;
		}
		else if (mode == 1)
		{
			lpNetWork.SetNetWorkMode(NetWorkMode::Guest);
			updateMode_ = UpdateMode::SethostIP;
		}
		else if (mode == 2)
		{
			reConnect_ = true;
			lpNetWork.SetNetWorkMode(NetWorkMode::Guest);
			updateMode_ = UpdateMode::SethostIP;
		}
		else if (mode == 3)
		{
			lpNetWork.SetNetWorkMode(NetWorkMode::Offline);
			gameStart_ = true;
		}
	} while (0 > mode || mode > 3);

	switch (lpNetWork.GetNetWorkMode())
	{
	case NetWorkMode::Offline:
		TRACE("あなたはオフラインです\n");
		break;
	case NetWorkMode::Host:
		TRACE("あなたはホストです\n");
		break;
	case NetWorkMode::Guest:
		TRACE("あなたはゲストです\n");
		break;
	default:
		AST();
		break;
	}
}

void LoginScene::StartInit(void)
{
	// ﾎｽﾄ側の処理
	if (lpNetWork.GetNetWorkMode() == NetWorkMode::Host)
	{
		// 初期化が完了したらstanbyを送信
		// この前に変数もろもろ初期化
		if (lpNetWork.GetActive() == ActiveState::Init)
		{
			// TMXﾃﾞｰﾀのｻｲｽﾞ送信
			mapObj_->SendTmxData();
			lpNetWork.SendStanby();
			TRACE("初期化情報を送信、開始合図待ち\n");
		}
		if (lpNetWork.GetActive() == ActiveState::Play)
		{
			gameStart_ = true;
			TRACE("ゲームスタート\n");
		}
	}

	// ｹﾞｽﾄ側の処理
	if (lpNetWork.GetNetWorkMode() == NetWorkMode::Guest)
	{
		if (lpNetWork.GetActive() == ActiveState::Play)
		{
			gameStart_ = true;
			lpNetWork.SendStart();
			TRACE("ゲームスタート情報送信\n");
		}
	}
}

void LoginScene::SetHostIP(void)
{
	IPDATA hostIP;
	std::string ip;
	if (reConnect_)
	{
		std::ifstream ifs("ini/hostIP.txt");
		if (ifs)
		{
			ifs >> ip;
		}
		else
		{
			TRACE("前回接続したIPアドレスがありません\n");
			TRACE("接続先のIPアドレスを入力\n");
			std::cin >> ip;
		}
	}
	else
	{
		TRACE("接続先のIPアドレスを入力\n");
		std::cin >> ip;
	}

	// IPDATAに変換したipをhostIPに入れる
	std::istringstream iss(ip);
	std::string str;
	auto ipConvert = [&](unsigned char& Hip) {
		std::getline(iss, str, '.');
		Hip = atoi(str.c_str());
	};
	ipConvert(hostIP.d1);
	ipConvert(hostIP.d2);
	ipConvert(hostIP.d3);
	ipConvert(hostIP.d4);

	TRACE("接続先のIPアドレス:%d.%d.%d.%d\n", hostIP.d1, hostIP.d2, hostIP.d3, hostIP.d4);
	lpNetWork.ConnectHost(hostIP);

	if (lpNetWork.GetNetHandle() != -1)
	{
		// ﾌｧｲﾙへの書き出し
		std::ofstream ofs("ini/hostIP.txt");
		ofs << ip;
		updateMode_ = UpdateMode::StartInit;
		TRACE("接続成功\n");
		TRACE("IPアドレスをファイル出力\n");
	}
	else
	{
		TRACE("接続失敗\n");
	}
}
