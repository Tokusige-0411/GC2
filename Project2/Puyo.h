#pragma once
#include<memory>
#include<map>
#include"Vector2.h"
#include"input/INPUT_ID.h"

enum class Puyo_Type
{
	NON,
	RED,
	BRUE,
	GREEN,
	YELLOW,
	PURPLE,
	WALL,
	MAX
};

struct DirBit
{
	unsigned int up : 1;
	unsigned int down : 1;
	unsigned int left : 1;
	unsigned int right : 1;
};

union DirPermit
{
	DirBit bit;
	unsigned int perBit;
};

class Puyo
{
public:
	Puyo(Vector2&& pos, Puyo_Type id);
	~Puyo();

	bool Update(void);
	void Move(INPUT_ID id);
	void Draw(void);

	const Vector2& Pos(void);
	void Pos(Vector2&& pos);
	const Vector2& Size(void);
	const Vector2 Grid(int size);
	const Puyo_Type& Type(void);
	void SetDirPermit(DirPermit dirParmit);
	const DirPermit& GetDirParmit(void);
	void SetDrawPermit(DirPermit drawPermit);

	void SoftDrop(void);

	bool Alive(void);
	void Alive(bool flag);

	bool AddPuyonCnt(void);

private:
	const Vector2 _puyoSize;					// �Ղ�̻���
	DirPermit dirParmit_;						// �ǂ��ɓ����邩���׸�
	DirPermit drawParmit_;						// �ǂ��ɐL�΂��̂����׸�

	Vector2 pos_;								// ���W
	Puyo_Type puyoID_;							// �Ղ�̎��
	bool alive_;								// �����邩�����Ȃ���

	int color_;									// �F
	std::map<Puyo_Type, int> colorMap_;			// �װ���ފi�[ϯ��

	int dropInt_;								// ��ۯ�߂̲������
	int dropCnt_;								// ��ۯ�߂̊Ԋu����
	int dropSpeed_;								// ��ۯ�߂̽�߰��

	int puyonCnt_;								// �Ղ��̶����
	int puyonCntMax_;							// �Ղ�񎞊�
};

