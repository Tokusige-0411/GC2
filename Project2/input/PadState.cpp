#include<DxLib.h>
#include"PadState.h"

PadState::PadState()
{
	_padID = DX_INPUT_PAD1;

	_keyCon.emplace_back(XINPUT_BUTTON_DPAD_UP);
	_keyCon.emplace_back(XINPUT_BUTTON_DPAD_DOWN);
	_keyCon.emplace_back(XINPUT_BUTTON_DPAD_RIGHT);
	_keyCon.emplace_back(XINPUT_BUTTON_DPAD_LEFT);
	_keyCon.emplace_back(XINPUT_BUTTON_B);
	_keyCon.emplace_back(XINPUT_BUTTON_A);
}

PadState::~PadState()
{
}

void PadState::Update(void)
{
	XINPUT_STATE input;

	SetOld();
	GetJoypadXInputState(_padID, &input);
	for (auto id : INPUT_ID())
	{
		State(id, input.Buttons[_keyCon[static_cast<int>(id)]]);
	}
}
