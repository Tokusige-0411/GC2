#include "FireGenerator.h"
#include "Scene/SceneMng.h"

void FireGenerator::operator()(void)
{
	auto now = lpSceneMng.GetTime();
	if (length)
	{
		map[chipPos.y * 21 + chipPos.x].time = now;
		map[chipPos.y * 21 + chipPos.x].drawFlag = true;
		length--;
	}
}
