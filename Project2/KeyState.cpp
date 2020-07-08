#include<Dxlib.h>
#include"SceneMng.h"
#include"KeyState.h"

KeyState::KeyState(PLAYER_NUM player)
{
	GetHitKeyStateAll(_buf);
	auto keyList = lpSceneMng.GetKeyList(player);

	_keyCon.reserve(static_cast<size_t>(end(INPUT_ID())));
	for (auto id : INPUT_ID())
	{
		_keyCon.emplace_back(keyList[id]);
	}
	return;
}

KeyState::~KeyState()
{
}

void KeyState::Update(void)
{
	SetOld();
	for (auto id : INPUT_ID())
	{
		State(id, _buf[_keyCon[static_cast<int>(id)]]);
	}
	GetHitKeyStateAll(_buf);
}
