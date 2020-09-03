#include <Dxlib.h>
#include"../_debug/_DebugConOut.h"
#include "MenuScene.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "Transition.h"
#include "../SceneMng.h"
#include "../input/KeyInput.h"
#include "../input/Mouse.h"
#include "../input/PadInput.h"

MenuScene::MenuScene(unique_Base scene, bool draw, bool update)
{
	childScene_ = std::move(scene);
	draw_ = draw;
	update_ = update;
	menuCount_ = 0;
	cursorPos_.emplace_back(Vector2{ 200, 240 });
	cursorPos_.emplace_back(Vector2{ 200, 340 });
	cursorPos_.emplace_back(Vector2{ 200, 440 });
	contVec_.emplace_back(std::make_unique<KeyInput>());
	contVec_.emplace_back(std::make_unique<PadInput>());
	contVec_.emplace_back(std::make_unique<Mouse>());
	for (auto&& cont : contVec_)
	{
		cont->SetUp(0);
	}
	poseMenu_ = LoadGraph("image/PoseMenu.png");
	menuCursor_ = LoadGraph("image/MenuCursor.png");
	black_ = LoadGraph("image/BlackOut.png");
	TRACE("ƒ|[ƒY‰æ–Ê");
}

MenuScene::~MenuScene()
{
}

unique_Base MenuScene::Update(unique_Base own)
{
	for (auto&& cont : contVec_)
	{
		(*cont)();
	}
	if (update_)
	{
		childScene_ = childScene_->Update(std::move(childScene_));
	}
	//if ((poseKey_) && (!poseKeyOld_))
	//{
	//	return std::move(childScene_);
	//}
	// ÒÆ­°¶°¿Ù‚ÌˆÚ“®
	for (auto&& cont : contVec_)
	{
		auto data = (*cont).GetContData();
		if ((data[INPUT_ID::DOWN][static_cast<int>(Trg::Now)] && !data[INPUT_ID::DOWN][static_cast<int>(Trg::Old)]))
		{
			menuCount_++;
			if (menuCount_ >= 3)
			{
				menuCount_ = 0;
			}
		}
		if ((data[INPUT_ID::UP][static_cast<int>(Trg::Now)] && !data[INPUT_ID::UP][static_cast<int>(Trg::Old)]))
		{
			menuCount_--;
			if (menuCount_ < 0)
			{
				menuCount_ = 2;
			}
		}
		if (data[INPUT_ID::R_ROTA][static_cast<int>(Trg::Now)] && !data[INPUT_ID::R_ROTA][static_cast<int>(Trg::Old)])
		{
			switch (menuCount_)
			{
			case(0):
				return std::move(childScene_);
				break;
			case(1):
				return std::make_unique<Transition>(std::move(own), std::make_unique<GameScene>());
				break;
			case(2):
				return std::make_unique<Transition>(std::move(own), std::make_unique<TitleScene>());
				break;
			default:
				break;
			}
		}
	}
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
		0.0, 1.0, 0.0f, Layer::Ui, 
		DX_BLENDMODE_ALPHA, 50, DrawType::Image});

	lpSceneMng.AddDrawQue({
		poseMenu_,
		lpSceneMng.GetScreenCenter().x, lpSceneMng.GetScreenCenter().y,
		0.0, 1.0, 0.5f, Layer::Ui,
		DX_BLENDMODE_NOBLEND, 255, DrawType::Image });

	if ((lpSceneMng.GetFrameCnt() / 45 % 2) == 0)
	{
		lpSceneMng.AddDrawQue({
			menuCursor_,
			cursorPos_[menuCount_].x, cursorPos_[menuCount_].y,
			0.0, 1.0, 1.0f, Layer::Ui,
			DX_BLENDMODE_NOBLEND, 255, DrawType::Image });
	}

}
