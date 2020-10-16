#pragma once
#include <memory>
#include <vector>
#include <functional>
#include "BaseScene.h"
#include "../NetWark/NetWorkState.h"
#include "../common/Vector2.h"
#include "SceneMng.h"
#include "../input/Controller.h"

enum class UpdateMode
{
	SetNetworkMode,
	StartInit,
	PlayerUpdate,
	SethostIP,
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
	void SetNetWorkMode(void);
	void StartInit(void);
	void SetHostIP(void);
	void PlayerUpdate(void);
	std::map<UpdateMode, std::function<void()>> func_;

	std::unique_ptr<Controller> input_;
	int screen_size_x_;
	int screen_size_y_;
	int imgHandle_;

	UpdateMode updateMode_;
};

