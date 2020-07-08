#pragma once
#include<map>
#include"INPUT_ID.h"

using KeyPair = std::pair<int, int>;
using KeyMap = std::map<INPUT_ID, KeyPair>;

class InputState
{
public:
	InputState();
	virtual ~InputState();
	virtual void Update(void) = 0;
	void SetOld(void);							// oldî•ñ‹L‰¯
	const KeyMap& State(void)const;				// ·°‚Ìî•ñ‚ğ‚»‚Ì‚Ü‚Ü•Ô‚·
	const KeyPair& State(INPUT_ID id)const;		// ·°‚Ìî•ñ‚Ì’†g‚ğ•Ô‚·
	bool State(INPUT_ID id, int data);

private:
	KeyMap _state;
};

