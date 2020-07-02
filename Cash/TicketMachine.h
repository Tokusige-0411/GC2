#pragma once
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include "MouseCtl.h"
#include "CardServer.h"
#include "PayType.h"

using MapInt = std::map<int, int>;
using VecInt = std::vector<int>;
using SharedMouse = std::shared_ptr<MouseCtl>;
using InsertFunc = std::function<void(PayType&, MapInt&, int)>;

#define lpTicketMachine TicketMachine::GetInstance()

struct InsertMax;
struct InsertCash;
struct InsertCard;

class TicketMachine
{
public:
	static TicketMachine& GetInstance()
	{
		static TicketMachine s_Instance;
		return s_Instance;
	}
	void Run(void);							// –ˆÌÚ°ÑÀs
	//bool InsertCash(int cash);				// “ü‹àó•tˆ—
	//bool InsertCard(void);
	bool Insert(PayType type, int cash);
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
	std::map<PayType, bool(TicketMachine::*)()> _pay;

	std::map<PayType, std::function<bool(PayType&, MapInt&, int)>> _insert;

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
	const int pay_btn_sizeX;								// ÎŞÀİ»²½ŞX
	const int pay_btn_sizeY;								// ÎŞÀİ»²½ŞY

	std::vector<int> _moneyType;							// ‚¨‹à‚Ìí—Ş
	SharedMouse _mouse;										// Ï³½‚Ìî•ñ

	std::string _btnKey;									// •\¦‚·‚éÎŞÀİ‚Ö‚Ì·°
	Vector2 _btnPos;										// ÎŞÀİ‚ğ•\¦‚·‚éÀ•W

	PayType _payType;										// x•¥‚¢•û–@
	MapInt _cashDataChange;									// ‚¨‚Â‚è‚ÌÃŞ°À

	MapInt _payData;										// x•¥‚¢î•ñ

	bool _paySuccess;										// x•¥‚¢‚ª¬Œ÷‚µ‚½‚©‚Ç‚¤‚©
};

