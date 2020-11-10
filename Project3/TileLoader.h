#pragma once
#include <memory>
#include <string>
#include <map>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

#define lpTileLoader TileLoader::GetInstance()

struct TMXInfo
{
	int width{};					// ‰¡Ï½”
	int height{};					// cÏ½”
	int tileWidth{};				// À²Ù‰¡•
	int tileHeight{};				// À²Ùc•
};

struct TSXInfo
{
	int width{};					// ‰¡Ï½”
	int height{};					// cÏ½”
	int tileWidth{};				// À²Ù‰¡•
	int tileHeight{};				// À²Ùc•
	int chipNum{};					// À²Ù‘”
	std::string imageName{};
};

using MapData = std::map<std::string, std::vector<int>>;

class TileLoader
{
public:
	static TileLoader& GetInstance()
	{
		return *s_Instance;
	}

	bool TMXLoader(std::string fileName);		// TMXÌ§²Ù‚ÌÛ°ÀŞ°
	bool TSXLoader(std::string fileName);		// TSXÌ§²Ù‚ÌÛ°ÀŞ°
	void SendTmxData(void);						// TMX‚Ìî•ñ‘—MŠÖ”
	void Draw(void);							// Ï¯Ìß•`‰æ
	const TMXInfo& GetTmxInfo(void);			// TMXî•ñæ“¾
	const TSXInfo& GetTsxInfo(void);			// TSXî•ñæ“¾
	const MapData& GetMapData(void);			// Ï¯ÌßÃŞ°Àæ“¾

private:
	struct TileLoderDeleter
	{
		void operator() (TileLoader* tileLoader) const
		{
			delete tileLoader;
		}
	};

	// Ï¯ÌßŠÖ˜A
	TMXInfo tmxInfo_;						// TMXî•ñ
	TSXInfo tsxInfo_;						// TSXî•ñ
	MapData mapData_;						// Ï¯Ìßî•ñ

	rapidxml::xml_document<> doc_;			// xmlÌ§²Ù‚ÌeÉ°ÄŞ

	std::map<std::string, bool> version_;	// Ï¯ÌßÊŞ°¼Ş®İŠÇ—

	bool Init(void);						// ‰Šú‰»
	TileLoader();
	~TileLoader();
	static std::unique_ptr<TileLoader, TileLoderDeleter> s_Instance;
};

