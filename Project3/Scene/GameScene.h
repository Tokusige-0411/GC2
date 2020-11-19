#pragma once
#include <vector>
#include <memory>
#include <chrono>
#include <list>
#include "BaseScene.h"
#include "../input/Controller.h"
#include "../TileLoader.h"
#include <Vector2.h>

class Object;

using uniqueObj = std::unique_ptr<Object>;

class GameScene :
    public BaseScene
{
public:
    bool Init(void) override;                           // ‰Šú‰»
    unique_Base Update(unique_Base own) override;       // XV
    void Draw(void) override;                           // •`‰æ

    void SetBombObj(int owner, int self, Vector2 pos, bool sendFlag);

    GameScene();
    ~GameScene();

private:
    void DrawOwnScreen(void) override;

    MapData mapData_;
    TMXInfo mapInfo_;

    std::chrono::system_clock::time_point start_;					// •bŠÔŠÇ—ŠJn
    std::chrono::system_clock::time_point end_;						// •bŠÔŠÇ—I—¹
    int timeCnt_;                                                   // ‰½•bŒo‚Á‚½‚©

    std::list<uniqueObj> objList_;
};

