#pragma once
#include"Vector2.h"

enum class Puyo_Type
{
	RED,
	BRUE,
	GREEN,
	YELLOW,
	PURPLE,
	MAX
};

class Puyo
{
public:
	Puyo(Vector2&& pos, Puyo_Type id);
	~Puyo();
	void Update(void);
	void Move(void);
	void Draw(void);

private:
	const Vector2 _puyoSize;
	Vector2 _pos;
	Puyo_Type _puyoID;
};

