#pragma once
#include <memory>
#include <vector>
#include <functional>
#include <string>
#include "BaseScene.h"
#include "../NetWark/NetWorkState.h"
#include "../common/Vector2.h"
#include "SceneMng.h"

enum class UpdateMode
{
	SetNetworkMode,
	StartInit,
	GameStart,
	SethostIP,
};

class LoginScene :
	public BaseScene
{
public:
	LoginScene();
	~LoginScene();
	bool Init() override;
	unique_Base Update(unique_Base own, double delta) override;
	void Draw(void) override;

private:
	void DrawOwnScreen(void) override;

	void SetNetWorkMode(void);								// ﾈｯﾄﾜｰｸﾓｰﾄﾞ設定
	void StartInit(void);									// ｹﾞｰﾑに必要な情報の初期化
	void SetHostIP(void);									// 接続先のIPｱﾄﾞﾚｽを入力する							

	std::map<UpdateMode, std::function<void()>> func_;		// ﾈｯﾄﾜｰｸの状態ごとに更新を変える
	bool reConnect_;										// 再接続管理ﾌﾗｸﾞ
	bool gameStart_;										// ｹﾞｰﾑｽﾀｰﾄﾌﾗｸﾞ
	UpdateMode updateMode_;									// ｱｯﾌﾟﾃﾞｰﾄ
};
