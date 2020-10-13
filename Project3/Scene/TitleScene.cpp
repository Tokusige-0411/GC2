#include <Dxlib.h>
#include "TitleScene.h"
#include "../NetWark/NetWark.h"
#include "../_debug/_DebugConOut.h"

TitleScene::TitleScene()
{
	screen_size_x_ = 0;
	screen_size_y_ = 0;

	auto ipdata = lpNetWork.GetIP();
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
}
