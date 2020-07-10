#include <Dxlib.h>
#include "KeyInput.h"

ContType KeyInput::GetType()
{
    return ContType::KeyBoard;
}

bool KeyInput::SetUp(int no)
{
    if (no == 0)
    {
        _inputTbl = {
            {INPUT_ID::UP, KEY_INPUT_W},
            {INPUT_ID::DOWN, KEY_INPUT_S},
            {INPUT_ID::LEFT, KEY_INPUT_A},
            {INPUT_ID::RIGHT, KEY_INPUT_D},
            {INPUT_ID::R_ROTA, KEY_INPUT_B},
            {INPUT_ID::L_ROTA, KEY_INPUT_V}
        };
    }
    else
    {
        _inputTbl = {
            {INPUT_ID::UP, KEY_INPUT_UP},
            {INPUT_ID::DOWN, KEY_INPUT_DOWN},
            {INPUT_ID::LEFT, KEY_INPUT_LEFT},
            {INPUT_ID::RIGHT, KEY_INPUT_RIGHT},
            {INPUT_ID::R_ROTA, KEY_INPUT_NUMPAD2},
            {INPUT_ID::L_ROTA, KEY_INPUT_NUMPAD1}
        };
    }

    return true;
}

void KeyInput::Update(void)
{
    GetHitKeyStateAll(_keyData.data());

    for (auto id : INPUT_ID())
    {
        _data[id][static_cast<int>(Trg::Old)] = _data[id][static_cast<int>(Trg::Now)];
        _data[id][static_cast<int>(Trg::Now)] = _keyData[_inputTbl[id]];
    }
}
