#pragma once
#include<memory>
#include<map>
#include"Vector2.h"
#include"input/INPUT_ID.h"

enum class Puyo_Type
{
	NON,
	RED,
	BRUE,
	GREEN,
	YELLOW,
	PURPLE,
	WALL,
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
	bool Update(void);
	void Move(INPUT_ID id);
	void Draw(void);
	const Vector2& Pos(void);
	void Pos(Vector2&& pos);
	const Vector2& Size(void);
	const Vector2 Grid(int size);
	const Puyo_Type& Type(void);
	void SetDirPermit(DirPermit dirParmit);
	const DirPermit& GetDirParmit(void);
	void SoftDrop(void);
	bool Alive(void);
	void Alive(bool flag);
	bool AddPuyonCnt(void);

private:
	const Vector2 _puyoSize;
	DirPermit _dirParmit;

	Vector2 _pos;
	Puyo_Type _puyoID;
	bool alive_;

	int color_;
	std::map<Puyo_Type, int> colorMap_;

	int dropInt_;
	int dropCnt_;
	int dropSpeed_;

	int puyonCnt_;
	int puyonCntMax_;
};

