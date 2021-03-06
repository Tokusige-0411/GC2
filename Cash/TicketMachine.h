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
	void Run(void);							// 毎ﾌﾚｰﾑ実行
	//bool InsertCash(int cash);				// 入金受付処理
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

	std::map<std::string, int> _images;						// 画像情報格納
	std::map<PayType, std::function<void(void)>> _draw;
	std::map<PayType, bool(TicketMachine::*)()> _pay;

	std::map<PayType, std::function<bool(PayType&, MapInt&, int)>> _insert;

	const int screen_sizeX;									// ｽｸﾘｰﾝｻｲｽﾞX
	const int screen_sizeY;									// ｽｸﾘｰﾝｻｲｽﾞY
	const int money_sizeX;									// moneyｲﾒｰｼﾞX
	const int money_sizeY;									// moneyｲﾒｰｼﾞY
	const int font_size;									// ﾌｫﾝﾄｻｲｽﾞ
	const int comment_offset;								// ｺﾒﾝﾄ部分のｵﾌｾｯﾄ
	const int draw_offsetX;									// 文字描画ｵﾌｾｯﾄX
	const int draw_offsetY;									// 文字描画ｵﾌｾｯﾄY
	const int price_cash;									// 現金支払い値段
	const int price_card;									// ｶｰﾄﾞ支払い値段
	const int pay_btn_sizeX;								// ﾎﾞﾀﾝｻｲｽﾞX
	const int pay_btn_sizeY;								// ﾎﾞﾀﾝｻｲｽﾞY

	std::vector<int> _moneyType;							// お金の種類
	SharedMouse _mouse;										// ﾏｳｽの情報

	std::string _btnKey;									// 表示するﾎﾞﾀﾝへのｷｰ
	Vector2 _btnPos;										// ﾎﾞﾀﾝを表示する座標

	PayType _payType;										// 支払い方法
	MapInt _cashDataChange;									// おつりのﾃﾞｰﾀ

	MapInt _payData;										// 支払い情報

	bool _paySuccess;										// 支払いが成功したかどうか
};

