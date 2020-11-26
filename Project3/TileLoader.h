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

enum class BlockType
{
	Non = 1,
	Floor_Blue,
	Floor_Red,
	Char,
	Black,
	Wall_Blue,
	Wall_Red,
	Break_Block,
	Item_1,
	Item_2,
	Item_3,
	Item_4,
	Max
};

struct ClossFire
{
	int up : 1;
	int down : 1;
	int right : 1;
	int left : 1;
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
	Vector2 pos{};
	FireDir dir{};
	double animCnt{};
	int length{};
};

class FireGenerator;

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
	void FireUpdate(double delta);
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
	double delta_;

	FireMap fireMap_;
	std::list<std::unique_ptr<FireGenerator>> fireGeneratorList_;

	rapidxml::xml_document<> doc_;			// xmļ�ق̐eɰ��

	std::map<std::string, bool> version_;	// ϯ���ް�ޮ݊Ǘ�

	bool Init(void);						// ������
};

