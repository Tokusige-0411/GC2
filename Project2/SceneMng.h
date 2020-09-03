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
	Handle,				// ‰æ‘œÊİÄŞÙ
	X,					// À•WX
	Y,					// À•WY
	Rad,				// Šp“x
	Ex,
	ZOrder,				// Zµ°ÀŞ°
	Layer,				// Ú²Ô°
	DrawMode,	
	DrawNum,
	Type,
	Max
};

// •`‰æ‚·‚é‚Æ‚«‚É•K—v‚Èî•ñ<ÊİÄŞÙ, X, Y, rad, ex, Zorder, Layer, image‚©effect‚©>
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

	bool AddDrawQue(DrawQueT que);

	const Vector2 GetScreenCenter(void);
	void SetGameExit(void);
	int GetFrameCnt(void);

private:
	bool SysInit();
	void DrawImage(int handle, const Vector2 pos, double rad, double ex);
	void DrawEffect(int handle, const Vector2 pos, double rad, double ex);
	SceneMng();
	~SceneMng();

	const Vector2 screenSize_;
	const Vector2 screenCenter_;

	unique_Base activeScene_;											// “®‚¢‚Ä‚¢‚é¼°İ

	int frame_;
	std::vector<DrawQueT> drawList_;									// DrawQue‚ğ’™‚ß‚Æ‚­“z
	std::map<DrawType, void (SceneMng::*)(int, const Vector2, double, double)> drawSet_;		// ´Ìª¸Ä‚ğ•`‰æ‚·‚é‚Ì‚©Image‚ğ•`‰æ‚·‚é‚Ì‚©
	int gameExit_;

	std::mt19937 _mt;
};

