#pragma once
#include <memory>
#include <string>
#include <map>

#define lpTileLoader TileLoader::GetInstance()

enum class TileLayer
{
	Char,
	Obj,
	Item,
	Bg
};

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

	// Ο―ΜίΦA
	int width_;					// ‘Ο½
	int height_;				// cΟ½
	int tileWidth_;				// ΐ²Ω‘
	int tileHeight_;			// ΐ²Ωc

	std::map<TileLayer, std::vector<int>> mapData_;

	bool Init(void);
	TileLoader();
	~TileLoader();
	static std::unique_ptr<TileLoader, TileLoderDeleter> s_Instance;
};

