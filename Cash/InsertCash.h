#pragma once
#include"TicketMachine.h"

struct InsertCash
{
	bool operator()(PayType& paytype, MapInt& payData, int cash)
	{
		payData.try_emplace(cash, 0);
		payData[cash]++;
	}
};
