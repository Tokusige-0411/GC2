#pragma once
#include"TicketMachine.h"

struct InsertCard
{
	bool operator()(PayType& paytype, MapInt& payData, int cash)
	{
		payData = lpCardServer.GetCardState();
	}
};