#include <DxLib.h>
#include "MouseCtl.h"

MouseCtl::MouseCtl()
{
	mouseData = 0;
	mouseDataOld = 0;
}

MouseCtl::~MouseCtl()
{
}

bool MouseCtl::GetClickTrg(void)
{
	//return (mouseData & MOUSE_INPUT_LEFT) && !(mouseDataOld & MOUSE_INPUT_LEFT);
	return (mouseData == MOUSE_INPUT_LEFT) && !(mouseDataOld == MOUSE_INPUT_LEFT);
	//return (mouseData && !mouseDataOld);
}

bool MouseCtl::GetClicking(void)
{
	//return (mouseData & MOUSE_INPUT_LEFT) && (mouseDataOld & MOUSE_INPUT_LEFT);
	return (mouseData == MOUSE_INPUT_LEFT) && (mouseDataOld == MOUSE_INPUT_LEFT);
	//return (mouseData && mouseDataOld);
}

const Vector2& MouseCtl::GetPos(void)
{
	return _pos;
}

void MouseCtl::Update(void)
{
	// œ≥ΩÇÃç¿ïWéÊìæ
	GetMousePoint(&_pos.x, &_pos.y);

	// œ≥ΩÇÃ∏ÿØ∏èÛë‘éÊìæ
	mouseDataOld = mouseData;
	mouseData = GetMouseInput();
}
