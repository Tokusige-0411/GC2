#pragma once
#include"Vector2.h"
class MouseCtl
{
public:
	MouseCtl();
	~MouseCtl();
	bool GetClickTrg(void);			// �د������u��
	bool GetClicking(void);			// �د���
	const Vector2& GetPos(void);	// ϳ��̍��W�擾
	void Update(void);				// Input�ް��̍X�V

private:
	Vector2 _pos;					// ϳ��̍��W
	char mouseData;					// ���͏��
	char mouseDataOld;				// 1�ڰёO�̓��͏��
};

