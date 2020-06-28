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

	bool Run(void);							// Ҳ�ٰ��
	void MergeChange(MapInt& change);			// ���ނ�̎󂯎��


private:
	Myself();
	~Myself();

	void Draw(void);

	bool SysInit(void);						// ���ъ֘A������
	bool MyInit(void);						// �w���ґ��̏�����

	const int screen_sizeX;					// ��ذݻ���X
	const int screen_sizeY;					// ��ذݻ���Y
	const int money_sizeX;					// money�Ұ��X
	const int money_sizeY;					// money�Ұ��Y
	const int font_size;					// ̫�Ļ���

	SharedMouse _mouse;		// ϳ��̲ݽ�ݽ

	//std::map<std::string, int> _images;		// �摜���i�[
	MapInt _cash;							// �����̖���

	static Myself* s_Instance;
};

