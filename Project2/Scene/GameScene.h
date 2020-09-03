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

    std::vector<std::unique_ptr<Field>> playerField_;           // ﾌﾟﾚｲﾔｰ情報
    int ojamaCnt_;                                              // 他プレイヤーのおじゃま管理変数
    int gameSceneBG_;                                           // ｹﾞｰﾑｼｰﾝ背景
    bool gameEnd_;                                              // ｹﾞｰﾑ終了判定
    bool ownFlag_;                                              // ﾘｻﾞﾙﾄの時に1回しかMakeしないように
    int poseKey_;                                               // ﾎﾟｰｽﾞｷｰ
    int poseKeyOld_;                                            // ﾎﾟｰｽﾞｷｰのﾄﾘｶﾞｰ
    int startCnt_;
    std::array<int, 2> readyGoH_;
    Vector2 logoPos_;
    int logoVec_;
    double ex_;
    int fade_;
};

