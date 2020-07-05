#pragma once
#include <tuple>
#include"Vector2.h"

class Field
{
public:
	Field();
	Field(Vector2 offset);
	~Field();
	void Update(void);
	void Draw(void);
	int GetScreenID(void);
	Vector2 GetOffset(void);

private:
	const Vector2 _chip;
	const Vector2 _puyoSize;
	const Vector2 _fieldSize;

	Vector2 _offset;
	Vector2 _puyoPos1;
	Vector2 _puyoPos2;

	int _screenID;
};

