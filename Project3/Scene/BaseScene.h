#pragma once
#include <memory>

class BaseScene;

using unique_Base = std::unique_ptr<BaseScene>;

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	virtual bool Init();
	virtual unique_Base Update(unique_Base own) = 0;
	virtual void Draw(void);

protected:
	int screenID_;
};

