#pragma once
#include <vector>
#include "InputState.h"
#include "PLAYER_NUM.h"

class KeyState :
    public InputState
{
public:
    KeyState(PLAYER_NUM player);
    ~KeyState();
    void Update(void) override;

private:
    char _buf[256];                 // ‘S‚Ä‚Ì·°‚Ìî•ñ
    std::vector<int> _keyCon;       // ·°ºİÌ¨¸Şî•ñ
};

