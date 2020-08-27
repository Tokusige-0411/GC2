#pragma once
#include "BaseScene.h"
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
};

