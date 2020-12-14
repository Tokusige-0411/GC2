#pragma once
#include <memory>
#include <vector>
#include <functional>
#include <string>
#include "BaseScene.h"
#include "../NetWark/NetWorkState.h"
#include "../NetWark/NetWark.h"
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

	void SetNetWorkModeDraw(void);
	void StartInitDraw(void);
	void SetHostIPDraw(void);

	std::map<UpdateMode, std::function<void()>> func_;		// ﾈｯﾄﾜｰｸの状態ごとに更新を変える
	std::map<UpdateMode, std::function<void()>> drawFunc_;	// 状態ごとに
	ArrayIP ipData_;										// IPアドレス格納
	std::vector<std::string> reConnectIP_;
	std::string select_;
	bool reConnect_;										// 再接続管理ﾌﾗｸﾞ
	bool gameStart_;										// ｹﾞｰﾑｽﾀｰﾄﾌﾗｸﾞ
	UpdateMode updateMode_;									// ｱｯﾌﾟﾃﾞｰﾄ
};
