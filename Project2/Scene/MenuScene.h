#pragma once
#include <vector>
#include "BaseScene.h"
#include "../Vector2.h"
#include "../input/Controller.h"

class MenuScene :
	public BaseScene
{
public:
	MenuScene(unique_Base scene, bool draw, bool update);
	~MenuScene();
	unique_Base Update(unique_Base own) override;
	void Draw(void) override;

private:
	unique_Base childScene_;
	bool draw_;
	bool update_;
	int black_;
	int poseMenu_;
	int menuCursor_;
	int menuCount_;
	std::vector<Vector2> cursorPos_;
	std::vector<std::unique_ptr<Controller>> contVec_;
};

