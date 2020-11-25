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
	Vector2 pos;
	FireDir dir;
	int animCnt;
	bool drawFlag;
	std::chrono::system_clock::time_point time;
};

class FireGenerator;

using MapData = std::map<std::string, std::vector<int>>;
using FireMap = std::vector<Fire>;

class TileLoader
{
public:
	TileLoader();
	~TileLoader();

	bool TMXLoader(std::string fileName);				// TMXﾌｧｲﾙのﾛｰﾀﾞｰ
	bool TSXLoader(std::string fileName);				// TSXﾌｧｲﾙのﾛｰﾀﾞｰ
	void SendTmxData(void);								// TMXの情報送信関数
	void Draw(void);									// ﾏｯﾌﾟ描画
	void FireUpdate(void);
	const TMXInfo& GetTmxInfo(void);					// TMX情報取得
	const TSXInfo& GetTsxInfo(void);					// TSX情報取得
	const MapData& GetMapData(void);					// ﾏｯﾌﾟﾃﾞｰﾀ取得
	int GetMapData(std::string layer, Vector2 pos);		// 指定された位置のﾏｯﾌﾟ情報を返す
	void SetFireGenerator(const Vector2& pos, int length);

private:
	// ﾏｯﾌﾟ関連
	TMXInfo tmxInfo_;						// TMX情報
	TSXInfo tsxInfo_;						// TSX情報
	MapData mapData_;						// ﾏｯﾌﾟ情報

	FireMap fireMap_;
	std::list<std::unique_ptr<FireGenerator>> fireGeneratorList_;

	rapidxml::xml_document<> doc_;			// xmlﾌｧｲﾙの親ﾉｰﾄﾞ

	std::map<std::string, bool> version_;	// ﾏｯﾌﾟﾊﾞｰｼﾞｮﾝ管理

	bool Init(void);						// 初期化
};

