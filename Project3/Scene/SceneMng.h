#pragma once
#include <memory>
#include <vector>
#include <map>
#include <chrono>
#include "BaseScene.h"
#include "../common/Vector2.h"

#define lpSceneMng SceneMng::GetInstance()

enum class DrawType
{
	Image,
	Effect,
	Max
};

enum class Layer
{
	Bg,
	Item,
	Obj,
	Char,
};

enum class DrawQue
{
	Handle,				// �摜�����
	X,					// ���WX
	Y,					// ���WY
	Rad,				// �p�x
	Ex,
	ZOrder,				// Z���ް
	Layer,				// ڲ԰
	Max
};

// �`�悷��Ƃ��ɕK�v�ȏ��<�����, X, Y, rad, ex, Zorder, Layer>
using DrawQueT = std::tuple<int, int, int, double, double, float, Layer>;
using time_point = std::chrono::system_clock::time_point;

class SceneMng
{
public:
	static SceneMng& GetInstance()
	{
		static SceneMng s_Instance;
		return s_Instance;
	}

	void Run();

	const Vector2 GetScreenSize(void);
	const Vector2 GetScreenCenter(void);
	const int& GetFrameCnt(void);
	const time_point& GetTime(void);

	void AddDrawQue(DrawQueT dQue);

private:
	bool SysInit();								// ���я�񏉊���

	void Draw();								// �`��
		
	SceneMng();
	~SceneMng();

	const Vector2 screenSize_;					// ��ذ݂̑傫��
	const Vector2 screenCenter_;				// ��ذ݂̒��S���W

	unique_Base activeScene_;					// �����Ă��鼰�

	std::vector<DrawQueT> drawList_;			// �`�悷�����

	int frame_;									// �ڰѶ���
	time_point now_;
	time_point befor_;
};

