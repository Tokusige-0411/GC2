#pragma once
#include <memory>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <chrono>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <Vector2.h>

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

enum class BlockType
{
	Non = 1,
	Floor_Blue,
	Floor_Red,
	Char,
	Black,
	Wall_Blue,
	Wall_Red,
	Break_Block,
	Item_1,
	Item_2,
	Item_3,
	Item_4,
	Max
};

struct ClossFire
{
	int up : 1;
	int down : 1;
	int right : 1;
	int left : 1;
};

enum class FireDir
{
	Right,
	Down,
	Left,
	Up,
	Max
};

struct Fire
{
	Vector2 pos{};
	FireDir dir{};
	double animCnt{};
	int length{};
};

class FireGenerator;

using MapData = std::map<std::string, std::vector<int>>;
using FireMap = std::vector<Fire>;

class TileLoader
{
public:
	TileLoader();
	~TileLoader();

	bool TMXLoader(std::string fileName);				// TMXÌ§²Ù‚ÌÛ°ÀŞ°
	bool TSXLoader(std::string fileName);				// TSXÌ§²Ù‚ÌÛ°ÀŞ°
	void SendTmxData(void);								// TMX‚Ìî•ñ‘—MŠÖ”
	void Draw(void);									// Ï¯Ìß•`‰æ
	void FireUpdate(double delta);
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
	double delta_;

	FireMap fireMap_;
	std::list<std::unique_ptr<FireGenerator>> fireGeneratorList_;

	rapidxml::xml_document<> doc_;			// xmlÌ§²Ù‚ÌeÉ°ÄŞ

	std::map<std::string, bool> version_;	// Ï¯ÌßÊŞ°¼Ş®İŠÇ—

	bool Init(void);						// ‰Šú‰»
};

