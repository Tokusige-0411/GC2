#pragma once
#include<memory>
#include<functional>
#include<list>
#include<vector>
#include<array>
#include"input/Controller.h"
#include"Puyo.h"
#include"OjamaPuyo.h"
#include"Vector2.h"
#include"PLAYER_NUM.h"

class NextPuyoCtl;
class PlayerUnit;
struct DropMode;
struct FallMode;
struct RensaMode;
struct PuyonMode;
struct MunyonMode;
struct WinMode;
struct LoseMode;
struct DrawMode;

enum class FieldState
{
	Drop,
	Puyon,
	Rensa,
	Fall,
	Munyon,
	Win,
	Lose,
	Draw,
	Max
};

enum class ResultF
{
	Win,
	Lose,
	Draw,
	Max
};

using SharedPuyo = std::shared_ptr<Puyo>;
using PairPuyo = std::pair<SharedPuyo, SharedPuyo>;

class Field
{
public:
	Field();
	Field(Vector2&& offset, Vector2&& size);
	~Field();
	int Update(int ojama);						// ｽﾃｰｼﾞのｱｯﾌﾟﾃﾞｰﾄ
	void Draw(void);							// ｽﾃｰｼﾞ描画
	void DrawField(void);						// ｽﾃｰｼﾞ描画用関数
	int GetScreenID(void);						// ｽｸﾘｰﾝID取得
	Vector2 GetOffset(void);					// ｵﾌｾｯﾄ位置取得
	bool InstancePuyo(void);					// ぷよをｲﾝｽﾀﾝｽする
	bool SetEraseData(SharedPuyo& puyo);		// 消せるぷよの設定をする
	bool SetParmit(SharedPuyo& puyo);			// ぷよ一個一個Parmit調べる
	void InstanceOjama(void);					// 指定個数おじゃまﾘｽﾄに追加する
	void SetResult(ResultF result);				// ﾘｻﾞﾙﾄ設定
	const ResultF GetResult(void);				// ﾘｻﾞﾙﾄ情報取得
	Vector2 ConvertGrid(Vector2 grid);			// ｸﾞﾘｯﾄﾞを座標に換算する

private:
	friend class PlayerUnit;
	friend struct DropMode;
	friend struct FallMode;
	friend struct RensaMode;
	friend struct PuyonMode;
	friend struct MunyonMode;
	friend struct WinMode;
	friend struct LoseMode;
	friend struct DrawMode;

	bool Init(void);													// 初期化
	void ChangeCont(void);												// ｺﾝﾄﾛｰﾗｰを変える
	void OjamaDraw(void);												// おじゃまの描画部分
	void GuideDraw(void);												// ｶﾞｲﾄﾞ表示
	void LogoDraw(int handle, Vector2 pos, double rad);					// ﾛｺﾞの表示
	void SetLogo(void);

	const Vector2 stgGridSize_;											// ｽﾃｰｼﾞのﾏｽｻｲｽﾞ
	Vector2 fieldSize_;													// ｽﾃｰｼﾞの描画ｻｲｽﾞ
	Vector2 offset_;													// 描画位置補正座標
	int blockSize_;														// 1ﾏｽのｻｲｽﾞ
	double rad_;														// ｽﾃｰｼﾞの角度
	int fallSpeed_;														// ｽﾃｰｼﾞの落ちるｽﾋﾟｰﾄﾞ
	int resultEndCnt_;													// 勝ち負け表示時間

	std::unique_ptr<NextPuyoCtl> nextCtl_;								// ﾈｸｽﾄぷよ管理用
	std::vector<SharedPuyo> puyoVec_;									// ぷよの情報(後々vectorに)

	std::vector<SharedPuyo> dataBase_;									// ｽﾃｰｼﾞのﾃﾞｰﾀ
	std::vector<SharedPuyo*> data_;										// ｽﾃｰｼﾞにｱｸｾｽするためのﾃﾞｰﾀ部

	std::vector<SharedPuyo> eraseDataBase_;								// 削除ぷよのﾃﾞｰﾀ
	std::vector<SharedPuyo*> eraseData_;								// 削除ぷよｱｸｾｽﾃﾞｰﾀ

	std::list<SharedPuyo> ojamaList_;									// おじゃまﾘｽﾄ
	int rensaCnt_;														// 何連鎖めか
	int rensaMax_;														// 最大連鎖数
	int erasePuyoCnt_;													// 消したぷよの数
	int ojamaCnt_;														// ｲﾒｰｼﾞ的には相手に送るぷよの個数
	int ojamaFlag_;														// おじゃまが降っていいかどうか

	ResultF result_;													// 勝ったか負けたか
	static bool gameEnd_;												// ｹﾞｰﾑが終了したかどうか

	std::unique_ptr<PlayerUnit> playerUnit_;							// ぷよ操作関連ｸﾗｽ

	FieldState fieldState_;												// ﾌｨｰﾙﾄﾞがどの状態か(落下か連鎖か)
	std::map<FieldState, std::function<bool(Field&)>> fieldMode_;		// ﾌｨｰﾙﾄﾞ状態

	int player_;														// player何か
	static int plCount_;												// ﾌﾟﾚｲﾔｰ番号

	int screenID_;														// ｽｸﾘｰﾝ情報

	static std::array<int , 2> changeKey_;								// ｺﾝﾄﾛｰﾗｰ変更ｷｰ
	std::pair<int, int> changeTrg_;										// ｺﾝﾄﾛｰﾗｰ変更ｷｰの押下判定
	ContType contType_;													// 何で操作するか
	std::map<ContType, std::unique_ptr<Controller>> contMap_;			// ｺﾝﾄﾛｰﾗｰのﾏｯﾌﾟ

	int targetID_;														// 操作ぷよ強調

	PairPuyo guidePuyo;													// 落下地点表示ぷよ

	std::array<int, 2> fieldBG_;										// ﾌｨｰﾙﾄﾞの背景
	int fieldFrame_;													// ﾌｨｰﾙﾄﾞのﾌﾚｰﾑ

	std::array<int, 10> rensaLogo_;										// 連鎖のﾛｺﾞ
	Vector2 rensaLogoPos_;												// 連鎖のﾛｺﾞ表示位置
	int rensaLogoDrawCnt_;
	int rensaDrawCnt_;

	std::map<ResultF, int> resultLogo_;									// 勝ち負けﾛｺﾞﾊﾝﾄﾞﾙ
	Vector2 resultLogoPos_;												// 勝ち負けﾛｺﾞ表示位置
};

