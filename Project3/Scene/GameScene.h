#pragma once
#include "BaseScene.h"
#include "../input/Controller.h"

class GameScene :
    public BaseScene
{
public:
    bool Init(void) override;                           // ‰Šú‰»
    unique_Base Update(unique_Base own) override;       // XV
    void Draw(void) override;                           // •`‰æ

    GameScene();
    ~GameScene();

private:
    std::unique_ptr<Controller> input_;         // ²İ¯ÌßÄî•ñ
};

