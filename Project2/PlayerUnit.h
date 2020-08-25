#pragma once
#include "Field.h"

class PlayerUnit
{
public:
	PlayerUnit(Field& field);
	~PlayerUnit();
	int Update(int targetID);

private:
	Field& field_;
	int targetID_;
};

