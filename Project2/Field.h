#pragma once
#include<memory>
#include"InputState.h"
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

	std::shared_ptr<InputState> _input;				// ���͏��

	std::unique_ptr<Puyo> _puyo;

	PLAYER_ID _player;								// player����
	static int _plCount;										// ��ڲ԰�ԍ�

	int _screenID;
};

