#pragma once
#include"Vector2.h"

class Field
{
public:
	Field();
	Field(Vector2 offset);
	~Field();
	void Draw(void);

private:
	Vector2 _offset;
	int _screenID;
};

