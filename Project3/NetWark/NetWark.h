#pragma once
#include<DxLib.h>
#include<memory>
#include <utility>
#include<array>
#include<vector>
#include<chrono>
#include<thread>
#include<mutex>
#include<map>
#include<functional>
#include"NetWorkState.h"

#define BIT_NUM 2
#define INT_BYTE_CNT 4
#define lpNetWork NetWark::GetInstance()

// ﾒｯｾｰｼﾞ種別
enum class MesType : unsigned char
{
	Non = 100,		// ﾀｲﾌﾟなし
	Stanby,			// ｽﾀﾝﾊﾞｲ
	Game_Start,		// ｹﾞｰﾑ開始
	TMX_Size,		// TMXのｻｲｽﾞ情報
	TMX_Data,		// TMXのCSVﾃﾞｰﾀ情報
	Pos,			// ﾌﾟﾚｲﾔｰの座標
	Set_Bomb,
	Deth,
	Max,
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

class NetWark
{
public:
	static NetWark& GetInstance()
	{
		return *s_Instance;
	}

	void RunUpdata(void);									// Update別ｽﾚｯﾄﾞ化
	void Update(void);										// 更新
	void InitCloseNetWork(void);							// 切断時ﾈｯﾄﾜｰｸ情報初期化

	void AddMesList(int id, MesPacketList& list, std::mutex& mutex);

	bool SendMes(MesPacket& packet, MesType type);			// ﾃﾞｰﾀ部ありﾒｯｾｰｼﾞ送信
	bool SendMes(MesType type);								// ﾃﾞｰﾀ部なしﾒｯｾｰｼﾞ送信
	void SendStanby(void);									// ｽﾀﾝﾊﾞｲ情報送信
	void SendStart(void);									// ｹﾞｰﾑｽﾀｰﾄ情報送信

	bool SetNetWorkMode(NetWorkMode mode);					// ﾈｯﾄﾜｰｸﾓｰﾄﾞの設定
	NetWorkMode GetNetWorkMode(void);						// ﾈｯﾄﾜｰｸﾓｰﾄﾞ取得

	ActiveState GetActive(void);							// ｱｸﾃｨﾌﾞｽﾃｰﾄ取得

	int GetNetHandle(void);									// ﾈｯﾄﾜｰｸﾊﾝﾄﾞﾙ取得

	ActiveState ConnectHost(IPDATA hostIP);					// ﾎｽﾄへの接続

	ArrayIP GetIP(void);									// IPｱﾄﾞﾚｽ取得



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

	std::vector<std::pair<MesPacketList&, std::mutex&>> playerList_;		// 送られてきたﾌﾟﾚｲﾔｰ情報を格納する場所

	NetWark();
	~NetWark();
	static std::unique_ptr<NetWark, NetWorkDeleter> s_Instance;
};

