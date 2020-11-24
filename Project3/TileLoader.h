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

	bool TMXLoader(std::string fileName);				// TMXﾌｧｲﾙのﾛｰﾀﾞｰ
	bool TSXLoader(std::string fileName);				// TSXﾌｧｲﾙのﾛｰﾀﾞｰ
	void SendTmxData(void);								// TMXの情報送信関数
	void Draw(void);									// ﾏｯﾌﾟ描画
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

	std::vector<Fire> fireMap_;
	std::list<FireGenerator> fireGeneratorList_;

	rapidxml::xml_document<> doc_;			// xmlﾌｧｲﾙの親ﾉｰﾄﾞ

	std::map<std::string, bool> version_;	// ﾏｯﾌﾟﾊﾞｰｼﾞｮﾝ管理

	bool Init(void);						// 初期化
};

