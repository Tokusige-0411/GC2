#include<Dxlib.h>
#include"Field.h"

Field::Field()
{
}

Field::Field(Vector2 offset)
{
	_offset = offset;
	_screenID = 0;
}

Field::~Field()
{
}

void Field::Draw()
{
	DrawBox(_offset.x, _offset.y, 6 * 40 + 1, 12 * 40 + 1, 0xffffff, true);
}
