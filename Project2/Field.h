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
	Max
};

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
	bool SetEraseData(void);
	//bool SetParmit(std::unique_ptr<Puyo>& puyo);			// ‚Õ‚æˆêŒÂˆêŒÂParmit’²‚×‚é

private:
	friend class PlayerUnit;

	const Vector2 stgGridSize_;
	Vector2 _fieldSize;
	Vector2 _offset;
	int _blockSize;

	std::unique_ptr<Controller> _controller;		// “ü—Íî•ñ

	std::vector<std::unique_ptr<Puyo>> _puyoVec;		// ‚Õ‚æ‚Ìî•ñ(ŒãXvector‚É)

	std::vector<Puyo_Type> _dataBase;				// ½Ã°¼Ş‚ÌÃŞ°À
	std::vector<Puyo_Type*> _data;					// ½Ã°¼Ş‚É±¸¾½‚·‚é‚½‚ß‚ÌÃŞ°À•”

	std::vector<Puyo_Type> eraseDataBase_;			// íœ‚Õ‚æ‚ÌÃŞ°À
	std::vector<Puyo_Type*> eraseData_;				// íœ‚Õ‚æ±¸¾½ÃŞ°À

	std::unique_ptr<PlayerUnit> playerUnit_;		// ‚Õ‚æ‘€ìŠÖ˜A¸×½

	FieldState fieldState_;							// Ì¨°ÙÄŞ‚ª‚Ç‚Ìó‘Ô‚©(—‰º‚©˜A½‚©)

	int _player;									// player‰½‚©
	static int _plCount;							// ÌßÚ²Ô°”Ô†

	int _screenID;									// ½¸Ø°İî•ñ
};

