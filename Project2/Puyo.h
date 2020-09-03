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
	const Vector2 puyoSize_;					// ‚Õ‚æ‚Ì»²½Ş
	DirPermit dirPermit_;						// ‚Ç‚±‚É“®‚¯‚é‚©‚ÌÌ×¸Ş
	DirPermit oldDirPermit_;					// 1ÌÚ°Ñ‘O‚Ì‚Ç‚±‚É“®‚¯‚é‚©‚ÌÌ×¸Ş
	DirPermit drawPermit_;						// ‚Ç‚±‚ÉL‚Î‚·‚Ì‚©‚ÌÌ×¸Ş

	std::map<Puyo_ID, int> colorMap_;			// ¶×°º°ÄŞŠi”[Ï¯Ìß

	Vector2 pos_;								// À•W
	Puyo_ID puyoID_;							// ‚Õ‚æ‚Ìí—Ş
	bool alive_;								// Á‚¦‚é‚©Á‚¦‚È‚¢‚©
	int color_;									// F

	int dropInt_;								// ÄŞÛ¯Ìß‚Ì²İÀ°ÊŞÙ
	int dropCnt_;								// ÄŞÛ¯Ìß‚ÌŠÔŠu¶³İÄ
	int dropSpeed_;								// ÄŞÛ¯Ìß‚Ì½Ëß°ÄŞ

	int puyonCnt_;								// ‚Õ‚æ‚ñ‚Ì¶³İÀ°
	int puyonNum_;								// ã‚©‚ç‰½”Ô–Ú‚©

	int munyonCnt_;								// ‚Ş‚É‚å‚ñ¶³İÀ°
};

