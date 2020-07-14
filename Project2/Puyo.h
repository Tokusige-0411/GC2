#pragma once
#include<memory>
#include"Vector2.h"
#include"input/INPUT_ID.h"

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
	void Move(INPUT_ID id);
	void Draw(void);
	Vector2 Pos(void);
	Vector2 Size(void);

private:
	const Vector2 _puyoSize;
	Vector2 _pos;
	Puyo_Type _puyoID;
};

