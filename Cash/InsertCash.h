#pragma once
#include"TicketMachine.h"

struct InsertCash
{
	void operator()(MapInt& cashData, PairInt& cardData, int cash)
	{
		cashData.try_emplace(cash, 0);
		cashData[cash]++;
	}
};
