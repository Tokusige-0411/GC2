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

	std::unique_ptr<Puyo> _puyo;					// ‚Õ‚æ‚Ìî•ñ(ŒãXvector‚É)
	std::vector<std::unique_ptr<Puyo>> _puyoList;	// ‚Õ‚æ‚Ìî•ñ(Ì¨°ÙÄŞ“à)

	std::vector<int> _dataBase;
	std::vector<int*> _data;

	// ËŞ¯Ä‰‰Z‚Å‚à‚Å‚«‚»‚¤A‚Æ‚è‚ ‚¦‚¸Ï¯Ìß
	std::map<INPUT_ID, bool> _moveFlag;				// Še•ûŒüˆÚ“®‚Å‚«‚é‚©‚Ç‚¤‚©

	int _player;									// player‰½‚©
	static int _plCount;							// ÌßÚ²Ô°”Ô†

	int _screenID;									// ½¸Ø°İî•ñ
};

