#pragma once
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include "MouseCtl.h"
#include "CardServer.h"

using MapInt = std::map<int, int>;
using VecInt = std::vector<int>;
using SharedMouse = std::shared_ptr<MouseCtl>;

#define lpTicketMachine TicketMachine::GetInstance()

enum class PayType
{
	CASH,		// Œ»‹à
	CARD,		// IC¶°ÄŞ
	MAX,		// –¢İ’è
};

class TicketMachine
{
public:
	static TicketMachine& GetInstance()
	{
		static TicketMachine s_Instance;
		return s_Instance;
	}
	void Run(void);							// –ˆÌÚ°ÑÀs
	bool InsertCash(int cash);				// “ü‹àó•tˆ—
	bool InsertCard(void);
	void Draw(void);
	VecInt& GetMoneyType(void);
	bool Init(SharedMouse mouse);

private:
	bool InitDraw(void);
	bool InitPay(void);

	bool PayCash(void);
	bool PayCard(void);
	bool PayMax(void);

	void Clear(void);
	void DrawBtn(void);

	TicketMachine();
	~TicketMachine();

	std::map<std::string, int> _images;						// ‰æ‘œî•ñŠi”[
	std::map<PayType, std::function<void(void)>> _draw;
	std::map < PayType, bool(TicketMachine::*)()> _pay;

	const int screen_sizeX;									// ½¸Ø°İ»²½ŞX
	const int screen_sizeY;									// ½¸Ø°İ»²½ŞY
	const int money_sizeX;									// money²Ò°¼ŞX
	const int money_sizeY;									// money²Ò°¼ŞY
	const int font_size;									// Ì«İÄ»²½Ş
	const int comment_offset;								// ºÒİÄ•”•ª‚ÌµÌ¾¯Ä
	const int draw_offsetX;									// •¶š•`‰æµÌ¾¯ÄX
	const int draw_offsetY;									// •¶š•`‰æµÌ¾¯ÄY
	const int price_cash;									// Œ»‹àx•¥‚¢’l’i
	const int price_card;									// ¶°ÄŞx•¥‚¢’l’i
	const int pay_btn_sizeX;								// 
	const int pay_btn_sizeY;								// 

	std::vector<int> _moneyType;							// x•¥‚¢è–@
	SharedMouse _mouse;										// Ï³½‚Ìî•ñ

	std::string _btnKey;									// •\¦‚·‚éÎŞÀİ‚Ö‚Ì·°
	Vector2 _btnPos;										// ÎŞÀİ‚ğ•\¦‚·‚éÀ•W

	PayType _payType;
	MapInt _cashData;
	MapInt _cashDataChange;
	PairInt _cardData;
	bool _paySuccess;
};

