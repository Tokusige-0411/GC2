#pragma once
#include <memory>

class BaseScene;

using unique_Base = std::unique_ptr<BaseScene>;

class BaseScene
{
public:
	BaseScene();
	~BaseScene();
	virtual unique_Base Update(unique_Base own) = 0;
	virtual void Draw(void) = 0;
};

