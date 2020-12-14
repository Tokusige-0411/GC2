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

	void SetNetWorkMode(void);								// ȯ�ܰ�Ӱ�ސݒ�
	void StartInit(void);									// �ްтɕK�v�ȏ��̏�����
	void SetHostIP(void);									// �ڑ����IP���ڽ����͂���	

	void SetNetWorkModeDraw(void);
	void StartInitDraw(void);
	void SetHostIPDraw(void);

	std::map<UpdateMode, std::function<void()>> func_;		// ȯ�ܰ��̏�Ԃ��ƂɍX�V��ς���
	std::map<UpdateMode, std::function<void()>> drawFunc_;	// ��Ԃ��Ƃ�
	ArrayIP ipData_;										// IP�A�h���X�i�[
	std::vector<std::string> reConnectIP_;
	std::string select_;
	bool reConnect_;										// �Đڑ��Ǘ��׸�
	bool gameStart_;										// �ްѽ����׸�
	UpdateMode updateMode_;									// �����ް�
};
