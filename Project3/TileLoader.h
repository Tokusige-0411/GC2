#pragma once
#include <memory>
#include <string>
#include <map>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

#define lpTileLoader TileLoader::GetInstance()

struct TMXInfo
{
	int width{};					// ��Ͻ��
	int height{};					// �cϽ��
	int tileWidth{};				// ��ى���
	int tileHeight{};				// ��ُc��
};

struct TSXInfo
{
	int width{};					// ��Ͻ��
	int height{};					// �cϽ��
	int tileWidth{};				// ��ى���
	int tileHeight{};				// ��ُc��
	int chipNum{};					// ��ّ���
	std::string imageName{};
};

struct charBit
{
	unsigned char ch1;
	unsigned char ch2;
	unsigned char ch3;
	unsigned char ch4;
};

union revInt
{
	charBit charB;
	unsigned int string;
};

using MapData = std::map<std::string, std::vector<int>>;

class TileLoader
{
public:
	static TileLoader& GetInstance()
	{
		return *s_Instance;
	}

	bool TMXLoader(std::string fileName);
	bool TSXLoader(std::string fileName);
	void SendTmxSizeData(void);
	void SendTmxData(void);
	void Draw(void);
	const TMXInfo& GetTmxInfo(void);
	const TSXInfo& GetTsxInfo(void);
	const MapData& GetMapData(void);

private:
	struct TileLoderDeleter
	{
		void operator() (TileLoader* tileLoader) const
		{
			delete tileLoader;
		}
	};

	// ϯ�ߊ֘A
	TMXInfo tmxInfo_{};
	TSXInfo tsxInfo_{};
	MapData mapData_;

	rapidxml::xml_document<> doc_;

	std::map<std::string, bool> version_;

	bool Init(void);
	TileLoader();
	~TileLoader();
	static std::unique_ptr<TileLoader, TileLoderDeleter> s_Instance;
};

