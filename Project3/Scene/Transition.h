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
    unique_Base nextScene_;         // ŽŸ‚Ì¼°Ý
    unique_Base oldScene_;          // ‘O‚Ì¼°Ý
};

