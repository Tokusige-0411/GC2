#pragma once
#include <memory>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <Vector2.h>

struct FireGenerator;

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

struct Fire
{
	Dir dir;
};

using MapData = std::map<std::string, std::vector<int>>;

class TileLoader
{
public:
	TileLoader();
	~TileLoader();

	bool TMXLoader(std::string fileName);				// TMXÌ§²Ù‚ÌÛ°ÀŞ°
	bool TSXLoader(std::string fileName);				// TSXÌ§²Ù‚ÌÛ°ÀŞ°
	void SendTmxData(void);								// TMX‚Ìî•ñ‘—MŠÖ”
	void Draw(void);									// Ï¯Ìß•`‰æ
	const TMXInfo& GetTmxInfo(void);					// TMXî•ñæ“¾
	const TSXInfo& GetTsxInfo(void);					// TSXî•ñæ“¾
	const MapData& GetMapData(void);					// Ï¯ÌßÃŞ°Àæ“¾
	int GetMapData(std::string layer, Vector2 pos);		// w’è‚³‚ê‚½ˆÊ’u‚ÌÏ¯Ìßî•ñ‚ğ•Ô‚·
	void SetFireGenerator(const Vector2& pos, int length);

private:
	// Ï¯ÌßŠÖ˜A
	TMXInfo tmxInfo_;						// TMXî•ñ
	TSXInfo tsxInfo_;						// TSXî•ñ
	MapData mapData_;						// Ï¯Ìßî•ñ

	std::vector<Fire> fireMap_;
	std::list<FireGenerator> fireGeneratorList_;

	rapidxml::xml_document<> doc_;			// xmlÌ§²Ù‚ÌeÉ°ÄŞ

	std::map<std::string, bool> version_;	// Ï¯ÌßÊŞ°¼Ş®İŠÇ—

	bool Init(void);						// ‰Šú‰»
};

