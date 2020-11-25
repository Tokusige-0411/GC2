#pragma once
#include <chrono>
#include <Vector2.h>
#include "TileLoader.h"

class FireGenerator
{
public:
	FireGenerator(Vector2 pos, int length, FireMap& map, MapData& data);
	~FireGenerator();
	void operator()(void);

private:
	Vector2 chipPos_;
	
	int length_;
	std::map<FireDir, int> lengthMap_;

	FireMap& map_;
	MapData& mapData_;

	std::chrono::system_clock::time_point start_;
};
