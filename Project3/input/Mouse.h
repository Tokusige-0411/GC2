#pragma once
#include "Controller.h"

class Mouse :
    public Controller
{
    ContType GetType() override;
    bool SetUp(int no) override;
    void Update(void) override;

private:
    int _keyData{};
    std::map<INPUT_ID, int> _inputTbl;
};

