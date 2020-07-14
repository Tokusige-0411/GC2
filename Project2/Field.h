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

	std::unique_ptr<Controller> _controller;		// 入力情報

	std::unique_ptr<Puyo> _puyo;					// ぷよの情報(後々vectorに)
	std::vector<std::unique_ptr<Puyo>> _puyoList;	// ぷよの情報(ﾌｨｰﾙﾄﾞ内)

	std::vector<int> _dataBase;
	std::vector<int*> _data;

	int _player;									// player何か
	static int _plCount;							// ﾌﾟﾚｲﾔｰ番号

	int _screenID;									// ｽｸﾘｰﾝ情報
};

