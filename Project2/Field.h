#pragma once
#include<memory>
#include"input/InputState.h"
#include"input/Controller.h"
#include"Puyo.h"
#include"Vector2.h"
#include"PLAYER_NUM.h"

class PlayerUnit;

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

private:
	const Vector2 stgGridSize_;
	Vector2 _fieldSize;
	Vector2 _offset;
	int _blockSize;

	std::unique_ptr<Controller> _controller;		// 入力情報

	std::vector<std::unique_ptr<Puyo>> _puyo;		// ぷよの情報(後々vectorに)

	std::vector<Puyo_Type> _dataBase;				// ｽﾃｰｼﾞのﾃﾞｰﾀ
	std::vector<Puyo_Type*> _data;					// ｽﾃｰｼﾞにｱｸｾｽするためのﾃﾞｰﾀ部

	std::vector<Puyo_Type> eraseDataBase_;			// 削除ぷよのﾃﾞｰﾀ
	std::vector<Puyo_Type*> eraseData_;				// 削除ぷよｱｸｾｽﾃﾞｰﾀ

	std::unique_ptr<PlayerUnit> playerUnit_;		// ぷよ操作関連ｸﾗｽ

	int _player;									// player何か
	static int _plCount;							// ﾌﾟﾚｲﾔｰ番号

	int _screenID;									// ｽｸﾘｰﾝ情報

	friend class PlayerUnit;
};

