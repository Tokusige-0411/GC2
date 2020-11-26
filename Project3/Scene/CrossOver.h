#pragma once
#include "Transition.h"
class CrossOver :
    public Transition
{
public:
    CrossOver(unique_Base next, unique_Base old);
    ~CrossOver();
    bool Init() override;
    unique_Base Update(unique_Base own, double delta) override;
    void Draw(void) override;

private:
    int fadeCnt_;
};

