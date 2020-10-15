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

	TRACE("������IP�A�h���X:%d.%d.%d.%d\n",ipdata.d1, ipdata.d2, ipdata.d3, ipdata.d4);
	TRACE("��������͂��Ă��������B\n");
	TRACE("0:�z�X�g�ɂȂ�A1:�Q�X�g�ɂȂ�A2:�I�t���C��\n");
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
		TRACE("�ڑ����IP�A�h���X�����\n");
		std::cin >> ip;
		// IPDATA�ɕϊ�����ip��hostIP�ɓ����
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

		TRACE("�ڑ����IP�A�h���X:%d.%d.%d.%d\n", hostIP.d1, hostIP.d2, hostIP.d3, hostIP.d4);
		lpNetWork.ConnectHost(hostIP);
	}
	else if (mode == 2)
	{
		lpNetWork.SetNetWorkMode(NetWorkMode::Offline);
	}

	switch (lpNetWork.GetNetWorkMode())
	{
	case NetWorkMode::Offline:
		TRACE("���Ȃ��̓I�t���C���ł�\n");
		break;
	case NetWorkMode::Host:
		TRACE("���Ȃ��̓z�X�g�ł�\n");
		break;
	case NetWorkMode::Gest:
		TRACE("���Ȃ��̓Q�X�g�ł�\n");
		break;
	default:
		AST();
		break;
	}
	TRACE("�A�N�e�B�u���:%d\n", lpNetWork.GetActive());

	data_ = 0;
	imgHandle_ = LoadGraph("image/mario.png");
	pos_ = lpSceneMng.GetScreenCenter();
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
		pos_.y -= 3;
	}
	if (contData[INPUT_ID::RIGHT][static_cast<int>(Trg::Now)] && contData[INPUT_ID::RIGHT][static_cast<int>(Trg::Old)])
	{
		pos_.x += 3;
	}
	if (contData[INPUT_ID::DOWN][static_cast<int>(Trg::Now)] && contData[INPUT_ID::DOWN][static_cast<int>(Trg::Old)])
	{
		pos_.y += 3;
	}
	if (contData[INPUT_ID::LEFT][static_cast<int>(Trg::Now)] && contData[INPUT_ID::LEFT][static_cast<int>(Trg::Old)])
	{
		pos_.x -= 3;
	}

	int handle = lpNetWork.GetNetHandle();
	NetWorkSend(handle, &pos_, sizeof(int));

	if (GetNetWorkDataLength(handle) >= sizeof(int))
	{
		NetWorkRecv(handle, &data_, sizeof(int));
	}

	return own;
}

void TitleScene::Draw(void)
{
	DrawGraph(pos_.x, pos_.y, imgHandle_, true);
}
