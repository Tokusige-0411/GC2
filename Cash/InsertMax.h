#pragma once
#include"TicketMachine.h"

struct InsertMax
{
	bool operator()(PayType& payType, MapInt& payData, int cash)
	{
		// ｶｰﾄﾞ支払いに行くか、現金支払いに行くか
		// payTypeの変更？
		return true;
	}
};