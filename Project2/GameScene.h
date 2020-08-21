#pragma once
#include<vector>
#include"BaseScene.h"
#include"Field.h"

class GameScene :
    public BaseScene
{
public:
    GameScene();
    ~GameScene();
    unique_Base Update(unique_Base own) override;
    void Draw(void) override;

private:
    std::vector<std::unique_ptr<Field>> playerField_;
    int ojamaCnt_;                                           // ‘¼ƒvƒŒƒCƒ„[‚Ì‚¨‚¶‚á‚ÜŠÇ—•Ï”
};

