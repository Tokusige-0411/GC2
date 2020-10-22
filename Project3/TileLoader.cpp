#include <fstream>
#include <iostream>
#include <sstream>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include "TileLoader.h"

#include "_debug/_DebugConOut.h"

std::unique_ptr<TileLoader, TileLoader::TileLoderDeleter> TileLoader::s_Instance(new TileLoader());

bool TileLoader::TMXLoader(void)
{
	rapidxml::xml_document<> doc;
	rapidxml::file<> file("MapData.tmx");
	doc.parse<0>(file.data());
	rapidxml::xml_node<>* map = doc.first_node("map");

	// ﾊﾞｰｼﾞｮﾝﾁｪｯｸ
	auto version = map->first_attribute("version")->value();
	if (version_.find(version) == version_.end())
	{
		return false;
		TRACE("マップバージョン未対応\n");
	}

	// ﾏｯﾌﾟﾃﾞｰﾀ読み込み
	tmxInfo_.width = std::atoi(map->first_attribute("width")->value());
	tmxInfo_.height = atoi(map->first_attribute("height")->value());
	tmxInfo_.tileWidth = atoi(map->first_attribute("tilewidth")->value());
	tmxInfo_.tileHeight = atoi(map->first_attribute("tileheight")->value());

	for (rapidxml::xml_node<>* layer = map->first_node("layer");
		layer != nullptr;
		layer = layer->next_sibling())
	{
		std::string name = layer->first_attribute("name")->value();
		rapidxml::xml_node<>* data = layer->first_node("data");
		std::istringstream iss(data->value());
		std::string str;
		while (std::getline(iss, str, ','))
		{
			mapData_[name].emplace_back(std::atoi(str.c_str()));
		}
	}
	return true;
}

bool TileLoader::TSXLoader(void)
{
	rapidxml::xml_document<> doc;
	rapidxml::file<> file("MapTile.tsx");
	doc.parse<0>(file.data());
	rapidxml::xml_node<>* tileset = doc.first_node("tileset");

	// ﾊﾞｰｼﾞｮﾝﾁｪｯｸ
	auto version = tileset->first_attribute("version")->value();
	if (version_.find(version) == version_.end())
	{
		return false;
		TRACE("マップバージョン未対応\n");
	}

	// ﾏｯﾌﾟﾁｯﾌﾟの情報取得
	tsxInfo_.tileWidth = std::atoi(tileset->first_attribute("tilewidth")->value());
	tsxInfo_.tileHeight = std::atoi(tileset->first_attribute("tileheight")->value());
	tsxInfo_.width = std::atoi(tileset->first_attribute("columns")->value());
	tsxInfo_.height = std::atoi(tileset->first_attribute("tilecount")->value()) / tsxInfo_.width;
	tsxInfo_.chipNum = std::atoi(tileset->first_attribute("tilecount")->value());

	// ﾌｧｲﾙの場所取得
	rapidxml::xml_node<>* image = tileset->first_node("image");
	tsxInfo_.fileName = image->first_attribute("source")->value();

	return true;
}

void TileLoader::Draw(void)
{
}

const TMXInfo& TileLoader::GetTmxInfo(void)
{
	return tmxInfo_;
}

const TSXInfo& TileLoader::GetTsxInfo(void)
{
	return tsxInfo_;
}

const MapData& TileLoader::GetMapData(void)
{
	return mapData_;
}

bool TileLoader::Init(void)
{
	version_["1.2"] = true;
	version_["1.4"] = true;
	return true;
}

TileLoader::TileLoader()
{
	Init();
}

TileLoader::~TileLoader()
{
}
