#include <DxLib.h>
#include "ResultScene.h"

ResultScene::ResultScene()
{
	Init();
}

ResultScene::~ResultScene()
{
}

bool ResultScene::Init()
{
	return true;
}

unique_Base ResultScene::Update(unique_Base own, double delta)
{
	return std::move(own);
}

void ResultScene::Draw(void)
{
	SetDrawScreen(screenID_);
	DrawOwnScreen();
}

void ResultScene::DrawOwnScreen(void)
{

}
