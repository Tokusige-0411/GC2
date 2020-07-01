#pragma once
#include"TicketMachine.h"

struct InsertCard
{
	void operator()(MapInt& cashData, PairInt& cardData, int cash)
	{
		cardData = lpCardServer.GetCardState();
	}
};