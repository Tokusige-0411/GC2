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
	CASH,		// ����
	CARD,		// IC����
	MAX,		// ���ݒ�
};

class TicketMachine
{
public:
	static TicketMachine& GetInstance()
	{
		static TicketMachine s_Instance;
		return s_Instance;
	}
	void Run(void);							// ���ڰю��s
	bool InsertCash(int cash);				// ������t����
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

	std::map<std::string, int> _images;						// �摜���i�[
	std::map<PayType, std::function<void(void)>> _draw;
	std::map < PayType, bool(TicketMachine::*)()> _pay;

	const int screen_sizeX;									// ��ذݻ���X
	const int screen_sizeY;									// ��ذݻ���Y
	const int money_sizeX;									// money�Ұ��X
	const int money_sizeY;									// money�Ұ��Y
	const int font_size;									// ̫�Ļ���
	const int comment_offset;								// ���ĕ����̵̾��
	const int draw_offsetX;									// �����`��̾��X
	const int draw_offsetY;									// �����`��̾��Y
	const int price_cash;									// �����x�����l�i
	const int price_card;									// ���ގx�����l�i
	const int pay_btn_sizeX;								// 
	const int pay_btn_sizeY;								// 

	std::vector<int> _moneyType;							// �x������@
	SharedMouse _mouse;										// ϳ��̏��

	std::string _btnKey;									// �\���������݂ւ̷�
	Vector2 _btnPos;										// ���݂�\��������W

	PayType _payType;
	MapInt _cashData;
	MapInt _cashDataChange;
	PairInt _cardData;
	bool _paySuccess;
};

