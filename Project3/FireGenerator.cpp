#include "FireGenerator.h"
#include "Scene/SceneMng.h"

FireGenerator::FireGenerator(Vector2 pos, int length, FireMap& map, MapData& data, TMXInfo& info):Firemap_(map), mapData_(data), mapInfo_(info)
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
			int num = (chipPos_.y - offset) * mapInfo_.width + chipPos_.x;
			auto SetFireMapUp = [&](int a) {
				Firemap_[num].time = end;
				Firemap_[num].drawFlag = true;
				Firemap_[num].animCnt = 0;
				Firemap_[num].length = SetAnim(a);
				Firemap_[num].pos = { chipPos_.x * mapInfo_.tileWidth, (chipPos_.y - offset) * mapInfo_.tileHeight };
				Firemap_[num].dir = FireDir::Up; 
			};
			if (mapData_["Obj"][num] == static_cast<int>(BlockType::Wall_Blue) ||
				mapData_["Obj"][num] == static_cast<int>(BlockType::Wall_Red))
			{
				lengthMap_[FireDir::Up] = 0;
			}
			else if (mapData_["Obj"][num] == static_cast<int>(BlockType::Break_Block))
			{
				lengthMap_[FireDir::Up] = 1;
				mapData_["Obj"][num] = 0;
				SetFireMapUp(length_ - 1);
				lengthMap_[FireDir::Up] = 0;
			}
			else
			{
				SetFireMapUp(offset);
				lengthMap_[FireDir::Up]--;
			}
		}

		if (lengthMap_[FireDir::Down])
		{
			int offset = abs(length_ - lengthMap_[FireDir::Down]);
			int num = (chipPos_.y + offset) * mapInfo_.width + chipPos_.x;
			auto SetFireMapUp = [&](int a) {
				Firemap_[num].time = end;
				Firemap_[num].drawFlag = true;
				Firemap_[num].animCnt = 0;
				Firemap_[num].length = SetAnim(a);
				Firemap_[num].pos = { chipPos_.x * mapInfo_.tileWidth, (chipPos_.y + offset) * mapInfo_.tileHeight };
				Firemap_[num].dir = FireDir::Down;
			};
			if (mapData_["Obj"][num] == static_cast<int>(BlockType::Wall_Blue) ||
				mapData_["Obj"][num] == static_cast<int>(BlockType::Wall_Red))
			{
				lengthMap_[FireDir::Down] = 0;
			}
			else if (mapData_["Obj"][num] == static_cast<int>(BlockType::Break_Block))
			{
				mapData_["Obj"][num] = 0;
				SetFireMapUp(length_ - 1);
				lengthMap_[FireDir::Down] = 0;
			}
			else
			{
				SetFireMapUp(offset);
				lengthMap_[FireDir::Down]--;
			}
		}

		if (lengthMap_[FireDir::Right])
		{
			int offset = abs(length_ - lengthMap_[FireDir::Right]);
			int num = chipPos_.y * mapInfo_.width + (chipPos_.x + offset);
			auto SetFireMapUp = [&](int a) {
				Firemap_[num].time = end;
				Firemap_[num].drawFlag = true;
				Firemap_[num].animCnt = 0;
				Firemap_[num].length = SetAnim(a);
				Firemap_[num].pos = { (chipPos_.x + offset)* mapInfo_.tileWidth, chipPos_.y * mapInfo_.tileHeight };
				Firemap_[num].dir = FireDir::Right;
			};
			if (mapData_["Obj"][num] == static_cast<int>(BlockType::Wall_Blue) ||
				mapData_["Obj"][num] == static_cast<int>(BlockType::Wall_Red))
			{
				lengthMap_[FireDir::Right] = 0;
			}
			else if (mapData_["Obj"][num] == static_cast<int>(BlockType::Break_Block))
			{
				mapData_["Obj"][num] = 0;
				SetFireMapUp(length_ - 1);
				lengthMap_[FireDir::Right] = 0;
			}
			else
			{
				SetFireMapUp(offset);
				lengthMap_[FireDir::Right]--;
			}
		}

		if (lengthMap_[FireDir::Left])
		{
			int offset = abs(length_ - lengthMap_[FireDir::Left]);
			int num = chipPos_.y * mapInfo_.width + (chipPos_.x - offset);
			auto SetFireMapUp = [&](int a) {
				Firemap_[num].time = end;
				Firemap_[num].drawFlag = true;
				Firemap_[num].animCnt = 0;
				Firemap_[num].length = SetAnim(a);
				Firemap_[num].pos = { (chipPos_.x - offset) * mapInfo_.tileWidth, chipPos_.y * mapInfo_.tileHeight };
				Firemap_[num].dir = FireDir::Left;
			};
			if (mapData_["Obj"][num] == static_cast<int>(BlockType::Wall_Blue) ||
				mapData_["Obj"][num] == static_cast<int>(BlockType::Wall_Red))
			{
				lengthMap_[FireDir::Left] = 0;
			}
			else if (mapData_["Obj"][num] == static_cast<int>(BlockType::Break_Block))
			{
				mapData_["Obj"][num] = 0;
				SetFireMapUp(length_ - 1);
				lengthMap_[FireDir::Left] = 0;
			}
			else
			{
				SetFireMapUp(offset);
				lengthMap_[FireDir::Left]--;
			}
		}
		start_ = end;
	}
}

int FireGenerator::GetLength(void)
{
	int count = 0;
	for(auto data : lengthMap_)
	{
		count += data.second;
	}
	return count;
}

int FireGenerator::SetAnim(int offset)
{
	if (offset == 0)
	{
		return offset;
	}
	else if(offset == (length_ - 1))
	{
		return 2;
	}
	return 1;
}
