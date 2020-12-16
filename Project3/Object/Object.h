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
	time_point startTime_;						// ������������

protected:
	Vector2 pos_;								// ���W
	Dir dir_;									// ����
	bool alive_;								// �����Ă邩				
	bool deth_;									// �A�j���[�V�����I�����Ă邩
	bool lost_;									// �l�b�g���[�N�ؒf������
	int animCnt_;								// �A�j���[�V��������
	std::map<AnimState, AnimVector> animMap_;	// state�ŃA�j���[�V�����𕪂���	

	MesPacketList mesList_;						// ���󂯎���
	std::mutex mtx_;							// �eobject��mutex

	std::function<bool(void)> update_;			// �޽Ă�νĂű����ްĂ𕪂���
	sharedMap mapObj_;
	int objectID_;
};

