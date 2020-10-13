#pragma once
#include <memory>
#include <vector>
#include <map>
#include <random>
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
	Char,
	Ui,
	Ex,
	Max
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
	DrawMode,	
	DrawNum,
	Type,
	Max
};

// �`�悷��Ƃ��ɕK�v�ȏ��<�����, X, Y, rad, ex, Zorder, Layer, image��effect��>
using DrawQueT = std::tuple<int, int, int, double, double, float, Layer, int, int, DrawType>;

class SceneMng
{
public:
	static SceneMng& GetInstance()
	{
		static SceneMng s_Instance;
		return s_Instance;
	}

	void Run();
	void Draw();

	const Vector2 GetScreenCenter(void);
	void SetGameExit(void);
	int GetFrameCnt(void);

private:
	bool SysInit();
	SceneMng();
	~SceneMng();

	const Vector2 screenSize_;
	const Vector2 screenCenter_;

	unique_Base activeScene_;											// �����Ă��鼰�

	int frame_;

	std::mt19937 _mt;
	int gameExit_;
};

