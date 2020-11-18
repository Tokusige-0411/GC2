#pragma once
#include <map>
#include <mutex>
#include <Vector2.h>
#include "../TileLoader.h"
#include "../NetWark/NetWark.h"

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

struct DirBit
{
	unsigned int up : 1;
	unsigned int right : 1;
	unsigned int down : 1;
	unsigned int left : 1;
};

union DirPermit
{
	DirBit bit;
	unsigned int perbit;
};

class Object
{
public:
	Object();
	Object(int id, Vector2 pos);
	virtual ~Object();
	virtual bool Update(MapData& mapData);
	virtual void Draw(void);
	Vector2 GetPos(void);
	bool IsPickUp(void);
	MesPacket PickUp(void);

private:
	virtual void Init(void);

protected:
	Vector2 pos_;
	Dir dir_;
	int animCnt_;

	std::vector<MesPacket> mesList_;
	std::mutex mtx_;
};

