#pragma once
#include <memory>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <chrono>
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

enum class FireDir
{
	Right,
	Down,
	Left,
	Up,
	Max
};

struct Fire
{
	FireDir dir;
	int animCnt;
	std::chrono::system_clock::time_point time;
	bool drawFlag;
};

using MapData = std::map<std::string, std::vector<int>>;
using FireMap = std::vector<Fire>;

class TileLoader
{
public:
	TileLoader();
	~TileLoader();

	bool TMXLoader(std::string fileName);				// TMX̧�ق�۰�ް
	bool TSXLoader(std::string fileName);				// TSX̧�ق�۰�ް
	void SendTmxData(void);								// TMX�̏�񑗐M�֐�
	void Draw(void);									// ϯ�ߕ`��
	void FireUpdate(void);
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

	FireMap fireMap_;
	std::list<FireGenerator> fireGeneratorList_;

	rapidxml::xml_document<> doc_;			// xmļ�ق̐eɰ��

	std::map<std::string, bool> version_;	// ϯ���ް�ޮ݊Ǘ�

	bool Init(void);						// ������
};

