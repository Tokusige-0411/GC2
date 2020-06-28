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

	bool Run(void);							// Ò²İÙ°Ìß
	void MergeChange(MapInt& change);			// ‚¨’Ş‚è‚Ìó‚¯æ‚è


private:
	Myself();
	~Myself();

	void Draw(void);

	bool SysInit(void);						// ¼½ÃÑŠÖ˜A‰Šú‰»
	bool MyInit(void);						// w”ƒÒ‘¤‚Ì‰Šú‰»

	const int screen_sizeX;					// ½¸Ø°İ»²½ŞX
	const int screen_sizeY;					// ½¸Ø°İ»²½ŞY
	const int money_sizeX;					// money²Ò°¼ŞX
	const int money_sizeY;					// money²Ò°¼ŞY
	const int font_size;					// Ì«İÄ»²½Ş

	SharedMouse _mouse;		// Ï³½‚Ì²İ½Àİ½

	//std::map<std::string, int> _images;		// ‰æ‘œî•ñŠi”[
	MapInt _cash;							// ‚¨‹à‚Ì–‡”

	static Myself* s_Instance;
};

