#pragma once
#include<vector>
#include<memory>
#include"BaseScene.h"
#include"../Field.h"

class GameScene :
    public BaseScene
{
public:
    GameScene();
    ~GameScene();
    unique_Base Update(unique_Base own) override;
    void Draw(void) override;
    void SetGameEnd(bool flag);
    const bool GetGameEnd(void);

private:
    bool Start(void);

    std::vector<std::unique_ptr<Field>> playerField_;           // ��ڲ԰���
    int ojamaCnt_;                                              // ���v���C���[�̂�����܊Ǘ��ϐ�
    int gameSceneBG_;                                           // �ްѼ�ݔw�i
    bool gameEnd_;                                              // �ްяI������
    bool ownFlag_;                                              // ػ��Ă̎���1�񂵂�Make���Ȃ��悤��
    int poseKey_;                                               // �߰�޷�
    int poseKeyOld_;                                            // �߰�޷����ضް
    int startCnt_;
    std::array<int, 2> readyGoH_;
    Vector2 logoPos_;
    int logoVec_;
    double ex_;
    int fade_;
};

