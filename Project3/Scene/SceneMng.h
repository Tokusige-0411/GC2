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
	Handle,				// 画像ﾊﾝﾄﾞﾙ
	X,					// 座標X
	Y,					// 座標Y
	Rad,				// 角度
	Ex,
	ZOrder,				// Zｵｰﾀﾞｰ
	Layer,				// ﾚｲﾔｰ
	Max
};

// 描画するときに必要な情報<ﾊﾝﾄﾞﾙ, X, Y, rad, ex, Zorder, Layer>
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
	bool SysInit();								// ｼｽﾃﾑ情報初期化

	void Draw();								// 描画
		
	SceneMng();
	~SceneMng();

	const Vector2 screenSize_;					// ｽｸﾘｰﾝの大きさ
	const Vector2 screenCenter_;				// ｽｸﾘｰﾝの中心座標

	unique_Base activeScene_;					// 動いているｼｰﾝ

	std::vector<DrawQueT> drawList_;			// 描画するもの

	int frame_;									// ﾌﾚｰﾑｶｳﾝﾄ
	time_point now_;
	time_point befor_;
};

