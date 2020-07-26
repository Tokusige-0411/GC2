#pragma once
#include "Field.h"

class PlayerUnit
{
public:
	PlayerUnit(Field& field);
	~PlayerUnit();
	void Update();

private:
	const Field& field_;
};
