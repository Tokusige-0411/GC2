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
	void SetDrawPermit(DirPermit drawPermit);

	void SoftDrop(void);

	bool Alive(void);
	void Alive(bool flag);

	bool AddPuyonCnt(void);

private:
	const Vector2 _puyoSize;					// ぷよのｻｲｽﾞ
	DirPermit dirParmit_;						// どこに動けるかのﾌﾗｸﾞ
	DirPermit drawParmit_;						// どこに伸ばすのかのﾌﾗｸﾞ

	Vector2 pos_;								// 座標
	Puyo_Type puyoID_;							// ぷよの種類
	bool alive_;								// 消えるか消えないか

	int color_;									// 色
	std::map<Puyo_Type, int> colorMap_;			// ｶﾗｰｺｰﾄﾞ格納ﾏｯﾌﾟ

	int dropInt_;								// ﾄﾞﾛｯﾌﾟのｲﾝﾀｰﾊﾞﾙ
	int dropCnt_;								// ﾄﾞﾛｯﾌﾟの間隔ｶｳﾝﾄ
	int dropSpeed_;								// ﾄﾞﾛｯﾌﾟのｽﾋﾟｰﾄﾞ

	int puyonCnt_;								// ぷよんのｶｳﾝﾀｰ
	int puyonCntMax_;							// ぷよん時間
};

