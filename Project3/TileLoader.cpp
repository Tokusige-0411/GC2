#include <fstream>
#include "TileLoader.h"

#include "_debug/_DebugConOut.h"

std::unique_ptr<TileLoader, TileLoader::TileLoderDeleter> TileLoader::s_Instance(new TileLoader());

bool TileLoader::Init(void)
{
	return true;
}

TileLoader::TileLoader()
{
	Init();
}

TileLoader::~TileLoader()
{
}
