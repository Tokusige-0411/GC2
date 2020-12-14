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
#include "../NetWark/HostState.h"
#include "../NetWark/GestState.h"
#include "../TileLoader.h"
#include "../common/imageMng.h"

LoginScene::LoginScene()
{
	Init();

	func_.emplace(UpdateMode::SetNetworkMode, std::bind(&LoginScene::SetNetWorkMode, this));
	func_.emplace(UpdateMode::SethostIP, std::bind(&LoginScene::SetHostIP, this));
	func_.emplace(UpdateMode::StartInit, std::bind(&LoginScene::StartInit, this));

	drawFunc_.emplace(UpdateMode::SetNetworkMode, std::bind(&LoginScene::SetNetWorkModeDraw, this));
	drawFunc_.emplace(UpdateMode::SethostIP, std::bind(&LoginScene::SetHostIPDraw, this));
	drawFunc_.emplace(UpdateMode::StartInit, std::bind(&LoginScene::StartInitDraw, this));

	updateMode_ = UpdateMode::SetNetworkMode;
	ipData_ = lpNetWork.GetIP();

	std::ifstream ifs("ini/hostIP.txt");
	if (!ifs)
	{
		select_ = "0:ホストになる\n1:ゲストになる\n\n3:オフライン";
	}
	else
	{
		std::string ip;
		ifs >> ip;
		//TRACE("0:ホストになる\n1:ゲストになる\n2:ゲストになる(前回接続したIPアドレスへ再接続)%s\n3:オフライン\n", ip.c_str());
		select_ = "0:ホストになる\n1:ゲストになる\n2:ゲストになる(前回接続したIPアドレスへ再接続)\n3:オフライン";
	}
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
	for (int i = 0; i < static_cast<int>(ipData_.size()); i++)
	{
		if (ipData_[i].d1 == 192)
		{
			DrawFormatString(0, 0, 0xffffff, "自分のグローバルIPアドレス:%d.%d.%d.%d", ipData_[i].d1, ipData_[i].d2, ipData_[i].d3, ipData_[i].d4);
		}
		else
		{
			DrawFormatString(0, 20, 0xffffff,"自分のローカルIPアドレス:%d.%d.%d.%d", ipData_[i].d1, ipData_[i].d2, ipData_[i].d3, ipData_[i].d4);
		}
	}

	(drawFunc_[updateMode_])();

	if (!lpNetWork.GetConnectFlag())
	{
		DrawString(0, 40, "待機中", 0xffffff);
	}
	else
	{
		auto now = lpSceneMng.GetTime();
		auto time = COUNT_DOWN_MAX - std::chrono::duration_cast<std::chrono::milliseconds>(now - lpNetWork.GetConnectTime()).count();
		if (time < 0)
		{
			time = 0;
		}
		DrawFormatString(0, 40, 0xffffff, "開始まで残り%d秒", time / 1000);
	}
}

void LoginScene::DrawOwnScreen(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	Draw();
}

void LoginScene::SetNetWorkMode(void)
{
	if (CheckHitKey(KEY_INPUT_NUMPAD0))
	{
		lpNetWork.SetNetWorkMode(NetWorkMode::Host);
		updateMode_ = UpdateMode::StartInit;
	}
	else if (CheckHitKey(KEY_INPUT_NUMPAD1))
	{
		lpNetWork.SetNetWorkMode(NetWorkMode::Guest);
		updateMode_ = UpdateMode::SethostIP;
	}
	else if (CheckHitKey(KEY_INPUT_NUMPAD2))
	{
		lpNetWork.SetNetWorkMode(NetWorkMode::Guest);
		updateMode_ = UpdateMode::SethostIP;
		reConnect_ = true;
	}
	else if (CheckHitKey(KEY_INPUT_NUMPAD3))
	{
		lpNetWork.SetNetWorkMode(NetWorkMode::Offline);
		gameStart_ = true;
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
			// 接続されたｹﾞｽﾄ全員にTMXﾃﾞｰﾀを送信
			mapObj_->SendTmxData();

			// 各ゲストにIDと人数を送信
			lpNetWork.SendPlayerID();

			// ｽﾀﾝﾊﾞｲ情報送信
			lpNetWork.SendStanby();

			TRACE("初期化情報を送信、開始合図待ち\n");
		}
		if (lpNetWork.GetActive() == ActiveState::Play)
		{
			lpNetWork.SendCountGame();
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

	if (lpNetWork.GetNetHandleList() != -1)
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

void LoginScene::SetNetWorkModeDraw(void)
{
	DrawString(200, 200, "接続方法を選択してください。", 0xffffff);
	DrawString(200, 220, select_.c_str(), 0xffffff);
}

void LoginScene::StartInitDraw(void)
{
}

void LoginScene::SetHostIPDraw(void)
{
}
