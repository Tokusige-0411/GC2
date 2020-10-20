#pragma once
#include <memory>

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

	TileLoader();
	~TileLoader();
	static std::unique_ptr<TileLoader, TileLoderDeleter> s_Instance;
};

