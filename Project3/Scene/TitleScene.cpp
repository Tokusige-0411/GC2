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

TitleScene::TitleScene()
{
	screen_size_x_ = 0;
	screen_size_y_ = 0;
	auto ipdata = lpNetWork.GetIP();

	TRACE("自分のIPアドレス:%d.%d.%d.%d",ipdata.d1, ipdata.d2, ipdata.d3, ipdata.d4);
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
		TRACE("接続先のIPアドレスを入力");
		std::cin >> ip;
		// IPDATAに変換したipをhostIPに入れる
		std::istringstream iss(ip);
		std::string str;
		std::array<std::string, 4> array;
		for (int i = 0; i < array.size(); i++)
		{
			std::getline(iss, str, '.');
			array[i] = str;
		}
		hostIP.d1 = atoi(array[0].data());
		hostIP.d2 = atoi(array[1].data());
		hostIP.d3 = atoi(array[2].data());
		hostIP.d4 = atoi(array[3].data());
		TRACE("接続先のIPアドレス:%d.%d.%d.%d", hostIP.d1, hostIP.d2, hostIP.d3, hostIP.d4);
		lpNetWork.ConnectHost(hostIP);
	}
	else if (mode == 2)
	{
		lpNetWork.SetNetWorkMode(NetWorkMode::Offline);
	}

	switch (lpNetWork.GetNetWorkMode())
	{
	case NetWorkMode::Offline:
		TRACE("あなたはオフラインです");
		break;
	case NetWorkMode::Host:
		TRACE("あなたはホストです");
		break;
	case NetWorkMode::Gest:
		TRACE("あなたはゲストです");
		break;
	default:
		AST();
		break;
	}
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
	return own;
}

void TitleScene::Draw(void)
{
	printf("数字を入力してください。\n");
}
