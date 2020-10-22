#pragma once
#include <memory>
#include <string>
#include <map>

#define lpTileLoader TileLoader::GetInstance()

struct TMXInfo
{
	int width;					// ‘Ο½
	int height;					// cΟ½
	int tileWidth;				// ΐ²Ω‘
	int tileHeight;				// ΐ²Ωc
};

struct TSXInfo
{
	int width;					// ‘Ο½
	int height;					// cΟ½
	int tileWidth;				// ΐ²Ω‘
	int tileHeight;				// ΐ²Ωc
	int chipNum;				// ΐ²Ω
	std::string fileName;

};

using MapData = std::map<std::string, std::vector<int>>;

class TileLoader
{
public:
	static TileLoader& GetInstance()
	{
		return *s_Instance;
	}

	bool TMXLoader(void);
	bool TSXLoader(void);
	void Draw(void);
	const TMXInfo& GetTmxInfo(void);
	const TSXInfo& GetTsxInfo(void);
	const MapData& GetMapData(void);

private:
	struct TileLoderDeleter
	{
		void operator() (TileLoader* tileLoader) const
		{
			delete tileLoader;
		}
	};

	// Ο―ΜίΦA
	TMXInfo tmxInfo_;
	TSXInfo tsxInfo_;

	MapData mapData_;

	std::map<std::string, bool> version_;

	bool Init(void);
	TileLoader();
	~TileLoader();
	static std::unique_ptr<TileLoader, TileLoderDeleter> s_Instance;
};

