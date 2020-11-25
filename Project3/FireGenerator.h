#pragma once
#include <Vector2.h>
#include "TileLoader.h"

struct FireGenerator
{
	void operator()(void);
	Vector2 chipPos;
	int length;
	FireMap& map;
};
