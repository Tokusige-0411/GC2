#pragma once
#include <vector>
#include <memory>
#include <chrono>
#include <list>
#include "BaseScene.h"
#include "../input/Controller.h"
#include "../TileLoader.h"
#include <Vector2.h>

union TimeUnion;
class Object;

using sharedObj = std::shared_ptr<Object>;

class GameScene :
    public BaseScene
{
public:
    bool Init(void) override;                           // ������
    unique_Base Update(unique_Base own, double delta) override;       // �X�V
    void Draw(void) override;                           // �`��

    void SetBombObj(int owner, int self, Vector2 pos, int blastLength, bool sendFlag);
    sharedObj GetPlayerObj(int id);
    void SetFireMap(const Vector2& pos, int length);

    GameScene();
    ~GameScene();

private:
    void DrawOwnScreen(void) override;

    MapData mapData_;
    TMXInfo mapInfo_;

    std::chrono::system_clock::time_point start_;					// �b�ԊǗ��J�n
    std::chrono::system_clock::time_point end_;						// �b�ԊǗ��I��
    int timeCnt_;                                                   // ���b�o������

    std::list<sharedObj> objList_;
};

