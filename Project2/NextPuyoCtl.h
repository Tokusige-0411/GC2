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

	void Add(int no);				// 引数分nextListに追加
 
private:
	const Vector2 nextScreenSize_;	// nextぷよのｽｸﾘｰﾝぼ大きさ
	NextList nextPuyoList_;			// nestぷよのﾘｽﾄ
	int drawCnt_;					// nextを表示させる個数
	Vector2 nextPos_;				// nextの位置
	int screenID_;					// nextを表示させる
};

