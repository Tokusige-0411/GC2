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
    std::vector<std::unique_ptr<Field>> playerField_;           // ﾌﾟﾚｲﾔｰ情報
    int ojamaCnt_;                                              // 他プレイヤーのおじゃま管理変数
    int gameSceneBG_;                                           // ｹﾞｰﾑｼｰﾝ背景
    bool gameEnd_;                                              // ｹﾞｰﾑ終了判定
};

