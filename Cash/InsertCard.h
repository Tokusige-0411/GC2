#pragma once
#include"TicketMachine.h"

struct InsertCaed
{
	void operator()(MapInt& cashData, PairInt& cardData, int cash)
	{
		cardData = lpCardServer.GetCardState();
	}
};