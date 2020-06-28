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

	PairInt GetCardState(void);		// ���ނ̎擾���
	bool Payment(int price);		// ���ގx������

private:
	CardServer();
	~CardServer();

	PairInt _cardData;				// {���݂̎c��, �c��������������z};
};

