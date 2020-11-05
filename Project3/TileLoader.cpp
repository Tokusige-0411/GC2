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

void TileLoader::SendTmxSizeData(void)
{
	std::ifstream ifp("MapData.tmx");
	ifp.seekg(0, ifp.end);
	//MesHeader sData = { MesType::TMX_Size, 0, 0, 90, 0 };
	TRACE("�T�C�Y�𑗂�����\n");
	//lpNetWork.SendMes(sData);
}

void TileLoader::SendTmxData(void)
{
	// ̧�ّ���ł�����@
	std::ifstream ifp("MapData.tmx");
	std::string str;
	//unionData csvData = { 0 };								// csv�ǂݍ��݂Ŏg�����p��
	//int charCnt = 0;											// ���̃f�[�^�𑗂邩

	// TMX_size��ͯ�ް����
	MesHeader header = { MesType::TMX_Size , 0, 0, sizeof(SizeData) };
	auto iHeader = Header{ header };
	MesPacket sendData;
	sendData.resize(sizeof(header) / sizeof(int) + 3);
	sendData[0].iData = iHeader.data[0];
	sendData[1].iData = iHeader.data[1];

	SizeData sizeData = { 0, 1000, 0 };
	std::vector<unsigned char> csvData;

	while (!ifp.eof())
	{
		// csv�̂Ƃ���܂ōs���΂�
		while (str.find("data encoding") == std::string::npos)
		{
			std::getline(ifp, str);
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
					csvData.emplace_back(static_cast<unsigned char>(std::atoi(csvStr.c_str())));
				}
			}
		}
	}

	// Tmx_Size���ް���
	sizeData.allSize = csvData.size();
	sizeData.count = sizeData.allSize / sizeData.size + 1;
	sendData[2].iData = sizeData.count;
	sendData[3].iData = sizeData.size;
	sendData[4].iData = sizeData.allSize;
	lpNetWork.SendMes(sendData);
	sendData.clear();

	// Tmx_Data��ͯ�ް��
	header = { MesType::TMX_Data, 0, 0, sizeData.size };
	iHeader = Header{ header };
	sendData.resize(sizeof(header) / sizeof(int) + sizeData.size);
	sendData[0].iData = iHeader.data[0];
	sendData[1].iData = iHeader.data[1];

	// Tmx_Data���ް���
	int count = 0;
	std::vector<unsigned char> sendVec;
	for (auto& data : csvData)
	{
		count++;
		if (count % sizeData.size == 0)
		{
			header.length = sendVec.size();
			header.id++;
			//lpNetWork.SendMes(header);
			sendVec.clear();
		}
	}

	header.length = sendVec.size();
	header.id++;
	//lpNetWork.SendMes(header);
	NetWorkSend(lpNetWork.GetNetHandle(), &sendVec, static_cast<int>(sendVec.size()));

	//while (!ifp.eof())
	//{
	//	// csv�̂Ƃ���܂ōs���΂�
	//	while (str.find("data encoding") == std::string::npos)
	//	{
	//		std::getline(ifp, str);
	//		if (ifp.eof())
	//		{
	//			break;
	//		}
	//	}

	//	// csv�̕�����ǂݍ���
	//	if (!ifp.eof())
	//	{
	//		while (std::getline(ifp, str))
	//		{
	//			if (str.find("/data") != std::string::npos)
	//			{
	//				break;
	//			}

	//			std::istringstream iss(str);
	//			std::string csvStr;
	//			while (std::getline(iss, csvStr, ','))
	//			{
	//				// �s���琔�������o����
	//				auto csvNum = std::atoi(csvStr.c_str());

	//				// ���p�̂ɃA�N�Z�X���ăV�t�g���Ȃ���f�[�^�����Ă���
	//				csvData.cData[(charCnt % 16) / 2] |= (csvNum << (4 * ((charCnt % 16) % 2)));

	//				if ((charCnt % 16) == 15)
	//				{
	//					// 16���o�����̂��ް��𑗂��
	//					sendData.data[0] = csvData.iData[0];
	//					sendData.data[1] = csvData.iData[1];
	//					lpNetWork.SendMes(sendData);
	//					sendData.sData++;
	//					TRACE("���M��:%d\n", sendData.sData);
	//					csvData = { 0 };
	//				}
	//				charCnt++;
	//			}
	//		}
	//	}
	//}
	//if ((charCnt % 16))
	//{
	//	sendData.data[0] = csvData.iData[0];
	//	sendData.data[1] = csvData.iData[1];
	//	lpNetWork.SendMes(sendData);
	//	sendData.sData++;
	//	TRACE("���M��:%d\n", sendData.sData);
	//}
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
