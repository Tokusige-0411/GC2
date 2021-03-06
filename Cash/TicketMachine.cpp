#include <Dxlib.h>
#include <algorithm>
#include "_debug/_DebugConOut.h"
#include "TicketMachine.h"
#include "Myself.h"
#include "InsertMax.h"
#include "InsertCash.h"
#include "InsertCard.h"

void TicketMachine::Run(void)
{
	Vector2 pos = _mouse->GetPos();
	
	auto chackBtn = [&]() {
		if ((pos.x >= _btnPos.x) && (pos.x <= _btnPos.x + pay_btn_sizeX) &&
			(pos.y >= _btnPos.y) && (pos.y <= _btnPos.y + pay_btn_sizeY))
		{
			return true;
		}
		return false;
	};

	//auto chackMoney = [&]() {
	//	if (pos.x < money_sizeX && pos.y < money_sizeY * (_moneyType.size() + 1))
	//	{
	//		return true;
	//	}
	//	return false;
	//};

	if ((_mouse->GetClicking()) && (chackBtn()))
	{
		_btnKey = "btn_push";
	}
	else
	{
		_btnKey = "btn";
	}

	if ((_mouse->GetClickTrg()) && (chackBtn()))
	{
		if (_paySuccess)
		{
			// 決済完了
			lpMyself.MergeChange(_cashDataChange);
			Clear();
		}
		else
		{
			// 決済処理の実行
			if ((this->*_pay[_payType])())
			{
				_paySuccess = true;
			}
		}
	}
}

//bool TicketMachine::InsertCash(int cash)
//{
//	if (_paySuccess)
//	{
//		return false;
//	}
//
//	if (_payType == PayType::MAX)
//	{
//		_payType = PayType::CASH;
//	}
//
//	if (_payType != PayType::CASH)
//	{
//		return false;
//	}
//	_payData.try_emplace(cash, 0);
//	_payData[cash]++;
//	return true;
//}
//
//bool TicketMachine::InsertCard(void)
//{
//	if (_payType == PayType::MAX)
//	{
//		_payType = PayType::CARD;
//	}
//	else
//	{
//		return false;
//	}
//
//	auto data = lpCardServer.GetCardState();
//	_payData.try_emplace(data.first, data.second);
//
//	return true;
//}

bool TicketMachine::Insert(PayType type, int cash)
{
	if (_paySuccess)
	{
		return false;
	}
	if (_payType == PayType::MAX)
	{
		_payType = type;
	}
	return _insert[type](_payType, _payData, cash);
}

void TicketMachine::Draw(void)
{
	// 切符の値段表示
	DrawString(
		screen_sizeX / 2 - font_size,
		money_sizeY / 2,
		"切符の価格　現金:130円　電子マネー:124円",
		0xffffff
	);

	if (_draw.find(_payType) != _draw.end())
	{
		_draw[_payType]();
	}
	//if (_draw.count(_payType))
	//{
	//	_draw[_payType]();
	//}

	DrawBtn();
}

VecInt& TicketMachine::GetMoneyType(void)
{
	return _moneyType;
}

bool TicketMachine::InitDraw(void)
{
	_draw.try_emplace(PayType::MAX, [&]() {
		TRACE("functionのDraw:MAX\n"); 
		DrawGraph(0, 0, _images["money"], true);
		DrawString(0, comment_offset + GetFontSize() / 2,
			"左の枠内の現金かICｶｰﾄﾞを選択しクリックして入金してください。\n入金が完了したら決済ボタンを押してください。",
			0xffffff);
		});
	_draw.try_emplace(PayType::CARD, [&]() {
		TRACE("functionのDraw:CARD\n");
		int moneyLine = 0;
		DrawGraph(0, 0, _images["act_card"], true);
		if (_paySuccess)
		{
			DrawString(0, comment_offset + GetFontSize() / 2,
				"決済完了\nICカードを受け取る際は受け取りボタンを押してください。",
				0xffffff);
			DrawString(draw_offsetX, draw_offsetY, "電子マネー", 0xffffff);
			DrawFormatString(
				draw_offsetX + GetFontSize(),
				draw_offsetY + GetFontSize(),
				0xffffff, "残高　　　　%d円", _payData.begin()->first
			);
			DrawFormatString(
				draw_offsetX + GetFontSize(),
				draw_offsetY + GetFontSize() * 2,
				0xffffff, "引去額　　　%d円", _payData.begin()->second
			);

		}
		else
		{
			DrawString(0, comment_offset + GetFontSize() / 2,
				"左の枠内のICカードをクリックして入金してください。\n入金が完了したら決済ボタンを押してください。",
				0xffffff);
			DrawString(draw_offsetX, draw_offsetY, "電子マネー", 0xffffff);
			DrawFormatString(
				draw_offsetX + GetFontSize(),
				draw_offsetY + GetFontSize(),
				0xffffff, "残高　　　　%d円", _payData.begin()->first
			);
			if (_payData.begin()->first < price_card)
			{
				DrawString(
					draw_offsetX,
					(draw_offsetY + GetFontSize() * 3) + moneyLine * GetFontSize(),
					"金額が足りません", 0xff0000, true
				);
			}
		}
	});
	_draw.try_emplace(PayType::CASH, [&]() {
		TRACE("functionのDraw:CASH\n");
		int moneyLine = 0;
		int totalMoney = 0;
		DrawGraph(0, 0, _images["act_money"], true);
		if (_paySuccess)
		{
			DrawString(0, comment_offset + GetFontSize() / 2,
				"決済完了\nお釣りを受け取る際は受け取りボタンを押してください。",
				0xffffff);
			DrawString(draw_offsetX, draw_offsetY, "投入金額", 0xffffff);
			DrawString(draw_offsetX, draw_offsetY, "　　　　　　　枚数", 0xffffff);

			for (auto moneyData : _payData)
			{
				DrawFormatString(
					draw_offsetX + GetFontSize(),
					(draw_offsetY + GetFontSize()) + moneyLine * GetFontSize(),
					0xffffff, "%d円", moneyData.first
				);
				DrawFormatString(
					draw_offsetX + GetFontSize(),
					(draw_offsetY + GetFontSize()) + moneyLine * GetFontSize(),
					0xffffff, "        %d枚", moneyData.second
				);
				moneyLine++;
				totalMoney += (moneyData.first * moneyData.second);
			}

			DrawFormatString(
				draw_offsetX,
				(draw_offsetY + GetFontSize() * 2) + moneyLine * GetFontSize(),
				0xffffff, "合計金額%d円", totalMoney
			);

			DrawString(draw_offsetX * 2, draw_offsetY, "釣銭", 0xffffff);
			DrawString(draw_offsetX * 2, draw_offsetY, "　　　　　　　枚数", 0xffffff);

			int changeLine = 0;
			for (auto moneyData : _cashDataChange)
			{
				DrawFormatString(
					draw_offsetX * 2,
					(draw_offsetY + GetFontSize()) + changeLine * GetFontSize(),
					0xffffff, "%5d円", moneyData.first
				);
				DrawFormatString(
					draw_offsetX * 2,
					(draw_offsetY + GetFontSize()) + changeLine * GetFontSize(),
					0xffffff, "        %9d枚", moneyData.second
				);
				changeLine++;
				totalMoney += (moneyData.first * moneyData.second);
			}
		}
		else
		{
			DrawString(0, comment_offset + GetFontSize() / 2,
				"左の枠内の現金を選択しクリックして入金してください。\n入金が完了したら決済ボタンを押してください。",
				0xffffff);
			DrawString(draw_offsetX, draw_offsetY, "投入金額", 0xffffff);
			DrawString(draw_offsetX, draw_offsetY, "　　　　　　　枚数", 0xffffff);

			for (auto moneyData : _payData)
			{
				DrawFormatString(
					draw_offsetX + GetFontSize(),
					(draw_offsetY + GetFontSize()) + moneyLine * GetFontSize(),
					0xffffff, "%d円", moneyData.first
				);
				DrawFormatString(
					draw_offsetX + GetFontSize(),
					(draw_offsetY + GetFontSize()) + moneyLine * GetFontSize(),
					0xffffff, "        %d枚", moneyData.second
				);
				moneyLine++;
				totalMoney += (moneyData.first * moneyData.second);
			}

			DrawFormatString(
				draw_offsetX,
				(draw_offsetY + GetFontSize() * 2) + moneyLine * GetFontSize(),
				0xffffff, "合計金額%d円", totalMoney
			);

			if (totalMoney < price_cash)
			{
				DrawString(
					draw_offsetX,
					(draw_offsetY + GetFontSize() * 3) + moneyLine * GetFontSize(),
					"金額が足りません", 0xff0000, true
				);
			}
		}});

	return true;
}

bool TicketMachine::InitPay(void)
{
	_pay.try_emplace(PayType::MAX, &TicketMachine::PayMax);
	_pay.try_emplace(PayType::CARD, &TicketMachine::PayCard);
	_pay.try_emplace(PayType::CASH, &TicketMachine::PayCash);
	return true;
}

bool TicketMachine::PayCash(void)
{
	int totalCash = 0;
	auto tmpCashData = _payData;

	for (auto data : tmpCashData)
	{
		totalCash += data.first * data.second;
	}

	if (totalCash < price_cash)
	{
		return false;
	}

	int credit = price_cash;

	//for (int i = 0; i < _moneyType.size(); i++)
	//{
	//	if (tmpCashData.count(_moneyType[i]) > 0)
	//	{
	//		while (tmpCashData[_moneyType[i]] > 0)
	//		{
	//			credit -= _moneyType[i];
	//			tmpCashData[_moneyType[i]]--;
	//			if (credit <= 0)
	//			{
	//				break;
	//			}
	//		}
	//	}
	//	if (credit <= 0)
	//	{
	//		break;
	//	}
	//}

	for (auto& data : tmpCashData)
	{
		while (data.second)
		{
			credit -= data.first;
			data.second--;
			TRACE("使用したお金 %d円\n", data.first);
			if (credit <= 0)
			{
				break;
			}
		}
		if (credit <= 0)
		{
			_cashDataChange = tmpCashData;
			int change = -credit;

			//for (int i = static_cast<int>(_moneyType.size() - 1); i >= 0; --i)
			//{
			//	while (_moneyType[i] <= change)
			//	{
			//		_cashDataChange.try_emplace(_moneyType[i], 0);
			//		_cashDataChange[_moneyType[i]]++;
			//		change -= _moneyType[i];
			//		TRACE("お釣りに%d円追加\n", _moneyType[i]);
			//	}
			//	if (!change)
			//	{
			//		return true;
			//	}
			//}

			std::for_each(_moneyType.crbegin(), _moneyType.crend(),
				[&](int type) {
				while (type <= change)
				{
					_cashDataChange.try_emplace(type, 0);
					_cashDataChange[type]++;
					change -= type;
					TRACE("お釣りに%d円追加\n", type);
				}
			});
			if (!change)
			{
				return true;
			}

			break;
		}
	}

	return false;
}

bool TicketMachine::PayCard(void)
{
	if (lpCardServer.Payment(price_card))
	{
		auto data = lpCardServer.GetCardState();
		_payData.try_emplace(data.first, data.second);
		return true;
	}
	return false;
}

bool TicketMachine::PayMax(void)
{
	return false;
}

void TicketMachine::Clear(void)
{
	_btnKey = "btn";
	_paySuccess = false;
	_payType = PayType::MAX;
	_cashDataChange.clear();
	_payData.clear();
}

void TicketMachine::DrawBtn(void)
{
	SetFontSize(font_size * 2);

	DrawGraph(_btnPos.x, _btnPos.y, _images[_btnKey], true);
	//_btnPos += font_size / 2;
	std::string btnName = (!_paySuccess ? "　決　済" : " 受け取り");
	DrawString(_btnPos.x + font_size / 2, _btnPos.y + font_size / 2, btnName.c_str(), 0x000000);
}

bool TicketMachine::Init(SharedMouse mouse)
{
	TicketMachine::_mouse = mouse;
	_images.try_emplace("btn", LoadGraph("image/btn.png"));
	_images.try_emplace("btn_push", LoadGraph("image/btn_push.png"));
	// 画像の読み込み
	_images.try_emplace("money", LoadGraph("image/money.png"));
	_images.try_emplace("act_money", LoadGraph("image/active_cash.png"));
	_images.try_emplace("act_card", LoadGraph("image/active_card.png"));

	// UI配置されているお金を追加
	if (_moneyType.size() == 0)
	{
		_moneyType.emplace_back(10);
		_moneyType.emplace_back(50);
		_moneyType.emplace_back(100);
		_moneyType.emplace_back(500);
		_moneyType.emplace_back(1000);
		_moneyType.emplace_back(5000);
		_moneyType.emplace_back(10000);
	}

	_btnPos = {
	(screen_sizeX - money_sizeX * 2) - pay_btn_sizeX,
	static_cast<int>(money_sizeY * (_moneyType.size()))
	};

	InitDraw();
	InitPay();

	_insert.try_emplace(PayType::MAX, InsertMax());
	_insert.try_emplace(PayType::CASH, InsertCash());
	_insert.try_emplace(PayType::CARD, InsertCard());


	return true;
}

TicketMachine::TicketMachine() :
	comment_offset(450),
	draw_offsetX(200),
	draw_offsetY(70),
	price_cash(130),
	price_card(124),
	screen_sizeX(800),
	screen_sizeY(600),
	money_sizeX(100),
	money_sizeY(50),
	font_size(18),
	pay_btn_sizeX(200),
	pay_btn_sizeY(50)
{
	Clear();
	TRACE("TicketMachineｲﾝｽﾀﾝｽ");
}


TicketMachine::~TicketMachine()
{
	TRACE("TicketMachineﾃﾞﾘｰﾄ");
}
