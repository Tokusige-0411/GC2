#pragma once
#include<memory>
#include"input/InputState.h"
#include"input/Controller.h"
#include"Puyo.h"
#include"Vector2.h"
#include"PLAYER_NUM.h"

class PlayerUnit;

enum class FieldState
{
	Drop,
	Rensa,
	Puyon,
	Fall,
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
	bool SetEraseData(SharedPuyo& puyo);
	bool SetParmit(SharedPuyo& puyo);			// �Ղ����Parmit���ׂ�

private:
	friend class PlayerUnit;

	const Vector2 stgGridSize_;
	Vector2 _fieldSize;
	Vector2 _offset;
	int _blockSize;

	std::unique_ptr<Controller> _controller;		// ���͏��

	std::vector<SharedPuyo> _puyoVec;	// �Ղ�̏��(��Xvector��)

	std::vector<SharedPuyo> _dataBase;				// �ð�ނ��ް�
	std::vector<SharedPuyo*> _data;					// �ð�ނɱ������邽�߂��ް���

	std::vector<SharedPuyo> eraseDataBase_;			// �폜�Ղ���ް�
	std::vector<SharedPuyo*> eraseData_;				// �폜�Ղ汸���ް�

	std::unique_ptr<PlayerUnit> playerUnit_;		// �Ղ摀��֘A�׽

	FieldState fieldState_;							// ̨���ނ��ǂ̏�Ԃ�(�������A����)

	int _player;									// player����
	static int _plCount;							// ��ڲ԰�ԍ�

	int _screenID;									// ��ذݏ��
};

