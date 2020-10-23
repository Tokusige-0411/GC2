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
#include "TitleScene.h"
#include "../NetWark/NetWark.h"
#include "../NetWark/HostState.h"
#include "../NetWark/GestState.h"
#include "../input/PadInput.h"
#include "../TileLoader.h"
#include "../common/imageMng.h"

TitleScene::TitleScene()
{
	Init();

	func_.try_emplace(UpdateMode::SetNetworkMode, std::bind(&TitleScene::SetNetWorkMode, this));
	func_.try_emplace(UpdateMode::SethostIP, std::bind(&TitleScene::SetHostIP, this));
	func_.try_emplace(UpdateMode::PlayerUpdate, std::bind(&TitleScene::PlayerUpdate, this));
	func_.try_emplace(UpdateMode::StartInit, std::bind(&TitleScene::StartInit, this));

	updateMode_ = UpdateMode::SetNetworkMode;
}

TitleScene::~TitleScene()
{
}

bool TitleScene::Init()
{
	GetDrawScreenSize(&screen_size_x_, &screen_size_y_);
	imgHandle_ = LoadGraph("image/mario.png");
	input_ = std::make_unique<PadInput>();
	input_->SetUp(0);
	reConnect_ = false;

	lpTileLoader.TMXLoader();
	lpTileLoader.TSXLoader();
	
	// ϯ�����ߓǂݍ���
	auto chipData = lpTileLoader.GetTsxInfo();
	lpImageMng.GetID("map", "image/" + chipData.fileName, Vector2{ chipData.tileWidth, chipData.tileHeight }, Vector2{ chipData.width, chipData.height });

	return true;
}

unique_Base TitleScene::Update(unique_Base own)
{
	(*input_)();
	if (!lpNetWork.Update())
	{
		updateMode_ = UpdateMode::SetNetworkMode;
	}
	(func_[updateMode_])();
	return own;
}

void TitleScene::Draw(void)
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

void TitleScene::SetNetWorkMode(void)
{
	auto ipdata = lpNetWork.GetIP();

	//TRACE("������IP�A�h���X:%d.%d.%d.%d\n", ipdata.d1, ipdata.d2, ipdata.d3, ipdata.d4);
	for (int i = 0; i < static_cast<int>(ipdata.size()); i++)
	{
		if (ipdata[i].d1 == 192)
		{
			TRACE("�����̃O���[�o��IP�A�h���X:%d.%d.%d.%d\n", ipdata[i].d1, ipdata[i].d2, ipdata[i].d3, ipdata[i].d4);
		}
		else
		{
			TRACE("�����̃��[�J��IP�A�h���X:%d.%d.%d.%d\n", ipdata[i].d1, ipdata[i].d2, ipdata[i].d3, ipdata[i].d4);
		}
	}

	// �O��ڑ�����IP���ڽ
	TRACE("�ڑ����@��I�����Ă�������\n");
	std::ifstream ifs("hostIP.txt");
	if (!ifs)
	{
		TRACE("0:�z�X�g�ɂȂ�\n1:�Q�X�g�ɂȂ�\n\n3:�I�t���C��\n");
	}
	else
	{
		std::string ip;
		ifs >> ip;
		TRACE("0:�z�X�g�ɂȂ�\n1:�Q�X�g�ɂȂ�\n2:�Q�X�g�ɂȂ�(�O��ڑ�����IP�A�h���X�֍Đڑ�)%s\n3:�I�t���C��\n", ip.c_str());
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
			lpNetWork.SetNetWorkMode(NetWorkMode::Gest);
			updateMode_ = UpdateMode::SethostIP;
		}
		else if (mode == 2)
		{
			reConnect_ = true;
			lpNetWork.SetNetWorkMode(NetWorkMode::Gest);
			updateMode_ = UpdateMode::SethostIP;
		}
		else if (mode == 3)
		{
			lpNetWork.SetNetWorkMode(NetWorkMode::Offline);
			updateMode_ = UpdateMode::PlayerUpdate;
		}
	} while (0 > mode || mode > 3);

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
}

void TitleScene::StartInit(void)
{
	// νđ��̏���
	if (lpNetWork.GetNetWorkMode() == NetWorkMode::Host)
	{
		// ������������������stanby�𑗐M
		// ���̑O�ɕϐ�������돉����
		if (lpNetWork.GetActive() == ActiveState::Init)
		{
			// TMX�ް��̻��ޑ��M
			lpTileLoader.SendTmxSizeData();
			lpTileLoader.SendTmxData();
			TRACE("TMX�t�@�C���̃T�C�Y���M\n");
			lpNetWork.SendStanby();
			TRACE("���������𑗐M�A�J�n���}�҂�\n");
		}
		if (lpNetWork.GetActive() == ActiveState::Play)
		{
			updateMode_ = UpdateMode::PlayerUpdate;
			TRACE("�Q�[���X�^�[�g\n");
		}
	}

	if (lpNetWork.GetNetWorkMode() == NetWorkMode::Gest)
	{
		if (lpNetWork.GetActive() == ActiveState::Play)
		{
			updateMode_ = UpdateMode::PlayerUpdate;
			lpNetWork.SendStart();
			TRACE("�Q�[���X�^�[�g��񑗐M\n");
		}
	}
}

void TitleScene::SetHostIP(void)
{
	IPDATA hostIP;
	std::string ip;
	if (reConnect_)
	{
		std::ifstream ifs("hostIP.txt");
		if (ifs)
		{
			ifs >> ip;
		}
		else
		{
			TRACE("�O��ڑ�����IP�A�h���X������܂���\n");
			TRACE("�ڑ����IP�A�h���X�����\n");
			std::cin >> ip;
		}
	}
	else
	{
		TRACE("�ڑ����IP�A�h���X�����\n");
		std::cin >> ip;
	}

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

	if (lpNetWork.GetNetHandle() != -1)
	{
		// ̧�قւ̏����o��
		std::ofstream ofs("hostIP.txt");
		ofs << ip;
		updateMode_ = UpdateMode::StartInit;
		TRACE("�ڑ�����\n");
		TRACE("IP�A�h���X���t�@�C���o��\n");
	}
	else
	{
		TRACE("�ڑ����s\n");
	}
}

void TitleScene::PlayerUpdate(void)
{
	//auto contData = input_->GetContData();
	//if (contData[INPUT_ID::UP][static_cast<int>(Trg::Now)] && contData[INPUT_ID::UP][static_cast<int>(Trg::Old)])
	//{
	//	sendData_.y -= 3;
	//}
	//if (contData[INPUT_ID::RIGHT][static_cast<int>(Trg::Now)] && contData[INPUT_ID::RIGHT][static_cast<int>(Trg::Old)])
	//{
	//	sendData_.x += 3;
	//}
	//if (contData[INPUT_ID::DOWN][static_cast<int>(Trg::Now)] && contData[INPUT_ID::DOWN][static_cast<int>(Trg::Old)])
	//{
	//	sendData_.y += 3;
	//}
	//if (contData[INPUT_ID::LEFT][static_cast<int>(Trg::Now)] && contData[INPUT_ID::LEFT][static_cast<int>(Trg::Old)])
	//{
	//	sendData_.x -= 3;
	//}
}
