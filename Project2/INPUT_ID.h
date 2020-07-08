#pragma once

enum class INPUT_ID
{
	UP, 
	DOWN,
	RIGHT,
	LEFT,
	R_ROTA,
	L_LOTA,
	MAX
};

INPUT_ID begin(INPUT_ID) { return INPUT_ID::UP; }
INPUT_ID end(INPUT_ID) { return INPUT_ID::MAX; }
INPUT_ID operator*(INPUT_ID id) { return id; }
INPUT_ID operator++(INPUT_ID& id) { return INPUT_ID(std::underlying_type<INPUT_ID>::type(id) + 1); }