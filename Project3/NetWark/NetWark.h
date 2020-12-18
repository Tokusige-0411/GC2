#pragma once
#include<DxLib.h>
#include<memory>
#include<utility>
#include<array>
#include<vector>
#include<chrono>
#include<thread>
#include<mutex>
#include<map>
#include<functional>
#include<list>
#include"NetWorkState.h"
#include"../Scene/SceneMng.h"

#define BIT_NUM 2
#define INT_BYTE_CNT 4
#define lpNetWork NetWark::GetInstance()
#define COUNT_DOWN_MAX 15000
#define GAME_START_TIME 10000
#define UNIT_ID_NUM 5

// ﾒｯｾｰｼﾞ種別
enum class MesType : unsigned char
{
	Non = 100,			// ﾀｲﾌﾟなし
	Count_Down_Room,	// 接続制限時間
	ID,					// ﾌﾟﾚｲﾔｰID
	Stanby_Host,		// ｽﾀﾝﾊﾞｲ
	Stanby_Guest,		// ｹﾞｰﾑ開始
	Count_Down_Game,	// ｹﾞｰﾑ開始時間
	TMX_Size,			// TMXのｻｲｽﾞ情報
	TMX_Data,			// TMXのCSVﾃﾞｰﾀ情報
	Pos,				// ﾌﾟﾚｲﾔｰの座標
	Set_Bomb,			// ﾎﾞﾑ設置
	Deth,				// 死亡情報
	Result,				// 結果情報
	Lost,				// 切断情報
	Max,
};

enum class StartState
{
	Wait,
	Countdown,
	GameStart,
	Max
};

// ｱﾗｲﾒﾝﾄに注意
// ﾍｯﾀﾞｰ情報
struct MesHeader 
{
	MesType type;				// ﾒｯｾｰｼﾞ種別
	unsigned char next;			// 次の情報があるかどうか
	unsigned short sendID;		// 何番目に送ったﾃﾞｰﾀか
	unsigned int length;		// ﾃﾞｰﾀ部の長さ
};

// ﾍｯﾀﾞｰ部分の共用体
union Header
{
	MesHeader mes;
	unsigned int data[2];
};

union TimeUnion
{
	std::chrono::system_clock::time_point time;
	unsigned int data[2];
};

// 送信ﾃﾞｰﾀ
union unionData
{
	unsigned int uiData;
	int iData;
	char cData[4];
};

using ArrayIP = std::array<IPDATA, 2>;						// IPｱﾄﾞﾚｽ格納配列
using MesPacket = std::vector<unionData>;					// ﾊﾟｹｯﾄﾃﾞｰﾀ
using MesPair = std::pair<MesType, MesPacket>;
using MesPacketList = std::vector<MesPair>;
using PairInt = std::pair<int, int>;

class NetWark
{
public:
	static NetWark& GetInstance()
	{
		return *s_Instance;
	}

	void RunUpdata(void);												// Update別ｽﾚｯﾄﾞ化
	void Update(void);													// 更新
	void InitCloseNetWork(void);										// 切断時ﾈｯﾄﾜｰｸ情報初期化
	void EndNetWork(void);

	void AddMesList(int id, MesPacketList& list, std::mutex& mutex);	// 

	bool SendMesAll(MesPacket packet, MesType type, int handle);		// ﾎｽﾄがみんなにﾃﾞｰﾀを送るときに使う
	bool SendMes(MesPacket& packet, MesType type, int handle);
	bool SendMes(MesPacket& packet, MesType type);						// ﾃﾞｰﾀ部ありﾒｯｾｰｼﾞ送信
	bool SendMes(MesType type);											// ﾃﾞｰﾀ部なしﾒｯｾｰｼﾞ送信
	void SendStanby(void);												// ｽﾀﾝﾊﾞｲ情報送信
	void SendStart(void);												// ｹﾞｰﾑｽﾀｰﾄ情報送信
	void SendCountRoom(void);											// 接続待機時間送信
	void SendCountGame(void);
	void SendPlayerID(void);											// 各ゲストにPlayerID送信
	void SendResult(void);
	void SendLost(int handle, int playerID);

	bool SetNetWorkMode(NetWorkMode mode);								// ﾈｯﾄﾜｰｸﾓｰﾄﾞの設定
	NetWorkMode GetNetWorkMode(void);									// ﾈｯﾄﾜｰｸﾓｰﾄﾞ取得

	ActiveState GetActive(void);										// ｱｸﾃｨﾌﾞｽﾃｰﾄ取得

	int GetNetHandleList(void);											// ﾈｯﾄﾜｰｸﾊﾝﾄﾞﾙ取得

	ActiveState ConnectHost(IPDATA hostIP);								// ﾎｽﾄへの接続

	ArrayIP GetIP(void);												// IPｱﾄﾞﾚｽ取得

	PairInt GetPlayerInf(void);
	void SetPlayerInf(int max);

	time_point GetConnectTime(void);	
	void SetConnectTime(time_point time);
	bool GetConnectFlag(void);
	void SetConnectFlag(bool flag);

	time_point GetStartTime(void);
	StartState GetStartState(void);
	void SetStartState(StartState state);

	MesPacket GetResult(void);
	void SetResult(unsigned int playerID);

private:
	struct NetWorkDeleter
	{
		void operator() (NetWark* netWork) const
		{
			delete netWork;
		}
	};

	bool Init(void);												// 初期化

	void MakeTmx(MesPacket tmxData);								// TMXﾌｧｲﾙ作成関数

	std::unique_ptr<NetWorkState> netState_;						// ﾈｯﾄﾜｰｸ状態管理
	bool revStanby_;												// ｽﾀﾝﾊﾞｲ状態管理ﾌﾗｸﾞ
	ArrayIP ipData_;												// IPｱﾄﾞﾚｽ格納
	MesPacket revBox_;												// 受信情報格納変数
	int intSendCnt_;												// 送信ﾃﾞｰﾀの上限

	std::thread updata_;											// 別ｽﾚｯﾄﾞ化したｱｯﾌﾟﾃﾞｰﾄ

	std::mutex revStanbyMtx_;										// revStanbyにﾛｯｸをかける
	std::mutex mtx_;												// 変数にﾛｯｸをかける
										
	std::chrono::system_clock::time_point start;					// 接続開始時間
	std::chrono::system_clock::time_point end;						// 接続終了時間

	time_point countStartTime_;										// ホストが接続し始めた時間
	bool connectFlag_;												// 接続ができたかどうか
	time_point gameStartTime_;										// ｹﾞｰﾑｽﾀｰﾄする時間
	StartState startState_;											// ｽﾀｰﾄまでのｽﾃｰﾄ管理
	PairInt playerInf_;												// <自分のPlayerID, Playerの総人数>
	MesPacket result_;												// ｹﾞｰﾑ結果情報

	std::vector<std::pair<MesPacketList&, std::mutex&>> playerMesList_;		// 送られてきたﾌﾟﾚｲﾔｰ情報を格納する場所

	NetWark();
	~NetWark();
	static std::unique_ptr<NetWark, NetWorkDeleter> s_Instance;
};

