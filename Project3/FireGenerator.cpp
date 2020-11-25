#include "FireGenerator.h"
#include "Scene/SceneMng.h"

FireGenerator::FireGenerator(Vector2 pos, int length, FireMap& map, MapData& data):map_(map), mapData_(data)
{
	chipPos_ = pos;
	length_ = length;
	lengthMap_.emplace(FireDir::Up, length);
	lengthMap_.emplace(FireDir::Down, length);
	lengthMap_.emplace(FireDir::Right, length);
	lengthMap_.emplace(FireDir::Left, length);
	start_ = lpSceneMng.GetTime();
}

FireGenerator::~FireGenerator()
{
}

void FireGenerator::operator()(void)
{
	auto end = lpSceneMng.GetTime();
	if (std::chrono::duration_cast<std::chrono::milliseconds>(end - start_).count() >= 150)
	{
		if (lengthMap_[FireDir::Up])
		{
			int offset = abs(length_ - lengthMap_[FireDir::Up]);
			if (mapData_["Obj"][(chipPos_.y - offset) * 21 + chipPos_.x] == 0)
			{
				map_[(chipPos_.y - offset) * 21 + chipPos_.x].time = end;
				map_[(chipPos_.y - offset) * 21 + chipPos_.x].drawFlag = true;
				map_[(chipPos_.y - offset) * 21 + chipPos_.x].pos = { chipPos_.x * 32, (chipPos_.y - offset) * 32 };
				lengthMap_[FireDir::Up]--;
			}
			else
			{
				lengthMap_[FireDir::Up] = 0;
			}
		}

		if (lengthMap_[FireDir::Down])
		{
			int offset = abs(length_ - lengthMap_[FireDir::Down]);
			if (mapData_["Obj"][(chipPos_.y + offset) * 21 + chipPos_.x] == 0)
			{
				map_[(chipPos_.y + offset) * 21 + chipPos_.x].time = end;
				map_[(chipPos_.y + offset) * 21 + chipPos_.x].drawFlag = true;
				map_[(chipPos_.y + offset) * 21 + chipPos_.x].pos = { chipPos_.x * 32, (chipPos_.y + offset) * 32 };
				lengthMap_[FireDir::Down]--;
			}
			else
			{
				lengthMap_[FireDir::Down] = 0;
			}
		}

		if (lengthMap_[FireDir::Right])
		{
			int offset = abs(length_ - lengthMap_[FireDir::Right]);
			if (mapData_["Obj"][chipPos_.y * 21 + (chipPos_.x + offset)] == 0)
			{
				map_[chipPos_.y * 21 + (chipPos_.x + offset)].time = end;
				map_[chipPos_.y * 21 + (chipPos_.x + offset)].drawFlag = true;
				map_[chipPos_.y * 21 + (chipPos_.x + offset)].pos = { (chipPos_.x + offset) * 32, chipPos_.y * 32 };
				lengthMap_[FireDir::Right]--;
			}
			else
			{
				lengthMap_[FireDir::Right] = 0;
			}
		}

		if (lengthMap_[FireDir::Left])
		{
			int offset = abs(length_ - lengthMap_[FireDir::Left]);
			if (mapData_["Obj"][chipPos_.y * 21 + (chipPos_.x - offset)] == 0)
			{
				map_[chipPos_.y * 21 + (chipPos_.x - offset)].time = end;
				map_[chipPos_.y * 21 + (chipPos_.x - offset)].drawFlag = true;
				map_[chipPos_.y * 21 + (chipPos_.x - offset)].pos = { (chipPos_.x - offset) * 32, chipPos_.y * 32 };
				lengthMap_[FireDir::Left]--;
			}
			else
			{
				lengthMap_[FireDir::Left] = 0;
			}
		}
		start_ = end;
	}
}
