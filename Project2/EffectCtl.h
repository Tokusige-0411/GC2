#pragma once
#include<map>
#include<list>
#include<string>
#include"Vector2.h"

#define lpEffectCtl EffectCtl::GetInstance()

class EffectCtl
{
public:
	static EffectCtl& GetInstance()
	{
		static EffectCtl s_Instance;
		return s_Instance;
	}

	void Update(void);
	void Draw(void);
	bool StopAll(void);
	bool Play(std::string name, const Vector2& pos);
	bool Init(Vector2&& size);

private:
	int GetHandles(std::string name);
	EffectCtl();
	~EffectCtl();

	std::map<std::string, int> handles_;		// ´Ìª¸ÄÄ¶ÊİÄŞÙ
	std::list<int> playList_;					// ´Ìª¸ÄÌßÚ²Ø½Ä
};

