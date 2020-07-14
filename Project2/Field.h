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

	std::unique_ptr<Puyo> _puyo;					// �Ղ�̏��(��Xvector��)
	std::vector<std::unique_ptr<Puyo>> _puyoList;	// �Ղ�̏��(̨���ޓ�)

	std::vector<int> _dataBase;
	std::vector<int*> _data;

	// �ޯĉ��Z�ł��ł������A�Ƃ肠����ϯ��
	std::map<INPUT_ID, bool> _moveFlag;				// �e�����ړ��ł��邩�ǂ���

	int _player;									// player����
	static int _plCount;							// ��ڲ԰�ԍ�

	int _screenID;									// ��ذݏ��
};

