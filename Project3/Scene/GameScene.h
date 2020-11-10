#pragma once
#include "BaseScene.h"
#include "../input/Controller.h"

class GameScene :
    public BaseScene
{
public:
    bool Init(void) override;                           // 初期化
    unique_Base Update(unique_Base own) override;       // 更新
    void Draw(void) override;                           // 描画

    GameScene();
    ~GameScene();

private:
    std::unique_ptr<Controller> input_;         // ｲﾝｯﾌﾟﾄ情報
};

