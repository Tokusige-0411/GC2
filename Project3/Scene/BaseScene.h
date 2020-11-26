#pragma once
#include <memory>
#include "../TileLoader.h"

class BaseScene;

using unique_Base = std::unique_ptr<BaseScene>;

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	virtual bool Init();
	virtual unique_Base Update(unique_Base own, double delta) = 0;
	virtual void Draw(void);
	virtual void DrawOwnScreen(void);

protected:
	int screenID_;
	std::shared_ptr<TileLoader> mapObj_;
};

