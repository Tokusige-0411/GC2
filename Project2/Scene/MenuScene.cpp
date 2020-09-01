#include <Dxlib.h>
#include"../_debug/_DebugConOut.h"
#include "MenuScene.h"
#include "../SceneMng.h"

MenuScene::MenuScene(unique_Base scene, bool draw, bool update)
{
	childScene_ = std::move(scene);
	draw_ = draw;
	update_ = update;
	black_ = LoadGraph("image/BlackOut.png");
	TRACE("ƒ|[ƒY‰æ–Ê");
}

MenuScene::~MenuScene()
{
}

unique_Base MenuScene::Update(unique_Base own)
{
	if (update_)
	{
		childScene_ = childScene_->Update(std::move(childScene_));
	}
	if (CheckHitKey(KEY_INPUT_F2))
	{
		return std::move(childScene_);
	}
	Draw();
	return std::move(own);
}

void MenuScene::Draw(void)
{
	if (draw_)
	{
		childScene_->Draw();
	}
	lpSceneMng.AddDrawQue({
		black_, 
		lpSceneMng.GetScreenCenter().x, lpSceneMng.GetScreenCenter().y, 
		0.0, 0.0f, Layer::Ui, 
		DX_BLENDMODE_ALPHA, 96, DrawType::Image});
}
