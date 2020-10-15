#include <Dxlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include "../_debug/_DebugConOut.h"
#include "TitleScene.h"
#include "../NetWark/NetWark.h"
#include "../NetWark/HostState.h"
#include "../NetWark/GestState.h"
#include "../input/PadInput.h"

TitleScene::TitleScene()
{
	screen_size_x_ = 0;
	screen_size_y_ = 0;
	auto ipdata = lpNetWork.GetIP();

	TRACE("自分のIPアドレス:%d.%d.%d.%d\n",ipdata.d1, ipdata.d2, ipdata.d3, ipdata.d4);
	TRACE("数字を入力してください。\n");
	TRACE("0:ホストになる、1:ゲストになる、2:オフライン\n");
	int mode;

	IPDATA hostIP;

	std::cin >> mode;
	if (mode == 0)
	{
		lpNetWork.SetNetWorkMode(NetWorkMode::Host);
	}
	else if (mode == 1)
	{
		lpNetWork.SetNetWorkMode(NetWorkMode::Gest);
		std::string ip;
		TRACE("接続先のIPアドレスを入力\n");
		std::cin >> ip;
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
	}
	else if (mode == 2)
	{
		lpNetWork.SetNetWorkMode(NetWorkMode::Offline);
	}

	switch (lpNetWork.GetNetWorkMode())
	{
	case NetWorkMode::Offline:
		TRACE("あなたはオフラインです\n");
		break;
	case NetWorkMode::Host:
		TRACE("あなたはホストです\n");
		break;
	case NetWorkMode::Gest:
		TRACE("あなたはゲストです\n");
		break;
	default:
		AST();
		break;
	}
	TRACE("アクティブ状態:%d\n", lpNetWork.GetActive());

	imgHandle_ = LoadGraph("image/mario.png");
	data_.x = lpSceneMng.GetScreenCenter().x;
	data_.y = lpSceneMng.GetScreenCenter().y;
	input_ = std::make_unique<PadInput>();
	input_->SetUp(0);
}

TitleScene::~TitleScene()
{
}

bool TitleScene::Init()
{
	GetDrawScreenSize(&screen_size_x_, &screen_size_y_);
	return true;
}

unique_Base TitleScene::Update(unique_Base own)
{
	lpNetWork.Update();
	(*input_)();

	auto contData = input_->GetContData();
	if (contData[INPUT_ID::UP][static_cast<int>(Trg::Now)] && contData[INPUT_ID::UP][static_cast<int>(Trg::Old)])
	{
		data_.y -= 3;
	}
	if (contData[INPUT_ID::RIGHT][static_cast<int>(Trg::Now)] && contData[INPUT_ID::RIGHT][static_cast<int>(Trg::Old)])
	{
		data_.x += 3;
	}
	if (contData[INPUT_ID::DOWN][static_cast<int>(Trg::Now)] && contData[INPUT_ID::DOWN][static_cast<int>(Trg::Old)])
	{
		data_.y += 3;
	}
	if (contData[INPUT_ID::LEFT][static_cast<int>(Trg::Now)] && contData[INPUT_ID::LEFT][static_cast<int>(Trg::Old)])
	{
		data_.x -= 3;
	}

	int handle = lpNetWork.GetNetHandle();
	NetWorkSend(handle, &data_, sizeof(PlayerData));

	if (GetNetWorkDataLength(handle) >= sizeof(PlayerData))
	{
		NetWorkRecv(handle, &data_, sizeof(PlayerData));
	}

	return own;
}

void TitleScene::Draw(void)
{
	DrawGraph(data_.x, data_.y, imgHandle_, true);
}
