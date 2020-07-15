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

struct DirBit
{
	unsigned int up : 1;
	unsigned int down : 1;
	unsigned int left : 1;
	unsigned int right : 1;
};

union DirPermit
{
	DirBit bit;
	unsigned int perBit;
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
	Vector2 Chip(void);
	void SetDirPermit(DirPermit dirParmit);

private:
	const Vector2 _puyoSize;
	DirPermit _dirParmit;
	Vector2 _pos;
	Puyo_Type _puyoID;
};

