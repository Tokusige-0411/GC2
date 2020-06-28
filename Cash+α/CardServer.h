#pragma once
#include <memory>

#define lpCardServer CardServer::GetInstance()

using PairInt = std::pair<int, int>;

class CardServer
{
public:
	static CardServer& GetInstance()
	{
		static CardServer s_Instance;
		return s_Instance;
	}

	PairInt GetCardState(void);		// ¶°ÄŞ‚Ìæ“¾ó‘Ô
	bool Payment(int price);		// ¶°ÄŞx•¥ˆ—

private:
	CardServer();
	~CardServer();

	PairInt _cardData;				// {Œ»İ‚Ìc‚, c‚‚©‚çˆø‚¢‚½‹àŠz};
};

