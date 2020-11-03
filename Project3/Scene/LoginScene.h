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
	PlayerUpdate,
	SethostIP,
};

class LoginScene :
	public BaseScene
{
public:
	LoginScene();
	~LoginScene();
	bool Init() override;
	unique_Base Update(unique_Base own) override;
	void Draw(void) override;

private:
	void SetNetWorkMode(void);
	void StartInit(void);
	void SetHostIP(void);
	void PlayerUpdate(void);

	std::map<UpdateMode, std::function<void()>> func_;
	bool reConnect_;
	UpdateMode updateMode_;
};
