#pragma once
#include"Vector2.h"
class MouseCtl
{
public:
	MouseCtl();
	~MouseCtl();
	bool GetClickTrg(void);			// ｸﾘｯｸした瞬間
	bool GetClicking(void);			// ｸﾘｯｸ中
	const Vector2& GetPos(void);	// ﾏｳｽの座標取得
	void Update(void);				// Inputﾃﾞｰﾀの更新

private:
	Vector2 _pos;					// ﾏｳｽの座標
	char mouseData;					// 入力情報
	char mouseDataOld;				// 1ﾌﾚｰﾑ前の入力情報
};

