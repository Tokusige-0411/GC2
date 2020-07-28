#pragma once
#include "../Field.h"

struct MunyonMode
{
	void operator()(Field& field) {
		field.fieldState_ = FieldState::Rensa;
	}
};