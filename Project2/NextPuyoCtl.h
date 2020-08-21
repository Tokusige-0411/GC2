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

	void Add(int no);				// ������nextList�ɒǉ�
 
private:
	NextList nextPuyoList_;			// nest�Ղ��ؽ�
	int drawCnt_;					// next��\���������
	Vector2 nextPos_;				// next�̈ʒu
	int screenID_;					// next��\��������
};

