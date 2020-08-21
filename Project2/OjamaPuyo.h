#pragma once
#include "Puyo.h"
class OjamaPuyo :
	public Puyo
{
public:
	OjamaPuyo();
	OjamaPuyo(Vector2&& pos, Puyo_ID id);
	~OjamaPuyo();
	bool Update(int no) override;
	void Draw(void) override;
	void SetStayInterval(int count) override;

private:
	int intervalCnt_;
};

