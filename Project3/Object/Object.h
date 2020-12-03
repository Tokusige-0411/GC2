#pragma once
#include <map>
#include <mutex>
#include <Vector2.h>
#include "../TileLoader.h"
#include "../NetWark/NetWark.h"
#include "../Scene/BaseScene.h"

enum class AnimState
{
	Idle,
	Walk,
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
static Dir end(Dir) { return Dir::Deth; }
static Dir operator*(Dir id) { return id; }
static Dir operator++(Dir& id) { return id = static_cast<Dir>(std::underlying_type<Dir>::type(id) + 1); }

using sharedMap = std::shared_ptr<TileLoader>;

class Object
{
public:
	Object();
	Object(int id, Vector2 pos);
	virtual ~Object();
	virtual bool Update(void);
	virtual bool UpdateDef(void);
	virtual void Draw(void);
	Vector2 GetPos(void);
	bool IsPickUp(void);
	MesPair PickUp(void);
	bool GetAlive(void) { return alive_; };
	bool GetDeth(void) { return deth_; };
	int GetObjectID(void) { return objectID_; };

private:
	virtual void Init(void);

protected:
	Vector2 pos_;							// 座標
	Dir dir_;								// 方向
	bool alive_;							// 生きてるか				
	bool deth_;								// アニメーション終了してるか
	int animCnt_;							// アニメーションｶｳﾝﾄ

	MesPacketList mesList_;					// 情報受け取り口
	std::mutex mtx_;

	std::function<bool(void)> update_;					// ｹﾞｽﾄとﾎｽﾄでｱｯﾌﾟﾃﾞｰﾄを分ける
	sharedMap mapObj_;
	int objectID_;
};

