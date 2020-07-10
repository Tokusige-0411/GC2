#pragma once
#include<map>
#include<vector>
#include"INPUT_ID.h"

using KeyPair = std::pair<int, int>;
using KeyMap = std::map<INPUT_ID, KeyPair>;

class InputState
{
public:
	InputState();
	virtual ~InputState();
	virtual void Update(void) = 0;
	void SetOld(void);							// old���L��
	const KeyMap& State(void)const;				// ���̏������̂܂ܕԂ�
	const KeyPair& State(INPUT_ID id)const;		// ���̏��̒��g��Ԃ�
	bool State(INPUT_ID id, int data);			// �����ۑ�

private:
	KeyMap _state;								// �L�[���

protected:
	std::vector<int> _keyCon;					// ����̨�ޏ��
};

