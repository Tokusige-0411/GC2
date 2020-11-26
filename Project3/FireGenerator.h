#pragma once
#include <chrono>
#include <Vector2.h>
#include "TileLoader.h"

class FireGenerator
{
public:
	FireGenerator(Vector2 pos, int length, FireMap& map, MapData& data, TMXInfo& info);
	~FireGenerator();
	void operator()(void);
	int GetLength(void);

private:
	int SetAnim(int offset);

	Vector2 chipPos_;
	
	int length_;
	std::map<FireDir, int> lengthMap_;

	FireMap& Firemap_;
	MapData& mapData_;
	TMXInfo& mapInfo_;

	std::chrono::system_clock::time_point start_;
};
