#pragma once
#include"Vector2.h"

class Field
{
public:
	Field();
	Field(Vector2&& offset, Vector2&& size);
	~Field();
	void Update(void);
	void Draw(void);
	bool Init(void);
	int GetScreenID(void);
	Vector2 GetOffset(void);

private:
	const Vector2 _puyoSize;

	 Vector2 _fieldSize;
	Vector2 _offset;
	Vector2 _puyoPos1;
	Vector2 _puyoPos2;

	int _screenID;
};

