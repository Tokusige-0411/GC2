#pragma once
#include "BaseScene.h"

class Transition :
	public BaseScene
{
public:
	Transition(unique_Base befor, unique_Base after);
	~Transition();
	unique_Base Update(unique_Base own) override;
	void Draw(void) override;
private:
	bool Init(void);
	unique_Base afterScene_;
	unique_Base drawScene_;
	int black_;
	int fadeCnt_;
};

