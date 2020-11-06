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
struct MesHeader 
{
	MesType type;
	unsigned char cData;
	unsigned short id;
	unsigned int length;
};

union Header
{
	MesHeader mes;
	int data[2];
};

struct SizeData
{
	unsigned int count;
	unsigned int size;
	unsigned int allSize;
};

union unionData
{
	unsigned int uiData;
	int iData;
	char cData[4];
};

using ArrayIP = std::array<IPDATA, 2>;
using MesPacket = std::vector<unionData>;

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

	bool SendMes(MesPacket& data);
	void SendStanby(void);
	void SendStart(void);

	bool SetNetWorkMode(NetWorkMode mode);
	NetWorkMode GetNetWorkMode(void);

	ActiveState GetActive(void);

	int GetNetHandle(void);

	ActiveState ConnectHost(IPDATA hostIP);

	ArrayIP GetIP(void);

	void SetHeader(Header header, MesPacket& packet);

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
	MesPacket revBox_;
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

