#include <DxLib.h>
#include "OjamaPuyo.h"

OjamaPuyo::OjamaPuyo()
{
}

OjamaPuyo::OjamaPuyo(Vector2&& pos, Puyo_ID id)
{
	pos_ = pos;
	puyoID_ = id;
	color_ = 0x888888;
	intervalCnt_ = 0;
}

OjamaPuyo::~OjamaPuyo()
{
}

bool OjamaPuyo::Update(int no)
{
	if (intervalCnt_)
	{
		intervalCnt_--;
	}
	else
	{
		return Puyo::Update(0);
	}
	return true;
}

void OjamaPuyo::Draw(void)
{
	DrawCircle(pos_.x, pos_.y, puyoSize_. x / 2, color_, true);
}

void OjamaPuyo::SetStayInterval(int count)
{
	intervalCnt_ = count / 6 * puyoSize_.y / dropSpeed_;
}
