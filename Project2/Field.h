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

	std::unique_ptr<Controller> _controller;		// “ü—Íî•ñ

	std::vector<std::unique_ptr<Puyo>> _puyo;		// ‚Õ‚æ‚Ìî•ñ(ŒãXvector‚É)

	std::vector<int> _dataBase;						// ½Ã°¼Ş‚ÌÃŞ°À
	std::vector<int*> _data;						// ½Ã°¼Ş‚É±¸¾½‚·‚é‚½‚ß‚ÌÃŞ°À•”

	int _player;									// player‰½‚©
	static int _plCount;							// ÌßÚ²Ô°”Ô†

	int _screenID;									// ½¸Ø°İî•ñ
};

