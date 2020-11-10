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

using MapData = std::map<std::string, std::vector<int>>;

class TileLoader
{
public:
	static TileLoader& GetInstance()
	{
		return *s_Instance;
	}

	bool TMXLoader(std::string fileName);		// TMX̧�ق�۰�ް
	bool TSXLoader(std::string fileName);		// TSX̧�ق�۰�ް
	void SendTmxData(void);						// TMX�̏�񑗐M�֐�
	void Draw(void);							// ϯ�ߕ`��
	const TMXInfo& GetTmxInfo(void);			// TMX���擾
	const TSXInfo& GetTsxInfo(void);			// TSX���擾
	const MapData& GetMapData(void);			// ϯ���ް��擾

private:
	struct TileLoderDeleter
	{
		void operator() (TileLoader* tileLoader) const
		{
			delete tileLoader;
		}
	};

	// ϯ�ߊ֘A
	TMXInfo tmxInfo_;						// TMX���
	TSXInfo tsxInfo_;						// TSX���
	MapData mapData_;						// ϯ�ߏ��

	rapidxml::xml_document<> doc_;			// xmļ�ق̐eɰ��

	std::map<std::string, bool> version_;	// ϯ���ް�ޮ݊Ǘ�

	bool Init(void);						// ������
	TileLoader();
	~TileLoader();
	static std::unique_ptr<TileLoader, TileLoderDeleter> s_Instance;
};

