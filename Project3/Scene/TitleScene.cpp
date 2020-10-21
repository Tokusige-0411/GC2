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

	rapidxml::xml_document<> doc;
	rapidxml::file<> file("MapData.tmx");
	doc.parse<0>(file.data());

	//rapidxml::xml_node<>* node = doc.first_node("map");
	//for (rapidxml::xml_attribute<>* tmp = node->first_attribute(); tmp != nullptr; tmp = tmp->next_attribute())
	//{
	//	std::cout << tmp->value();
	//}
	rapidxml::xml_node<>* node = doc.first_node("map");
	for (rapidxml::xml_node<>* child = node->first_node();
		child != nullptr;
		child = child->next_sibling()) {
		std::cout << child->name() << " : [" << child->value() << "]\n";
	}
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

}

void TitleScene::SetNetWorkMode(void)
{
	auto ipdata = lpNetWork.GetIP();

	//TRACE("自分のIPアドレス:%d.%d.%d.%d\n", ipdata.d1, ipdata.d2, ipdata.d3, ipdata.d4);
	for (int i = 0; i < ipdata.size(); i++)
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
	std::ifstream ifs("hostIP.txt");
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
		else if (mode == 1 || mode == 2)
		{
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
}

void TitleScene::StartInit(void)
{
	// ﾎｽﾄ側の処理
	if (lpNetWork.GetNetWorkMode() == NetWorkMode::Host)
	{
		// 初期化が完了したらstanbyを送信
		// この前に変数もろもろ初期化
		if (lpNetWork.GetActive() == ActiveState::Init)
		{
			lpNetWork.SendStanby();
			TRACE("初期化情報を送信、開始合図待ち\n");
		}
		if (lpNetWork.GetActive() == ActiveState::Play)
		{
			updateMode_ = UpdateMode::PlayerUpdate;
			TRACE("ゲームスタート\n");
		}
	}

	if (lpNetWork.GetNetWorkMode() == NetWorkMode::Gest)
	{
		if (lpNetWork.GetActive() == ActiveState::Play)
		{
			updateMode_ = UpdateMode::PlayerUpdate;
			lpNetWork.SendStart();
			TRACE("ゲームスタート情報送信\n");
		}
	}
}

void TitleScene::SetHostIP(void)
{
	IPDATA hostIP;
	std::string ip;
	std::ifstream ifs("hostIP.txt");
	if (!ifs)
	{
		TRACE("接続先のIPアドレスを入力\n");
		std::cin >> ip;
	}
	else
	{
		ifs >> ip;
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
		std::ofstream ofs("hostIP.txt");
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
