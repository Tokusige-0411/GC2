#pragma once
#include"TicketMachine.h"

struct InsertCard
{
	bool operator()(PayType& payType ,MapInt& payData, int cash)
	{
		if (payType != PayType::CARD)
		{
			return false;
		}
		auto data = lpCardServer.GetCardState();
		payData.try_emplace(data.first, data.second);
		return true;
	}
};