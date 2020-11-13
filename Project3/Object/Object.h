#pragma once
#include <map>
#include <Vector2.h>
#include "../TileLoader.h"

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
	virtual ~Object();
	virtual void Update(MapData& mapData);
	virtual void Draw(void);
	Vector2 GetPos(void);

private:
	virtual void Init(void);

protected:
	Vector2 pos_;
	Dir dir_;
	int animCnt_;
};

