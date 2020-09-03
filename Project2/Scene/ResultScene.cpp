#include<DxLib.h>
#include "ResultScene.h"
#include "../_debug/_DebugConOut.h"
#include "../SceneMng.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "Transition.h"
#include "../input/KeyInput.h"
#include "../input/Mouse.h"
#include "../input/PadInput.h"

ResultScene::ResultScene(unique_Base scene, bool draw, bool update)
{
	childScene_ = std::move(scene);
	draw_ = draw;
	update_ = update;
	Init();
	TRACE("Ø»ÞÙÄ¼°Ý");
}

ResultScene::~ResultScene()
{
}

unique_Base ResultScene::Update(unique_Base own)
{
	for (auto&& cont : contVec_)
	{
		(*cont)();
	}

	// ÒÆ­°¶°¿Ù‚ÌˆÚ“®
	for (auto&& cont : contVec_)
	{
		auto data = (*cont).GetContData();
		if ((data[INPUT_ID::DOWN][static_cast<int>(Trg::Now)] && !data[INPUT_ID::DOWN][static_cast<int>(Trg::Old)]) ||
			(data[INPUT_ID::UP][static_cast<int>(Trg::Now)] && !data[INPUT_ID::UP][static_cast<int>(Trg::Old)]))
		{
			menuCount_++;
			menuCount_ %= 2;
		}
		if (data[INPUT_ID::R_ROTA][static_cast<int>(Trg::Now)] && !data[INPUT_ID::R_ROTA][static_cast<int>(Trg::Old)])
		{
			switch (menuCount_)
			{
			case(0):
				return std::make_unique<Transition>(std::move(own), std::make_unique<GameScene>());
				break;
			case(1):
				return std::make_unique<Transition>(std::move(own), std::make_unique<TitleScene>());
				break;
			default:
				break;
			}
		}
	}

	if (update_)
	{
		childScene_ = childScene_->Update(std::move(childScene_));
	}
	//Draw();
	return std::move(own);
}

void ResultScene::Draw(void)
{
	if (draw_)
	{
		childScene_->Draw();
	}
	lpSceneMng.AddDrawQue({
	black_,
	lpSceneMng.GetScreenCenter().x, lpSceneMng.GetScreenCenter().y,
	0.0, 1.0, 0.0f, Layer::Ui,
	DX_BLENDMODE_ALPHA, 50, DrawType::Image });

	lpSceneMng.AddDrawQue({
	resultMenu_,
	lpSceneMng.GetScreenCenter().x, lpSceneMng.GetScreenCenter().y,
	0.0, 1.0, 0.5f, Layer::Ui,
	DX_BLENDMODE_NOBLEND, 255, DrawType::Image });

	if ((lpSceneMng.GetFrameCnt() / 45 % 2) == 0)
	{
		lpSceneMng.AddDrawQue({
		menuCursor_,
		cursorPos_[menuCount_ % 2].x, cursorPos_[menuCount_ % 2].y,
		0.0, 1.0, 0.5f, Layer::Ui,
		DX_BLENDMODE_NOBLEND, 255, DrawType::Image });
	}
}

bool ResultScene::Init(void)
{
	black_ = LoadGraph("image/BlackOut.png");
	resultMenu_ = LoadGraph("image/ResultMenu.png");
	menuCursor_ = LoadGraph("image/MenuCursor.png");
	menuCount_ = 0;
	cursorPos_.emplace_back(Vector2{ 280, 280 });
	cursorPos_.emplace_back(Vector2{ 280, 390 });
	contVec_.emplace_back(std::make_unique<KeyInput>());
	contVec_.emplace_back(std::make_unique<PadInput>());
	contVec_.emplace_back(std::make_unique<Mouse>());
	for (auto&& cont : contVec_)
	{
		cont->SetUp(0);
	}

	return true;
}
