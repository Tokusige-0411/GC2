#pragma once
#include<DxLib.h>
#include<memory>
#include<array>
#include<vector>
#include<chrono>
#include<thread>
#include<mutex>
#include"NetWorkState.h"

#define lpNetWork NetWark::GetInstance()

enum class MesType : unsigned char
{
	Stanby,
	Game_Start,
	TMX_Size,
	TMX_Data,
	Pos,
};

// ±◊≤“›ƒÇ…íçà”
struct MesData 
{
	MesType type;
	unsigned short sData;
	unsigned char cData;
	int data[2];
};

union unionData
{
	char cData[8];
	int iData[2];
	long long lData;
};

using ArrayIP = std::array<IPDATA, 2>;
using RevBox = std::vector<unionData>;

class NetWark
{
public:
	static NetWark& GetInstance()
	{
		return *s_Instance;
	}

	void RunUpdata(void);
	void Update(void);
	void CloseNetWork(void);

	bool SendMes(MesData& data);
	void SendStanby(void);
	void SendStart(void);

	bool SetNetWorkMode(NetWorkMode mode);
	NetWorkMode GetNetWorkMode(void);

	ActiveState GetActive(void);

	int GetNetHandle(void);

	ActiveState ConnectHost(IPDATA hostIP);

	ArrayIP GetIP(void);

private:
	struct NetWorkDeleter
	{
		void operator() (NetWark* netWork) const
		{
			delete netWork;
		}
	};

	std::unique_ptr<NetWorkState> netState_;
	bool revStanby;
	ArrayIP ipData_;
	RevBox revBox_;
	int revCnt_;

	std::thread updata_;
	std::mutex mtx_;

	MesType revState_;
	std::chrono::system_clock::time_point start;
	std::chrono::system_clock::time_point end;

	NetWark();
	~NetWark();
	static std::unique_ptr<NetWark, NetWorkDeleter> s_Instance;
};

