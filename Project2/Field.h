#pragma once
#include<memory>
#include<functional>
#include"input/InputState.h"
#include"input/Controller.h"
#include"Puyo.h"
#include"Vector2.h"
#include"PLAYER_NUM.h"

class PlayerUnit;
struct DropMode;
struct FallMode;
struct RensaMode;
struct PuyonMode;
struct MunyonMode;

enum class FieldState
{
	Drop,
	Puyon,
	Rensa,
	Fall,
	Munyon,
	Max
};

using SharedPuyo = std::shared_ptr<Puyo>;

class Field
{
public:
	Field();
	Field(Vector2&& offset, Vector2&& size);
	~Field();
	void Update(void);
	void Draw(void);
	bool Init(void);
	int GetScreenID(void);
	Vector2 GetOffset(void);
	bool InstancePuyo(void);
	bool SetEraseData(SharedPuyo& puyo);		// ������Ղ�̐ݒ������
	bool SetParmit(SharedPuyo& puyo);			// �Ղ����Parmit���ׂ�
	bool SetPuyon(SharedPuyo& puyo);			// �Ղ��Ǘ�
	bool SetMunyon(SharedPuyo& puyo);			// �Ղ���Ȃ���

private:
	friend class PlayerUnit;
	friend struct DropMode;
	friend struct FallMode;
	friend struct RensaMode;
	friend struct PuyonMode;
	friend struct MunyonMode;

	const Vector2 stgGridSize_;											// �ð�ނ�Ͻ����
	Vector2 _fieldSize;													// �ð�ނ̕`�滲��
	Vector2 _offset;													// �`��ʒu�␳���W
	int blockSize_;														// 1Ͻ�̻���

	std::unique_ptr<Controller> _controller;							// ���͏��

	std::vector<SharedPuyo> puyoVec_;									// �Ղ�̏��(��Xvector��)

	std::vector<SharedPuyo> _dataBase;									// �ð�ނ��ް�
	std::vector<SharedPuyo*> _data;										// �ð�ނɱ������邽�߂��ް���

	std::vector<SharedPuyo> eraseDataBase_;								// �폜�Ղ���ް�
	std::vector<SharedPuyo*> eraseData_;								// �폜�Ղ汸���ް�

	std::unique_ptr<PlayerUnit> playerUnit_;							// �Ղ摀��֘A�׽

	FieldState fieldState_;												// ̨���ނ��ǂ̏�Ԃ�(�������A����)
	std::map<FieldState, std::function<void(Field&)>> fieldMode_;		// ̨���ޏ��

	int _player;														// player����
	static int _plCount;												// ��ڲ԰�ԍ�

	int _screenID;														// ��ذݏ��
};

