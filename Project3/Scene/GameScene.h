#pragma once
#include <vector>
#include <memory>
#include "BaseScene.h"
#include "../input/Controller.h"
#include "../TileLoader.h"

class Object;

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
    MapData mapData_;
    TMXInfo mapInfo_;

    std::vector<std::shared_ptr<Object>> objList_;
};

