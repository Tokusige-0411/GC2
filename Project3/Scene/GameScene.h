#pragma once
#include <vector>
#include <memory>
#include <chrono>
#include "BaseScene.h"
#include "../input/Controller.h"
#include "../TileLoader.h"

class Object;

using uniqueObj = std::unique_ptr<Object>;

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
    void DrawOwnScreen(void) override;

    MapData mapData_;
    TMXInfo mapInfo_;

    std::chrono::system_clock::time_point start_;					// 秒間管理開始
    std::chrono::system_clock::time_point end_;						// 秒間管理終了
    int timeCnt_;                                                   // 何秒経ったか

    std::vector<uniqueObj> objList_;
};

