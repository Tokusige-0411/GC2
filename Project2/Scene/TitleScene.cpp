#include <Dxlib.h>
#include "TitleScene.h"
#include "../SceneMng.h"
#include "GameScene.h"
#include "Transition.h"
#include "../input/KeyInput.h"
#include "../input/Mouse.h"
#include "../input/PadInput.h"

TitleScene::TitleScene()
{
	Init();
}

TitleScene::~TitleScene()
{
}

unique_Base TitleScene::Update(unique_Base own)
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
				lpSceneMng.SetGameExit();
				break;
			default:
				break;
			}
		}
	}
	return std::move(own);
}

void TitleScene::Draw(void)
{
	lpSceneMng.AddDrawQue({
	titleBg_,
	lpSceneMng.GetScreenCenter().x, lpSceneMng.GetScreenCenter().y,
	0.0, 1.0, 0.0f, Layer::Bg,
	DX_BLENDMODE_NOBLEND, 255, DrawType::Image });

	lpSceneMng.AddDrawQue({
	title_,
	lpSceneMng.GetScreenCenter().x, lpSceneMng.GetScreenCenter().y,
	0.0, 1.0, 0.0f, Layer::Ui,
	DX_BLENDMODE_NOBLEND, 255, DrawType::Image });

	lpSceneMng.AddDrawQue({
	titleMenu_,
	lpSceneMng.GetScreenCenter().x, lpSceneMng.GetScreenCenter().y,
	0.0, 1.0, 0.0f, Layer::Ui,
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

bool TitleScene::Init(void)
{
	titleBg_ = LoadGraph("image/GameSceneBG.png");
	title_ = LoadGraph("image/Title.png");
	menuCursor_ = LoadGraph("image/MenuCursor.png");
	titleMenu_ = LoadGraph("image/TitleMenu.png");
	menuCount_ = 0;
	cursorPos_.emplace_back(Vector2{ 280, 410 });
	cursorPos_.emplace_back(Vector2{ 280, 530 });
	contVec_.emplace_back(std::make_unique<KeyInput>());
	contVec_.emplace_back(std::make_unique<PadInput>());
	contVec_.emplace_back(std::make_unique<Mouse>());
	for (auto&& cont : contVec_)
	{
		cont->SetUp(0);
	}
	return true;
}
