#include <DxLib.h>
#include "Myself.h"
#include "MouseCtl.h"
#include "_debug/_DebugConOut.h"

Myself* Myself::s_Instance = nullptr;

Myself::Myself():
	screen_sizeX(800),screen_sizeY(600),money_sizeX(100),money_sizeY(50),font_size(18)
{
	TRACE("Meselfｲﾝｽﾀﾝｽ");
}

Myself::~Myself()
{
	TRACE("Meselfﾃﾞﾘｰﾄ");
}

bool Myself::Run(void)
{
	// !(SysInit() == true)
	if (!SysInit())
	{
		return false;
	}

	if (!MyInit())
	{
		return false;
	}

	lpTicketMachine.Init(_mouse);

	SetDrawScreen(DX_SCREEN_BACK);

	while (!ProcessMessage() || CheckHitKey(KEY_INPUT_ESCAPE))
	{
		_mouse->Update();
		if (_mouse->GetClickTrg())
		{
			TRACE("クリックした\n");
			const VecInt& _moneyType = lpTicketMachine.GetMoneyType();
			Vector2 pos = _mouse->GetPos();
			if ((pos.x < money_sizeX))
			{
				if ((pos.y < static_cast<int>(_moneyType.size()) * money_sizeY))
				{
					int type = _moneyType[pos.y / money_sizeY];
					TRACE("お金の画像をクリック\n");
					if (_cash[type] > 0)
					{
						if (lpTicketMachine.InsertCash(type))
						{
							_cash[type]--;
						}
						//lpTicketMachine.Insert(type);
						//_cash[type]--;
					}
				}
				else
				{
					if (pos.y < static_cast<int>(_moneyType.size() + 1) * money_sizeY)
					{
						lpTicketMachine.InsertCard();
						//lpTicketMachine.Insert(type);
					}
				}
			}
		}

		lpTicketMachine.Run();

		Draw();
	}

	return true;
}

void Myself::MergeChange(MapInt& change)
{
	for (auto data : change)
	{
		_cash[data.first] += data.second;
	}
	TRACE("お釣りと切符受け取った");
}

void Myself::Draw(void)
{
	int moneyLine = 0;
	ClsDrawScreen();

	SetFontSize(font_size);
	for (auto cashData : _cash)
	{
		DrawFormatString(
			money_sizeX + 25, 
			money_sizeY * moneyLine + money_sizeY / 3, 
			0xffffff, "%2d 枚", cashData.second);
		moneyLine++;
	}

	const VecInt& _moneyType = lpTicketMachine.GetMoneyType();

	// 仕切り線
	DrawLine(
		0, money_sizeY * (_moneyType.size() + 2),
		screen_sizeX, money_sizeY * (_moneyType.size() + 2),
		0xffffff,
		true
	);
	DrawLine(
		money_sizeX * 2, 0,
		money_sizeX * 2, money_sizeY * (_moneyType.size() + 2),
		0xffffff,
		true
	);

	lpTicketMachine.Draw();

	ScreenFlip();
}

bool Myself::SysInit(void)
{
	ChangeWindowMode(true);
	SetGraphMode(screen_sizeX, screen_sizeY, 16);
	SetWindowText("TicketVendingMachine");

	if (DxLib_Init() == -1)
	{
		return false;
	}

	SetFontSize(font_size);
	return true;
}

bool Myself::MyInit(void)
{
	// ﾃﾞﾌｫﾙﾄ
	// std::unique_ptr<MouseCtl>(new MouseCtl()) _mouse;
	_mouse = std::make_shared<MouseCtl>();
	//_mouse.reset(new MouseCtl());

	// 保持する現金の追加
	_cash.try_emplace(10,	15);
	_cash.try_emplace(50,	 3);
	_cash.try_emplace(100,   2);
	_cash.try_emplace(500,	 1);
	_cash.try_emplace(1000,  1);
	_cash.try_emplace(5000,	 1);
	_cash.try_emplace(10000, 1);

	return true;
}
