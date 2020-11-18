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
    bool Init(void) override;                           // ������
    unique_Base Update(unique_Base own) override;       // �X�V
    void Draw(void) override;                           // �`��

    GameScene();
    ~GameScene();

private:
    void DrawOwnScreen(void) override;

    MapData mapData_;
    TMXInfo mapInfo_;

    std::chrono::system_clock::time_point start_;					// �b�ԊǗ��J�n
    std::chrono::system_clock::time_point end_;						// �b�ԊǗ��I��
    int timeCnt_;                                                   // ���b�o������

    std::vector<uniqueObj> objList_;
};

