#pragma once
#include <memory>
#include "BaseScene.h"
#include "../NetWark/NetWorkState.h"
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
	int screen_size_x_;
	int screen_size_y_;
	std::unique_ptr<NetWorkState> netState_;
};

