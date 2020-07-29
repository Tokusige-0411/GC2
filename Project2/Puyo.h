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
	const Vector2 _puyoSize;					// ‚Õ‚æ‚Ì»²½Ş
	DirPermit dirParmit_;						// ‚Ç‚±‚É“®‚¯‚é‚©‚ÌÌ×¸Ş
	DirPermit drawParmit_;						// ‚Ç‚±‚ÉL‚Î‚·‚Ì‚©‚ÌÌ×¸Ş

	Vector2 pos_;								// À•W
	Puyo_Type puyoID_;							// ‚Õ‚æ‚Ìí—Ş
	bool alive_;								// Á‚¦‚é‚©Á‚¦‚È‚¢‚©

	int color_;									// F
	std::map<Puyo_Type, int> colorMap_;			// ¶×°º°ÄŞŠi”[Ï¯Ìß

	int dropInt_;								// ÄŞÛ¯Ìß‚Ì²İÀ°ÊŞÙ
	int dropCnt_;								// ÄŞÛ¯Ìß‚ÌŠÔŠu¶³İÄ
	int dropSpeed_;								// ÄŞÛ¯Ìß‚Ì½Ëß°ÄŞ

	int puyonCnt_;								// ‚Õ‚æ‚ñ‚Ì¶³İÀ°
	int puyonCntMax_;							// ‚Õ‚æ‚ñŠÔ
};

