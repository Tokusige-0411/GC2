#pragma once
#include <memory>
#include <vector>
#include <map>
#include <random>
#include "Scene/BaseScene.h"
#include "Field.h"
#include "Vector2.h"
#include "input/INPUT_ID.h"
#include "PLAYER_NUM.h"

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
	Handle,				// 画像ﾊﾝﾄﾞﾙ
	X,					// 座標X
	Y,					// 座標Y
	Rad,				// 角度
	ZOrder,				// Zｵｰﾀﾞｰ
	Layer,				// ﾚｲﾔｰ
	DrawMode,	
	DrawNum,
	Type,
	Max
};

// 描画するときに必要な情報<ﾊﾝﾄﾞﾙ, X, Y, rad, Zorder, Layer, imageかeffectか>
using DrawQueT = std::tuple<int, int, int, double, float, Layer, int, int, DrawType>;

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

	bool AddDrawQue(DrawQueT que);

	const Vector2 GetScreenCenter(void);

private:
	bool SysInit();
	void DrawImage(int handle, const Vector2 pos, double rad);
	void DrawEffect(int handle, const Vector2 pos, double rad);
	SceneMng();
	~SceneMng();

	const Vector2 screenSize_;
	const Vector2 screenCenter_;

	unique_Base activeScene_;											// 動いているｼｰﾝ

	int frame_;
	std::vector<DrawQueT> drawList_;									// DrawQueを貯めとく奴
	std::map<DrawType, void (SceneMng::*)(int, Vector2)> drawSet_;		// ｴﾌｪｸﾄを描画するのかImageを描画するのか

	std::mt19937 _mt;
};

