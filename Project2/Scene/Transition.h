#pragma once
#include "BaseScene.h"
class Transition :
	public BaseScene
{
public:
	Transition();
	~Transition();
	unique_Base Update(unique_Base own) override;
	void Draw(void) override;
private:

};

