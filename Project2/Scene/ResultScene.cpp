#include "ResultScene.h"
#include "../_debug/_DebugConOut.h"

ResultScene::ResultScene(unique_Base scene, bool draw, bool update)
{
	childScene_ = std::move(scene);
	draw_ = draw;
	update_ = update;
	TRACE("Ø»ÞÙÄ¼°Ý");
}

ResultScene::~ResultScene()
{
}

unique_Base ResultScene::Update(unique_Base own)
{
	if (update_)
	{
		childScene_ = childScene_->Update(std::move(childScene_));
	}
	Draw();
	return std::move(own);
}

void ResultScene::Draw(void)
{
	if (draw_)
	{
		childScene_->Draw();
	}
}
