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
	bool SetEraseData(SharedPuyo& puyo);		// 消せるぷよの設定をする
	bool SetParmit(SharedPuyo& puyo);			// ぷよ一個一個Parmit調べる
	bool SetPuyon(SharedPuyo& puyo);			// ぷよん管理
	bool SetMunyon(SharedPuyo& puyo);			// ぷよをつなげる

private:
	friend class PlayerUnit;
	friend struct DropMode;
	friend struct FallMode;
	friend struct RensaMode;
	friend struct PuyonMode;
	friend struct MunyonMode;

	const Vector2 stgGridSize_;											// ｽﾃｰｼﾞのﾏｽｻｲｽﾞ
	Vector2 _fieldSize;													// ｽﾃｰｼﾞの描画ｻｲｽﾞ
	Vector2 _offset;													// 描画位置補正座標
	int blockSize_;														// 1ﾏｽのｻｲｽﾞ

	std::unique_ptr<Controller> _controller;							// 入力情報

	std::vector<SharedPuyo> puyoVec_;									// ぷよの情報(後々vectorに)

	std::vector<SharedPuyo> _dataBase;									// ｽﾃｰｼﾞのﾃﾞｰﾀ
	std::vector<SharedPuyo*> _data;										// ｽﾃｰｼﾞにｱｸｾｽするためのﾃﾞｰﾀ部

	std::vector<SharedPuyo> eraseDataBase_;								// 削除ぷよのﾃﾞｰﾀ
	std::vector<SharedPuyo*> eraseData_;								// 削除ぷよｱｸｾｽﾃﾞｰﾀ

	std::unique_ptr<PlayerUnit> playerUnit_;							// ぷよ操作関連ｸﾗｽ

	FieldState fieldState_;												// ﾌｨｰﾙﾄﾞがどの状態か(落下か連鎖か)
	std::map<FieldState, std::function<void(Field&)>> fieldMode_;		// ﾌｨｰﾙﾄﾞ状態

	int _player;														// player何か
	static int _plCount;												// ﾌﾟﾚｲﾔｰ番号

	int _screenID;														// ｽｸﾘｰﾝ情報
};

