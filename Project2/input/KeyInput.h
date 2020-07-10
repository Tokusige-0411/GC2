#pragma once
#include <array>
#include "Controller.h"

struct KeyInput :
    public Controller
{
    ContType GetType() override;
    bool SetUp(int no) override;
    void Update(void) override;

private:
    std::array<char, 256> _keyData;
    std::map<INPUT_ID, int> _inputTbl;
};

