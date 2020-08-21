#include <DxLib.h>
#include "Mouse.h"

ContType Mouse::GetType()
{
    return ContType::Mouse;
}

bool Mouse::SetUp(int no)
{
    _keyData = 0;

    _inputTbl = {
        {INPUT_ID::DOWN, MOUSE_INPUT_MIDDLE},
        {INPUT_ID::L_ROTA, MOUSE_INPUT_LEFT},
        {INPUT_ID::R_ROTA, MOUSE_INPUT_RIGHT},
    };

    return true;
}

void Mouse::Update(void)
{
    _keyData = GetMouseInput();
    for (auto id : INPUT_ID())
    {
        _data[id][static_cast<int>(Trg::Old)] = _data[id][static_cast<int>(Trg::Now)];
        _data[id][static_cast<int>(Trg::Now)] = false;
    }

    for (auto data : _inputTbl)
    {
        if (data.second & _keyData)
        {
            _data[data.first][static_cast<int>(Trg::Now)] = true;
        }
    }

    int rota = GetMouseWheelRotVol();
    if (rota > 0)
    {
        _data[INPUT_ID::LEFT][static_cast<int>(Trg::Now)] = true;
    }
    else if (rota < 0)
    {
        _data[INPUT_ID::RIGHT][static_cast<int>(Trg::Now)] = true;
    }
}
