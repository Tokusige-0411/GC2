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
	void Run(void);							// ���ڰю��s
	//bool InsertCash(int cash);				// ������t����
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

	std::map<std::string, int> _images;						// �摜���i�[
	std::map<PayType, std::function<void(void)>> _draw;
	std::map<PayType, bool(TicketMachine::*)()> _pay;

	std::map<PayType, std::function<bool(PayType&, MapInt&, int)>> _insert;

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
	const int pay_btn_sizeX;								// ���ݻ���X
	const int pay_btn_sizeY;								// ���ݻ���Y

	std::vector<int> _moneyType;							// �����̎��
	SharedMouse _mouse;										// ϳ��̏��

	std::string _btnKey;									// �\���������݂ւ̷�
	Vector2 _btnPos;										// ���݂�\��������W

	PayType _payType;										// �x�������@
	MapInt _cashDataChange;									// ������ް�

	MapInt _payData;										// �x�������

	bool _paySuccess;										// �x�����������������ǂ���
};

