#include"_debug/_DebugConOut.h"
#include "InputState.h"

InputState::InputState()
{
	// ·°‚Ì‰Šú‰»
	for (auto id : INPUT_ID())
	{
		_state.try_emplace(id, KeyPair{ 0, 1 });
	}
}

InputState::~InputState()
{
}

void InputState::SetOld(void)
{
	for (auto key : INPUT_ID())
	{
		try
		{
			_state[key].second = _state[key].first;
		}
		catch (...)
		{
			AST();
			_state.emplace(key, KeyPair{ 0, 1 });
		}
	}
}

const KeyMap& InputState::State(void) const
{
	return _state;
}

const KeyPair& InputState::State(INPUT_ID id) const
{
	static KeyPair defData = { 0, 0 };
	try
	{
		return (_state.at(id));
	}
	catch (...)
	{
		AST();
		return defData;
	}
}

bool InputState::State(INPUT_ID id, int data)
{
	if (_state.find(id) != _state.end())
	{
		_state[id].first = data;
		return true;
	}
	return false;
}
