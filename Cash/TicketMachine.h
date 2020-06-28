#pragma once
#include <map>
#include <vector>
#include <string>
#include <memory>
#include "MouseCtl.h"
#include "CardServer.h"

using MapInt = std::map<int, int>;
using VecInt = std::vector<int>;
using SharedMouse = std::shared_ptr<MouseCtl>;

#define lpTicketMachine TicketMachine::GetInstance()

enum class PayType
{
	CASH,		// 現金
	CARD,		// ICｶｰﾄﾞ
	MAX,		// 未設定
};

class TicketMachine
{
public:
	static TicketMachine& GetInstance()
	{
		static TicketMachine s_Instance;
		return s_Instance;
	}
	void Run(void);							// 毎ﾌﾚｰﾑ実行
	bool InsertCash(int cash);				// 入金受付処理
	bool InsertCard(void);
	void Draw(void);
	VecInt& GetMoneyType(void);
	bool Init(SharedMouse mouse);

private:
	bool PayCash(void);
	void Clear(void);
	void DrawBtn(void);

	TicketMachine();
	~TicketMachine();

	std::map<std::string, int> _images;		// 画像情報格納

	const int screen_sizeX;					// ｽｸﾘｰﾝｻｲｽﾞX
	const int screen_sizeY;					// ｽｸﾘｰﾝｻｲｽﾞY
	const int money_sizeX;					// moneyｲﾒｰｼﾞX
	const int money_sizeY;					// moneyｲﾒｰｼﾞY
	const int font_size;					// ﾌｫﾝﾄｻｲｽﾞ
	const int comment_offset;
	const int draw_offsetX;
	const int draw_offsetY;
	const int price_cash;
	const int price_card;
	const int pay_btn_sizeX;
	const int pay_btn_sizeY;

	std::vector<int> _moneyType;			// 支払い手法
	SharedMouse _mouse;						// ﾏｳｽの情報

	std::string _btnKey;					// 表示するﾎﾞﾀﾝへのｷｰ
	Vector2 _btnPos;						// ﾎﾞﾀﾝを表示する座標

	PayType _payType;
	MapInt _cashData;
	MapInt _cashDataChange;
	PairInt _cardData;
	bool _paySuccess;
};

