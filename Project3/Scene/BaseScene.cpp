#include <DxLib.h>
#include "BaseScene.h"
#include "SceneMng.h"

BaseScene::BaseScene()
{
	Init();
}

BaseScene::~BaseScene()
{
}

bool BaseScene::Init()
{
	screenID_ = MakeScreen(lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y, true);
	return true;
}

void BaseScene::Draw(void)
{
	DrawGraph(0, 0, screenID_, true);
}

void BaseScene::DrawOwnScreen(void)
{
	Draw();
}
