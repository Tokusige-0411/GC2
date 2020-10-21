#pragma once
#include <memory>
#include <string>

#define lpTileLoader TileLoader::GetInstance()

class TileLoader
{
public:
	static TileLoader& GetInstance()
	{
		return *s_Instance;
	}

private:
	struct TileLoderDeleter
	{
		void operator() (TileLoader* tileLoader) const
		{
			delete tileLoader;
		}
	};

	bool Init(void);

	TileLoader();
	~TileLoader();
	static std::unique_ptr<TileLoader, TileLoderDeleter> s_Instance;
};

