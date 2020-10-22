#pragma once
#include <memory>
#include <string>
#include <map>

#define lpTileLoader TileLoader::GetInstance()

struct TMXInfo
{
	int width;					// ��Ͻ��
	int height;					// �cϽ��
	int tileWidth;				// ��ى���
	int tileHeight;				// ��ُc��
};

struct TSXInfo
{
	int width;					// ��Ͻ��
	int height;					// �cϽ��
	int tileWidth;				// ��ى���
	int tileHeight;				// ��ُc��
	int chipNum;				// ��ّ���
	std::string fileName;

};

using MapData = std::map<std::string, std::vector<int>>;

class TileLoader
{
public:
	static TileLoader& GetInstance()
	{
		return *s_Instance;
	}

	bool TMXLoader(void);
	bool TSXLoader(void);
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
	TMXInfo tmxInfo_;
	TSXInfo tsxInfo_;

	MapData mapData_;

	std::map<std::string, bool> version_;

	bool Init(void);
	TileLoader();
	~TileLoader();
	static std::unique_ptr<TileLoader, TileLoderDeleter> s_Instance;
};

