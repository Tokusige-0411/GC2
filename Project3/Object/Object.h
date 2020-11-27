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
	Max
};

enum class Dir
{
	Up,
	Right,
	Down,
	Left,
	Max
};

static Dir begin(Dir) { return Dir::Up; }
static Dir end(Dir) { return Dir::Max; }
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
	int GetObjectID(void) { return objectID_; };

private:
	virtual void Init(void);

protected:
	Vector2 pos_;
	Dir dir_;
	bool alive_;
	int animCnt_;

	MesPacketList mesList_;
	std::mutex mtx_;

	std::function<bool(void)> update_;					// ¹Þ½Ä‚ÆÎ½Ä‚Å±¯ÌßÃÞ°Ä‚ð•ª‚¯‚é
	sharedMap mapObj_;
	int objectID_;
};

