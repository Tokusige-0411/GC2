#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <list>
#include "TileLoader.h"
#include "NetWark/NetWark.h"
#include <imageMng.h>
#include "FireGenerator.h"

#include "_debug/_DebugConOut.h"

bool TileLoader::TMXLoader(std::string fileName)
{
	rapidxml::file<> file(fileName.c_str());
	doc_.parse<0>(file.data());
	rapidxml::xml_node<>* map = doc_.first_node("map");

	// �ް�ޮ�����
	auto version = map->first_attribute("version")->value();
	if (version_.find(version) == version_.end())
	{
		return false;
		TRACE("�}�b�v�o�[�W�������Ή�\n");
	}

	// ϯ���ް��ǂݍ���
	tmxInfo_.width = std::atoi(map->first_attribute("width")->value());
	tmxInfo_.height = std::atoi(map->first_attribute("height")->value());
	tmxInfo_.tileWidth = std::atoi(map->first_attribute("tilewidth")->value());
	tmxInfo_.tileHeight = std::atoi(map->first_attribute("tileheight")->value());
	fireMap_.resize(tmxInfo_.width * tmxInfo_.height);

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

	// �ް�ޮ�����
	auto version = tileset->first_attribute("version")->value();
	if (version_.find(version) == version_.end())
	{
		return false;
		TRACE("�}�b�v�o�[�W�������Ή�\n");
	}

	// ϯ�����߂̏��擾
	tsxInfo_.tileWidth = std::atoi(tileset->first_attribute("tilewidth")->value());
	tsxInfo_.tileHeight = std::atoi(tileset->first_attribute("tileheight")->value());
	tsxInfo_.width = std::atoi(tileset->first_attribute("columns")->value());
	tsxInfo_.height = std::atoi(tileset->first_attribute("tilecount")->value()) / tsxInfo_.width;
	tsxInfo_.chipNum = std::atoi(tileset->first_attribute("tilecount")->value());

	// ̧�ق̏ꏊ�擾
	rapidxml::xml_node<>* image = tileset->first_node("image");
	std::string imageName = image->first_attribute("source")->value();
	tsxInfo_.imageName = imageName.substr(imageName.find_last_of("i"), imageName.length());

	return true;
}

void TileLoader::SendTmxData(void)
{
	// ̧�ّ���ł�����@
	//std::ifstream ifp("MapData.tmx");
	std::ifstream ifp("TestMap.tmx");
	std::string str;
	MesPacket csvData;
	unionData unionData = { 0 };
	int layerCnt = 0;
	int width = 0;
	int height = 0;

	int charCnt = 0;
	while (!ifp.eof())
	{
		// csv�̂Ƃ���܂ōs���΂�
		while (str.find("data encoding") == std::string::npos)
		{
			std::getline(ifp, str);
			if (str.find("map version") != std::string::npos)
			{

			}
			// ڲԐ�����
			if (str.find("/layer") != std::string::npos)
			{
				layerCnt++;
			}
			if (ifp.eof())
			{
				break;
			}
		}

		// csv�̕�����ǂݍ���
		if (!ifp.eof())
		{
			while (std::getline(ifp, str))
			{
				if (str.find("/data") != std::string::npos)
				{
					break;
				}

				std::istringstream iss(str);
				std::string csvStr;
				while (std::getline(iss, csvStr, ','))
				{
					// �s���琔�������o����
					auto csvNum = std::atoi(csvStr.c_str());
					unionData.cData[(charCnt % 8) / 2] |= (csvNum << 4 * (charCnt % 2));
					if ((charCnt % 8) == 7)
					{
						csvData.emplace_back(unionData);
						unionData = { 0 };
					}
					charCnt++;
				}
			}
		}
	}
	csvData.emplace_back(unionData);

	// Tmx_Size���ް����M
	MesPacket sendData;
	sendData.resize(1);
	//sendData[0].iData = csvData.size();
	sendData[0].cData[0] = 21;
	sendData[0].cData[1] = 17;
	sendData[0].cData[2] = 4;
	sendData[0].cData[3] = 0;
	lpNetWork.SendMes(sendData, MesType::TMX_Size);

	// Tmx_Data��ͯ�ް��
	lpNetWork.SendMes(csvData, MesType::TMX_Data);
}

void TileLoader::Draw(void)
{
	auto draw = [&](std::string key) {
		for (int y = 0; y < tmxInfo_.height; y++)
		{
			for (int x = 0; x < tmxInfo_.width; x++)
			{
				if (mapData_[key][y * tmxInfo_.width + x])
				{
					DrawGraph(x * tmxInfo_.tileWidth, y * tmxInfo_.tileHeight, lpImageMng.GetID("map")[mapData_[key][y * tmxInfo_.width + x] - 1], true);
				}
			}
		}
	};
	draw("Bg");
	draw("Item");
	draw("Obj");
	draw("Char");
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

int TileLoader::GetMapData(std::string layer, Vector2 pos)
{
	pos = { pos.x / tmxInfo_.tileWidth, pos.y / tmxInfo_.tileHeight };
	return mapData_[layer][pos.y * tmxInfo_.width + pos.x];
}

void TileLoader::SetFireGenerator(const Vector2& pos, int length)
{
	fireGeneratorList_.push_back(FireGenerator{pos, length});
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
