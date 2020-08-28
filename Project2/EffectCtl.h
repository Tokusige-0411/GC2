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
	void Draw(int handle, const Vector2 pos);
	bool StopAll(void);
	int Play(std::string name);
	bool Init(Vector2&& size);

private:
	int GetHandles(std::string name);
	EffectCtl();
	~EffectCtl();

	std::map<std::string, int> handles_;		// �̪�čĐ������
	std::list<int> playList_;					// �̪����ڲؽ�
};

