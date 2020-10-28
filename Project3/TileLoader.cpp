#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <list>
#include "TileLoader.h"
#include "NetWark/NetWark.h"

#include "_debug/_DebugConOut.h"

std::unique_ptr<TileLoader, TileLoader::TileLoderDeleter> TileLoader::s_Instance(new TileLoader());

bool TileLoader::TMXLoader(std::string fileName)
{
	rapidxml::file<> file(fileName.c_str());
	doc_.parse<0>(file.data());
	rapidxml::xml_node<>* map = doc_.first_node("map");

	// ﾊﾞｰｼﾞｮﾝﾁｪｯｸ
	auto version = map->first_attribute("version")->value();
	if (version_.find(version) == version_.end())
	{
		return false;
		TRACE("マップバージョン未対応\n");
	}

	// ﾏｯﾌﾟﾃﾞｰﾀ読み込み
	tmxInfo_.width = std::atoi(map->first_attribute("width")->value());
	tmxInfo_.height = std::atoi(map->first_attribute("height")->value());
	tmxInfo_.tileWidth = std::atoi(map->first_attribute("tilewidth")->value());
	tmxInfo_.tileHeight = std::atoi(map->first_attribute("tileheight")->value());

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

	rapidxml::xml_node<>* tileSet = map->first_node("tileset");
	std::string tsxData = tileSet->first_attribute("source")->value();
	std::string pass = fileName.substr(0, fileName.find_last_of("/") + 1);
	TSXLoader(pass + tsxData);

	return true;
}

bool TileLoader::TSXLoader(std::string fileName)
{
	rapidxml::file<> file(fileName.c_str());
	doc_.parse<0>(file.data());
	rapidxml::xml_node<>* tileset = doc_.first_node("tileset");

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
	std::string imageName = image->first_attribute("source")->value();
	tsxInfo_.imageName = imageName.substr(imageName.find_last_of("i"), imageName.length());

	return true;
}

void TileLoader::SendTmxSizeData(void)
{
	std::ifstream ifp("MapData.tmx");
	ifp.seekg(0, ifp.end);
	MesData sData = {MesType::TMX_Size, static_cast<int>(ifp.tellg()), 0};
	lpNetWork.SendMes(sData);
}

void TileLoader::SendTmxData(void)
{
	// 数字の部分だけ抜き取る
	std::list<int> chipData;
	for (auto& vecData : mapData_)
	{
		for (auto& data : vecData.second)
		{
			chipData.emplace_back(data);
		}
	}

	unsigned short sendCnt = 0;
	while(chipData.begin() != chipData.end())
	{
		MesData data;
		unsigned char* charData = reinterpret_cast<unsigned char*>(&data);
		unsigned short* shortData = reinterpret_cast<unsigned short*>(&data);
		charData[0] = static_cast<std::underlying_type<MesType>::type>(MesType::TMX_Data);
		for (int i = 4; i < sizeof(MesData); i++)
		{
			charData[i] = chipData.front();
			chipData.pop_front();
			if (chipData.begin() == chipData.end())
			{
				break;
			}
			charData[i] <<= 4;
			//charData[i] = chipData.front();
			//chipData.pop_front();
			//if (chipData.begin() == chipData.end())
			//{
			//	break;
			//}
		}
		shortData[1] = sendCnt;
		lpNetWork.SendMes(data);
		sendCnt++;
	}
	TRACE("送信した回数:%d\n", sendCnt);
	//int ch = 0;
	//char* string = reinterpret_cast<char*>(&ch);
	//for (int i = 0; i < sizeof(ch); i++)
	//{
	//	if (!ifp.eof())
	//	{
	//		string[i] = ifp.get();
	//	}
	//}
	//MesData sData = { MesType::TMX_Data, count, ch };
	//lpNetWork.SendMes(sData);
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
