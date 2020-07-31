#pragma once
#include "Puyo.h"
class OjamaPuyo :
	public Puyo
{
public:
	OjamaPuyo();
	OjamaPuyo(Vector2&& pos, Puyo_Type id);
	~OjamaPuyo();
};

