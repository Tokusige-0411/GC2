#include <fstream>
#include "TileLoader.h"

#include "_debug/_DebugConOut.h"

std::unique_ptr<TileLoader, TileLoader::TileLoderDeleter> TileLoader::s_Instance(new TileLoader());

TileLoader::TileLoader()
{
	std::ifstream ifs("MapData.tmx");
	std::string tmp;
	ifs >> tmp;
}

TileLoader::~TileLoader()
{
}
