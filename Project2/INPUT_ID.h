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

INPUT_ID begin(INPUT_ID);
INPUT_ID end(INPUT_ID);
INPUT_ID operator*(INPUT_ID id);
INPUT_ID operator++(INPUT_ID& id);