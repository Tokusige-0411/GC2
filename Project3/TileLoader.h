#pragma once
#include <memory>

class TileLoader
{
public:
	TileLoader();
	~TileLoader();

private:
	static struct TileLoderDereter
	{

	};

	std::unique_ptr<TileLoader, TileLoaderDereter>
};

