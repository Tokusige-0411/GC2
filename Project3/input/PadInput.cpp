#include <Dxlib.h>
#include "PadInput.h"

ContType PadInput::GetType()
{
	return ContType::Pad;
}

bool PadInput::SetUp(int no)
{
	_padID = no + 1;

	_inputTbl = {
		{INPUT_ID::UP, XINPUT_BUTTON_DPAD_UP},
		{INPUT_ID::DOWN, XINPUT_BUTTON_DPAD_DOWN},
		{INPUT_ID::LEFT, XINPUT_BUTTON_DPAD_LEFT},
		{INPUT_ID::RIGHT, XINPUT_BUTTON_DPAD_RIGHT},
		{INPUT_ID::SET_BOMB, XINPUT_BUTTON_B},
	};

	return true;
}

void PadInput::Update(void)
{
	XINPUT_STATE input;
	GetJoypadXInputState(_padID, &input);

	for (auto id : INPUT_ID())
	{
		_data[id][static_cast<int>(Trg::Old)] = _data[id][static_cast<int>(Trg::Now)];
		_data[id][static_cast<int>(Trg::Now)] = input.Buttons[_inputTbl[id]];
	}
}
