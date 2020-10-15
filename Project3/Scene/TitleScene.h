#pragma once
#include <memory>
#include "BaseScene.h"
#include "../NetWark/NetWorkState.h"
#include "../common/Vector2.h"
#include "SceneMng.h"
#include "../input/Controller.h"

struct PlayerData {
	int x, y;
};

class TitleScene :
	public BaseScene
{
public:
	TitleScene();
	~TitleScene();
	bool Init() override;
	unique_Base Update(unique_Base own) override;
	void Draw(void) override;

private:
	std::unique_ptr<Controller> input_;
	int screen_size_x_;
	int screen_size_y_;
	Vector2 pos_;
	PlayerData data_;
	int imgHandle_;
};

