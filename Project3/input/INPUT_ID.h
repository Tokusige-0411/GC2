#pragma once

enum class INPUT_ID
{
	UP,
	DOWN,
	RIGHT,
	LEFT,
	SET_BOMB,
	MAX
};

static INPUT_ID begin(INPUT_ID){return INPUT_ID::UP;}
static INPUT_ID end(INPUT_ID){return INPUT_ID::MAX;}
static INPUT_ID operator*(INPUT_ID id){return id;}
static INPUT_ID operator++(INPUT_ID& id){return id = static_cast<INPUT_ID>(std::underlying_type<INPUT_ID>::type(id) + 1);}