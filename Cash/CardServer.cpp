#include "CardServer.h"

PairInt CardServer::GetCardState(void)
{
	return _cardData;
}

bool CardServer::Payment(int price)
{
	if (_cardData.first >= price)
	{
		_cardData.first -= price;
		_cardData.second = price;
		return true;
	}
	return false;
}

CardServer::CardServer()
{
	_cardData = { 1000, 0 };
}


CardServer::~CardServer()
{
}
