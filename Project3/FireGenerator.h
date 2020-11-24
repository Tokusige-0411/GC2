#pragma once
#include <Vector2.h>

struct FireGenerator
{
	void operator()(void);
	Vector2 pos_;
	int length_;
};
