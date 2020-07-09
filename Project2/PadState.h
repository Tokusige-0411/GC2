#pragma once
#include "InputState.h"

class PadState :
	public InputState
{
public:
	PadState();
	~PadState();
	void Update(void);

private:
	int _padID;
};

