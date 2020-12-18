#include <Dxlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <fstream>
#include "../_debug/_DebugConOut.h"
#include "LoginScene.h"
#include "CrossOver.h"
#include "GameScene.h"
#include "../NetWark/HostState.h"
#include "../NetWark/GestState.h"
#include "../TileLoader.h"
#include "../common/imageMng.h"
#include "../input/KeyInput.h"

LoginScene::LoginScene()
{
	Init();
}

LoginScene::~LoginScene()
{
}

bool LoginScene::Init()
{
	func_.emplace(UpdateMode::SetNetworkMode, std::bind(&LoginScene::SetNetWorkMode, this));
	func_.emplace(UpdateMode::SethostIP, std::bind(&LoginScene::SetHostIP, this));
	func_.emplace(UpdateMode::StartInit, std::bind(&LoginScene::StartInit, this));

	drawFunc_.emplace(UpdateMode::SetNetworkMode, std::bind(&LoginScene::SetNetWorkModeDraw, this));
	drawFunc_.emplace(UpdateMode::SethostIP, std::bind(&LoginScene::SetHostIPDraw, this));
	drawFunc_.emplace(UpdateMode::StartInit, std::bind(&LoginScene::StartInitDraw, this));

	updateMode_ = UpdateMode::SetNetworkMode;
	ipData_ = lpNetWork.GetIP();

	gameStart_ = false;
	reConnect_ = false;

	std::ifstream ifs("ini/hostIP.txt");
	std::string ip;
	if (!ifs)
	{
		select_ = "0:ホストになる\n1:ゲストになる\n\n3:オフライン";
	}
	else
	{
		do
		{
			ifs >> ip;
			reConnectIP_.emplace_back(ip);
		} while (!ifs.eof());
		select_ = ("0:ホストになる\n1:ゲストになる\n2:ゲストになる(前回接続したIPアドレスへ再接続)\n3:オフライン");
		reConnect_ = true;
	}

	numPad_[0] = '1';
	numPad_[1] = '2';
	numPad_[2] = '3';
	numPad_[3] = '4';
	numPad_[4] = '5';
	numPad_[5] = '6';
	numPad_[6] = '7';
	numPad_[7] = '8';
	numPad_[8] = '9';
	numPad_[9] = '.';
	numPad_[10] = '0';
	numPad_[11] = 'E';

	padBoxPos_ = { 0, 0 };

	input_ = std::make_unique<KeyInput>();
	input_->SetUp(0);

	mapObj_ = std::make_shared<TileLoader>();

	lpNetWork.SetConnectFlag(false);
	lpNetWork.SetStartState(StartState::Wait);

	return true;
}

unique_Base LoginScene::Update(unique_Base own, double delta)
{
	(*input_)();
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
		if (time > 0)
		{
			DrawFormatString(0, 40, 0xffffff, "開始まで残り%d秒", time / 1000);
		}
		else
		{
			DrawFormatString(0, 40, 0xffffff, "準備完了合図待ち\n経過時間:%d秒", abs(time / 1000));
			if (abs(time / 1000) >= 10)
			{
				updateMode_ = UpdateMode::SetNetworkMode;
				lpNetWork.SetConnectFlag(false);
			}
		}
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
		reConnect_ = false;
	}
	else if (CheckHitKey(KEY_INPUT_NUMPAD2))
	{
		if (reConnect_)
		{
			lpNetWork.SetNetWorkMode(NetWorkMode::Guest);
			updateMode_ = UpdateMode::SethostIP;
		}
	}
	else if (CheckHitKey(KEY_INPUT_NUMPAD3))
	{
		lpNetWork.SetNetWorkMode(NetWorkMode::Offline);
		lpNetWork.SetStartState(StartState::GameStart);
		gameStart_ = true;
	}
}

void LoginScene::StartInit(void)
{
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
		}
		if (lpNetWork.GetActive() == ActiveState::Play)
		{
			lpNetWork.SendCountGame();
			gameStart_ = true;
		}
	}

	if (lpNetWork.GetNetWorkMode() == NetWorkMode::Guest)
	{
		if (lpNetWork.GetActive() == ActiveState::Play)
		{
			gameStart_ = true;
			lpNetWork.SendStart();
		}
	}

	if (lpNetWork.GetNetWorkMode() == NetWorkMode::Non)
	{

	}
}

void LoginScene::SetHostIP(void)
{
	IPDATA hostIP;
	auto connectHost = [&]() {	
		// IPDATAに変換したipをhostIPに入れる
		std::istringstream iss(stringIP_);
		std::string str;
		auto ipConvert = [&](unsigned char& Hip) {
			std::getline(iss, str, '.');
			Hip = atoi(str.c_str());
		};
		ipConvert(hostIP.d1);
		ipConvert(hostIP.d2);
		ipConvert(hostIP.d3);
		ipConvert(hostIP.d4); 

		lpNetWork.ConnectHost(hostIP);

		if (lpNetWork.GetNetHandleList() != -1)
		{
			// ﾌｧｲﾙへの書き出し
			std::ofstream ofs("ini/hostIP.txt");
			ofs << stringIP_;
			updateMode_ = UpdateMode::StartInit;
			TRACE("接続成功\n");
			TRACE("IPアドレスをファイル出力\n");
		}
	};

	auto contData = input_->GetContData();

	if (reConnect_)
	{
		stringIP_ = reConnectIP_[0];
		connectHost();
	}
	else
	{
		// 数字を入力してもらうか、パネルを作るか
		if (contData[INPUT_ID::UP][static_cast<int>(Trg::Now)] && !contData[INPUT_ID::UP][static_cast<int>(Trg::Old)])
		{
			if (padBoxPos_.y != 0)
			{
				padBoxPos_.y--;
			}
		}
		if (contData[INPUT_ID::DOWN][static_cast<int>(Trg::Now)] && !contData[INPUT_ID::DOWN][static_cast<int>(Trg::Old)])
		{
			padBoxPos_.y++;
		}
		if (contData[INPUT_ID::LEFT][static_cast<int>(Trg::Now)] && !contData[INPUT_ID::LEFT][static_cast<int>(Trg::Old)])
		{
			if (padBoxPos_.x != 0)
			{
				padBoxPos_.x--;
			}
		}
		if (contData[INPUT_ID::RIGHT][static_cast<int>(Trg::Now)] && !contData[INPUT_ID::RIGHT][static_cast<int>(Trg::Old)])
		{
			padBoxPos_.x++;
		}

		if (contData[INPUT_ID::SET_BOMB][static_cast<int>(Trg::Now)] && !contData[INPUT_ID::SET_BOMB][static_cast<int>(Trg::Old)])
		{
			auto count = (padBoxPos_.y % 4) * 3 + (padBoxPos_.x % 3);
			if (count == 11)
			{
				connectHost();
			}
			else
			{
				stringIP_ += numPad_[count];
			}
		}
	}
}

void LoginScene::SetNetWorkModeDraw(void)
{
	DrawString(200, 200, "接続方法を選択してください。", 0xffffff);
	DrawString(200, 220, select_.c_str(), 0xffffff);
}

void LoginScene::StartInitDraw(void)
{
	auto state = lpNetWork.GetActive();
	if (lpNetWork.GetNetWorkMode() == NetWorkMode::Host)
	{
		if (state == ActiveState::Wait)
		{
			DrawString(200, 200, "接続待ち", 0xffffff);
		}
		if (state == ActiveState::Init)
		{
			DrawString(200, 200, "ゲストからの接続を確認\nゲストへ初期化情報を送信します", 0xffffff);
		}
		if (state == ActiveState::Play)
		{
			DrawString(200, 200, "初期化情報の送信が完了しました\nゲームを開始します", 0xffffff);
		}
	}

	if (lpNetWork.GetNetWorkMode() == NetWorkMode::Guest)
	{
		if (state == ActiveState::Play)
		{
			DrawString(200, 200, "初期化の準備が終了しました\nゲームを開始します", 0xffffff);
		}
	}
}

void LoginScene::SetHostIPDraw(void)
{
	if (!reConnect_)
	{
		DrawString(100, 100, "接続先のIPアドレスを入力してください", 0xffffff);
		DrawString(100, 120, stringIP_.c_str(), 0xffffff);
		for (int i = 0; i < numPad_.size(); i++)
		{
			DrawString(200 + (50 * (i % 3)), 150 + (40 * (i / 3)), numPad_[i].c_str(), 0xffffff);
		}
		Vector2 offSet = {175 + (50 * (padBoxPos_.x % 3)), 140 + (40 * (padBoxPos_.y % 4))};
		DrawBox(offSet.x, offSet.y, offSet.x + 50, offSet.y + 40, 0xffffff, false);
	}
	else
	{
		DrawString(100, 100, "前回接続したIPアドレスへ接続します", 0xffffff);
		DrawFormatString(100, 120, 0xffffff, "接続先:%s", reConnectIP_[0]);
	}
}
