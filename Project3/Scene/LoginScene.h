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
	unique_Base Update(unique_Base own) override;
	void Draw(void) override;

private:
	void SetNetWorkMode(void);								// ȯ�ܰ�Ӱ�ސݒ�
	void StartInit(void);									// �ްтɕK�v�ȏ��̏�����
	void SetHostIP(void);									// �ڑ����IP���ڽ����͂���							

	std::map<UpdateMode, std::function<void()>> func_;		// ȯ�ܰ��̏�Ԃ��ƂɍX�V��ς���
	bool reConnect_;										// �Đڑ��Ǘ��׸�
	UpdateMode updateMode_;									// �����ްĊǗ�
};
