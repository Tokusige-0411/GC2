#pragma once
#include<memory>
#include"input/InputState.h"
#include"input/Controller.h"
#include"Puyo.h"
#include"Vector2.h"
#include"PLAYER_NUM.h"

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

private:
	Vector2 _fieldSize;
	Vector2 _offset;

	std::unique_ptr<Controller> _controller;		// ���͏��

	std::vector<std::unique_ptr<Puyo>> _puyo;		// �Ղ�̏��(��Xvector��)

	std::vector<int> _dataBase;						// �ð�ނ��ް�
	std::vector<int*> _data;						// �ð�ނɱ������邽�߂��ް���

	int _player;									// player����
	static int _plCount;							// ��ڲ԰�ԍ�

	int _screenID;									// ��ذݏ��
};

