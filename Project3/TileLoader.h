#pragma once
#include <memory>
#include <string>
#include <map>

#define lpTileLoader TileLoader::GetInstance()

struct TMXInfo
{
	int width{};					// â°œΩêî
	int height{};					// ècœΩêî
	int tileWidth{};				// ¿≤Ÿâ°ïù
	int tileHeight{};				// ¿≤Ÿècïù
};

struct TSXInfo
{
	int width{};					// â°œΩêî
	int height{};					// ècœΩêî
	int tileWidth{};				// ¿≤Ÿâ°ïù
	int tileHeight{};				// ¿≤Ÿècïù
	int chipNum{};					// ¿≤Ÿëçêî
	std::string fileName{};
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
	void SendTmxSizeData(void);
	void SendTmxData(void);
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

	// œØÃﬂä÷òA
	TMXInfo tmxInfo_{};
	TSXInfo tsxInfo_{};
	MapData mapData_;

	rapidxml::xml_document<> doc_;

	std::map<std::string, bool> version_;

	bool Init(void);
	TileLoader();
	~TileLoader();
	static std::unique_ptr<TileLoader, TileLoderDeleter> s_Instance;
};

