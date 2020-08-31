#pragma once
#include<vector>
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
    std::vector<std::unique_ptr<Field>> playerField_;           // ��ڲ԰���
    int ojamaCnt_;                                              // ���v���C���[�̂�����܊Ǘ��ϐ�
    int gameSceneBG_;                                           // �ްѼ�ݔw�i
    bool gameEnd_;                                              // �ްяI������
};

