#pragma once
#include "BaseScene.h"
class ResultScene :
	public BaseScene
{
public:
	ResultScene(unique_Base scene, bool draw, bool update);
	~ResultScene();
	unique_Base Update(unique_Base own) override;
	void Draw(void) override;

private:
	unique_Base childScene_;
	bool draw_;
	bool update_;
	int black_;
};

