#include <Dxlib.h>
#include"../_debug/_DebugConOut.h"
#include "MenuScene.h"

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
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawGraph(0, 0, black_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}
