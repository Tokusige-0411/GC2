#pragma once
#include <vector>
#include <map>
#include <mutex>
#include <Vector2.h>
#include "../TileLoader.h"
#include "../NetWark/NetWark.h"
#include "../Scene/BaseScene.h"

enum class AnimState
{
	Normal,
	Down,
	Left,
	Right,
	Up,
	Deth,
	Max
};

enum class Dir
{
	Down,
	Left,
	Right,
	Up,
	Deth,
	Max
};

static Dir begin(Dir) { return Dir::Up; }
static Dir end(Dir) { return Dir::Max; }
static Dir operator*(Dir id) { return id; }
static Dir operator++(Dir& id) { return id = static_cast<Dir>(std::underlying_type<Dir>::type(id) + 1); }

using sharedMap = std::shared_ptr<TileLoader>;
using AnimVector = std::vector<std::pair<int, unsigned int>>;

class Object
{
public:
	Object();
	virtual ~Object();
	virtual bool Update(void);
	virtual bool UpdateDef(void);
	virtual void Draw(void);
	Vector2 GetPos(void);
	bool IsPickUp(void);
	MesPair PickUp(void);
	bool GetAlive(void) { return alive_; };
	bool GetDeth(void) { return deth_; };
	bool GetLost(void) { return lost_; };
	int GetObjectID(void) { return objectID_; };

private:
	virtual void Init(void);
	time_point startTime_;						// 生成した時間

protected:
	Vector2 pos_;								// 座標
	Dir dir_;									// 方向
	bool alive_;								// 生きてるか				
	bool deth_;									// アニメーション終了してるか
	bool lost_;									// ネットワーク切断したか
	int animCnt_;								// アニメーションｶｳﾝﾄ
	std::map<AnimState, AnimVector> animMap_;	// stateでアニメーションを分ける	

	MesPacketList mesList_;						// 情報受け取り口
	std::mutex mtx_;							// 各objectのmutex

	std::function<bool(void)> update_;			// ｹﾞｽﾄとﾎｽﾄでｱｯﾌﾟﾃﾞｰﾄを分ける
	sharedMap mapObj_;
	int objectID_;
};

