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
	void SetOld(void);							// old情報記憶
	const KeyMap& State(void)const;				// ｷｰの情報をそのまま返す
	const KeyPair& State(INPUT_ID id)const;		// ｷｰの情報の中身を返す
	bool State(INPUT_ID id, int data);			// ｷｰ情報保存

private:
	KeyMap _state;								// キー情報

protected:
	std::vector<int> _keyCon;					// ｷｰｺﾝﾌｨｸﾞ情報
};

