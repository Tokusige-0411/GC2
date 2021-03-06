#pragma once
#include<memory>
#include<map>
#include"Vector2.h"
#include"input/INPUT_ID.h"

enum class Puyo_ID
{
	NON,
	RED,
	BRUE,
	GREEN,
	YELLOW,
	PURPLE,
	OJAMA,
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
	Puyo();
	Puyo(Vector2&& pos, Puyo_ID id);
	virtual ~Puyo();

	virtual bool Update(int no);
	void Move(INPUT_ID id);
	virtual  void Draw(void);

	const Vector2& Pos(void);
	void Pos(Vector2&& pos);
	const Vector2& Size(void);
	const Vector2 Grid(int size);
	const Puyo_ID& Type(void);
	void SetDirPermit(DirPermit dirParmit);
	const DirPermit& GetDirPermit(void);
	const DirPermit& GetOldDirPermit(void);
	void SetDrawPermit(DirPermit drawPermit);
	void SetDropSpeed(int speed, int interval);

	void SoftDrop(void);

	bool Alive(void);
	void Alive(bool flag);

	void SetPuyon(int num);
	bool CheckPuyon(void);

	void SetMunyon(void);
	bool CheckMunyon(void);
	void ResetMunyon(void);

	int GetColor(void);

	virtual void SetStayInterval(int count) {};

private:
	void Init(void);

protected:
	const Vector2 puyoSize_;					// ぷよのｻｲｽﾞ
	DirPermit dirPermit_;						// どこに動けるかのﾌﾗｸﾞ
	DirPermit oldDirPermit_;					// 1ﾌﾚｰﾑ前のどこに動けるかのﾌﾗｸﾞ
	DirPermit drawPermit_;						// どこに伸ばすのかのﾌﾗｸﾞ

	std::map<Puyo_ID, int> colorMap_;			// ｶﾗｰｺｰﾄﾞ格納ﾏｯﾌﾟ

	Vector2 pos_;								// 座標
	Puyo_ID puyoID_;							// ぷよの種類
	bool alive_;								// 消えるか消えないか
	int color_;									// 色

	int dropInt_;								// ﾄﾞﾛｯﾌﾟのｲﾝﾀｰﾊﾞﾙ
	int dropCnt_;								// ﾄﾞﾛｯﾌﾟの間隔ｶｳﾝﾄ
	int dropSpeed_;								// ﾄﾞﾛｯﾌﾟのｽﾋﾟｰﾄﾞ

	int puyonCnt_;								// ぷよんのｶｳﾝﾀｰ
	int puyonNum_;								// 上から何番目か

	int munyonCnt_;								// むにょんｶｳﾝﾀｰ
};

