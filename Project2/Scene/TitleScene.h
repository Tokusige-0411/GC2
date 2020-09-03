#pragma once
#include <vector>
#include "BaseScene.h"
#include "../Vector2.h"
#include "../input/Controller.h"

class TitleScene :
	public BaseScene
{
public:
	TitleScene();
	~TitleScene();
	unique_Base Update(unique_Base own) override;
	void Draw(void) override;

private:
	bool Init(void);
	int titleBg_;
	int title_;
	int menuCursor_;
	int titleMenu_;
	int menuCount_;
	std::vector<Vector2> cursorPos_;
	std::vector<std::unique_ptr<Controller>> contVec_;
};

