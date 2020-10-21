#include <fstream>
#include <iostream>
#include <sstream>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include "TileLoader.h"

#include "_debug/_DebugConOut.h"

std::unique_ptr<TileLoader, TileLoader::TileLoderDeleter> TileLoader::s_Instance(new TileLoader());

bool TileLoader::Init(void)
{
	rapidxml::xml_document<> doc;
	rapidxml::file<> file("MapData.tmx");
	doc.parse<0>(file.data());

	//rapidxml::xml_node<>* node = doc.first_node("map");
	//for (rapidxml::xml_attribute<>* tmp = node->first_attribute(); tmp != nullptr; tmp = tmp->next_attribute())
	//{
	//	std::cout << tmp->value();
	//}
	rapidxml::xml_node<>* map = doc.first_node("map");

	width_ = std::atoi(map->first_attribute("width")->value());
	height_ = atoi(map->first_attribute("height")->value());
	tileWidth_ = atoi(map->first_attribute("tilewidth")->value());
	tileHeight_ = atoi(map->first_attribute("tileheight")->value());

	for (rapidxml::xml_node<>* layer = map->first_node("layer");
		layer != nullptr;
		layer = layer->next_sibling())
	{
		int layerID = std::atoi(layer->first_attribute("id")->value());
		mapData_.try_emplace(static_cast<TileLayer>(layerID - 1));
		rapidxml::xml_node<>* data = layer->first_node("data");
		std::istringstream iss(data->value());
		std::string str;
		while (std::getline(iss, str, ','))
		{
			mapData_[static_cast<TileLayer>(layerID)].push_back(std::atoi(str.c_str()));
		}
	}
	return true;
}

TileLoader::TileLoader()
{
	Init();
}

TileLoader::~TileLoader()
{
}
