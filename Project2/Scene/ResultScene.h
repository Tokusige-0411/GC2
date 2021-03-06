#pragma once
#include <vector>
#include "BaseScene.h"
#include "../Vector2.h"
#include"../input/Controller.h"
class ResultScene :
	public BaseScene
{
public:
	ResultScene(unique_Base scene, bool draw, bool update);
	~ResultScene();
	unique_Base Update(unique_Base own) override;
	void Draw(void) override;

private:
	bool Init(void);								// 初期化

	unique_Base childScene_;						// 背景
	bool draw_;										// childSceneを描画するか
	bool update_;									// childSceneを更新するか
	int black_;										// 背景
	int resultMenu_;								// ﾒﾆｭｰ画面
	int menuCursor_;								// ﾒﾆｭｰｶｰｿﾙﾊﾝﾄﾞﾙ
	int menuCount_;									// 選択位置
	std::vector<Vector2> cursorPos_;				// ﾒﾆｭｰｶｰｿﾙの移動地点
	std::unique_ptr<Controller> contoroller_;		// Input関係;
	std::vector<std::unique_ptr<Controller>> contVec_;
};
