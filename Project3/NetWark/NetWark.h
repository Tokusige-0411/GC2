#pragma once
#include<DxLib.h>
#include<memory>
#include<array>
#include<vector>
#include"NetWorkState.h"

#define lpNetWork NetWark::GetInstance()

enum class MesType
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
	int data[2];
};

using ArrayIP = std::array<IPDATA, 2>;
using RevBox = std::vector<char>;

class NetWark
{
public:
	static NetWark& GetInstance()
	{
		return *s_Instance;
	}

	bool Update(void);
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
	NetWark();
	~NetWark();
	static std::unique_ptr<NetWark, NetWorkDeleter> s_Instance;
};

