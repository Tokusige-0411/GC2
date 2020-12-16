#pragma once
#include "BaseScene.h"
class ResultScene :
	public BaseScene
{
public:
	ResultScene();
	~ResultScene();
	bool Init() override;
	unique_Base Update(unique_Base own, double delta)override;
	void Draw(void)override;
	void DrawOwnScreen(void)override;

private:


};

