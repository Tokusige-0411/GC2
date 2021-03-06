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
	int width{};					// 横ﾏｽ数
	int height{};					// 縦ﾏｽ数
	int tileWidth{};				// ﾀｲﾙ横幅
	int tileHeight{};				// ﾀｲﾙ縦幅
};

struct TSXInfo
{
	int width{};					// 横ﾏｽ数
	int height{};					// 縦ﾏｽ数
	int tileWidth{};				// ﾀｲﾙ横幅
	int tileHeight{};				// ﾀｲﾙ縦幅
	int chipNum{};					// ﾀｲﾙ総数
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
	int up;
	int down;
	int right;
	int left;
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
	ClossFire fire{};
	double animCnt{};
	int length{};
};

class FireGenerator;

using MapData = std::map<std::string, std::vector<int>>;
using FireMap = std::vector<Fire>;
using BombMap = std::vector<bool>;

class TileLoader
{
public:
	TileLoader();
	~TileLoader();

	bool TMXLoader(std::string fileName);						// TMXﾌｧｲﾙのﾛｰﾀﾞｰ
	bool TSXLoader(std::string fileName);						// TSXﾌｧｲﾙのﾛｰﾀﾞｰ
	void SendTmxData(void);										// TMXの情報送信関数
	void Draw(void);											// ﾏｯﾌﾟ描画
	void FireUpdate(double delta);								// 爆風更新
	const TMXInfo& GetTmxInfo(void);							// TMX情報取得
	const TSXInfo& GetTsxInfo(void);							// TSX情報取得
	const MapData& GetMapData(void);							// ﾏｯﾌﾟﾃﾞｰﾀ取得
	int GetMapData(std::string layer, Vector2 pos);				// 指定された位置のﾏｯﾌﾟ情報を返す
	double GetFireMap(Vector2 pos);								// 指定場所に爆風があるか判断する
	void SetFireGenerator(const Vector2& pos, int length);
	void SetBombMap(const Vector2& pos, bool flag);
	bool GetBombMap(const Vector2& pos);

private:
	// ﾏｯﾌﾟ関連
	TMXInfo tmxInfo_;						// TMX情報
	TSXInfo tsxInfo_;						// TSX情報
	MapData mapData_;						// ﾏｯﾌﾟ情報
	double delta_;							// 時間間隔

	FireMap fireMap_;						// 爆風ﾏｯﾌﾟ
	BombMap bombMap_;						// ﾎﾞﾑ位置ﾏｯﾌﾟ

	std::list<std::unique_ptr<FireGenerator>> fireGeneratorList_;

	rapidxml::xml_document<> doc_;			// xmlﾌｧｲﾙの親ﾉｰﾄﾞ

	std::map<std::string, bool> version_;	// ﾏｯﾌﾟﾊﾞｰｼﾞｮﾝ管理

	bool Init(void);						// 初期化
};

