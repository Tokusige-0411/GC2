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
	std::ifstream ifs("ini/hostIP.txt");
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
		TRACE("���Ȃ��̓I�t���C���ł�\n");
		break;
	case NetWorkMode::Host:
		TRACE("���Ȃ��̓z�X�g�ł�\n");
		break;
	case NetWorkMode::Guest:
		TRACE("���Ȃ��̓Q�X�g�ł�\n");
		break;
	default:
		AST();
		break;
	}
}

void LoginScene::StartInit(void)
{
	// νđ��̏���
	if (lpNetWork.GetNetWorkMode() == NetWorkMode::Host)
	{
		// ������������������stanby�𑗐M
		// ���̑O�ɕϐ�������돉����
		if (lpNetWork.GetActive() == ActiveState::Init)
		{
			// TMX�ް��̻��ޑ��M
			mapObj_->SendTmxData();
			lpNetWork.SendStanby();
			TRACE("���������𑗐M�A�J�n���}�҂�\n");
		}
		if (lpNetWork.GetActive() == ActiveState::Play)
		{
			gameStart_ = true;
			TRACE("�Q�[���X�^�[�g\n");
		}
	}

	// �޽đ��̏���
	if (lpNetWork.GetNetWorkMode() == NetWorkMode::Guest)
	{
		if (lpNetWork.GetActive() == ActiveState::Play)
		{
			gameStart_ = true;
			lpNetWork.SendStart();
			TRACE("�Q�[���X�^�[�g��񑗐M\n");
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
		std::ofstream ofs("ini/hostIP.txt");
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
