#pragma once
#include <chrono>
#include <Vector2.h>
#include "TileLoader.h"

class FireGenerator
{
public:
	FireGenerator(Vector2 pos, int length, FireMap& map, MapData& data, TMXInfo& info);
	~FireGenerator();
	void operator()(double delta);
	int GetLength(void);

private:
	int SetAnim(int offset);

	Vector2 chipPos_;
	double fireTime_;
	
	int length_;
	std::map<FireDir, int> lengthMap_;

	FireMap& Firemap_;
	MapData& mapData_;
	TMXInfo& mapInfo_;
};
