#pragma once
#include "BaseScene.h"

class Transition :
    public BaseScene
{
public:
    Transition();
    ~Transition();
    unique_Base Update(unique_Base own) override;

private:

protected:
    unique_Base nextScene_;
    unique_Base oldScene_;
};

