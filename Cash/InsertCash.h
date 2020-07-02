#pragma once
#include"TicketMachine.h"

struct InsertCash
{
	bool operator()(PayType& payType, MapInt& payData, int cash)
	{
		if (payType != PayType::CASH)
		{
			return false;
		}
		payData.try_emplace(cash, 0);
		payData[cash]++;
		return true;
	}
};
