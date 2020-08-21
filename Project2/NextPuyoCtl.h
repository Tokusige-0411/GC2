#pragma once
#include "Puyo.h"
#include "Field.h"

using NextList = std::list<PairPuyo>;

class NextPuyoCtl
{
public:
	NextPuyoCtl(Vector2& nextPos, int haveCnt, int drawCnt);
	~NextPuyoCtl();
	void Draw(void);
	PairPuyo Pickup(void);

	void Add(int no);				// ˆø”•ªnextList‚É’Ç‰Á
 
private:
	NextList nextPuyoList_;			// nest‚Õ‚æ‚ÌØ½Ä
	int drawCnt_;					// next‚ğ•\¦‚³‚¹‚éŒÂ”
	Vector2 nextPos_;				// next‚ÌˆÊ’u
	int screenID_;					// next‚ğ•\¦‚³‚¹‚é
};

