#pragma once
#include <mutex>
#include <map>
#include <memory>
#include <vector>
#include "TicketMachine.h"

#define lpMyself Myself::GetInstance()
class MouseCtl;

class Myself
{
public:
	static Myself& GetInstance()
	{
		static std::once_flag once;
		std::call_once(once, Create);
		Create();
		return (*s_Instance);
	}
	static void Create()
	{
		if (s_Instance == nullptr)
		{
			s_Instance = new Myself();
		}
	}
	static void Destroy()
	{
		if (s_Instance != nullptr)
		{
			delete s_Instance;
		}
		s_Instance = nullptr;
	}

	bool Run(void);							// ﾒｲﾝﾙｰﾌﾟ
	void MergeChange(MapInt& change);			// お釣りの受け取り


private:
	Myself();
	~Myself();

	void Draw(void);

	bool SysInit(void);						// ｼｽﾃﾑ関連初期化
	bool MyInit(void);						// 購買者側の初期化

	const int screen_sizeX;					// ｽｸﾘｰﾝｻｲｽﾞX
	const int screen_sizeY;					// ｽｸﾘｰﾝｻｲｽﾞY
	const int money_sizeX;					// moneyｲﾒｰｼﾞX
	const int money_sizeY;					// moneyｲﾒｰｼﾞY
	const int font_size;					// ﾌｫﾝﾄｻｲｽﾞ

	SharedMouse _mouse;		// ﾏｳｽのｲﾝｽﾀﾝｽ

	//std::map<std::string, int> _images;		// 画像情報格納
	MapInt _cash;							// お金の枚数

	static Myself* s_Instance;
};

