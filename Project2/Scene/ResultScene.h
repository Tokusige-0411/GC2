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
	bool Init(void);								// ������

	unique_Base childScene_;						// �w�i
	bool draw_;										// childScene��`�悷�邩
	bool update_;									// childScene���X�V���邩
	int black_;										// �w�i
	int resultMenu_;								// �ƭ����
	int menuCursor_;								// �ƭ����������
	int menuCount_;									// �I���ʒu
	std::vector<Vector2> cursorPos_;				// �ƭ����ق̈ړ��n�_
	std::unique_ptr<Controller> contoroller_;		// Input�֌W;
	std::vector<std::unique_ptr<Controller>> contVec_;
};
