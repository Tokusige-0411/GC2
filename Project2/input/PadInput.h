#pragma once
#include "Controller.h"

struct PadInput :
	public Controller
{
	ContType GetType() override;
	bool SetUp(int no) override;
	void Update(void) override;

private:
	int _padID;
	std::map<INPUT_ID, int> _inputTbl;
};

