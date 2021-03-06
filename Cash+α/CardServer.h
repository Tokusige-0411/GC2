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

	PairInt GetCardState(void);		// ｶｰﾄﾞの取得状態
	bool Payment(int price);		// ｶｰﾄﾞ支払処理

private:
	CardServer();
	~CardServer();

	PairInt _cardData;				// {現在の残高, 残高から引いた金額};
};

