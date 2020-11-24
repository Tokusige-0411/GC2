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

	bool TMXLoader(std::string fileName);				// TMX̧�ق�۰�ް
	bool TSXLoader(std::string fileName);				// TSX̧�ق�۰�ް
	void SendTmxData(void);								// TMX�̏�񑗐M�֐�
	void Draw(void);									// ϯ�ߕ`��
	const TMXInfo& GetTmxInfo(void);					// TMX���擾
	const TSXInfo& GetTsxInfo(void);					// TSX���擾
	const MapData& GetMapData(void);					// ϯ���ް��擾
	int GetMapData(std::string layer, Vector2 pos);		// �w�肳�ꂽ�ʒu��ϯ�ߏ���Ԃ�
	void SetFireGenerator(const Vector2& pos, int length);

private:
	// ϯ�ߊ֘A
	TMXInfo tmxInfo_;						// TMX���
	TSXInfo tsxInfo_;						// TSX���
	MapData mapData_;						// ϯ�ߏ��

	std::vector<Fire> fireMap_;
	std::list<FireGenerator> fireGeneratorList_;

	rapidxml::xml_document<> doc_;			// xmļ�ق̐eɰ��

	std::map<std::string, bool> version_;	// ϯ���ް�ޮ݊Ǘ�

	bool Init(void);						// ������
};

