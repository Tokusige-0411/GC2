#pragma once
#include<DxLib.h>
#include<memory>
#include"NetWorkState.h"

#define lpNetWork NetWark::GetInstance()

enum class MesType
{
	Stanby,
	Game_Start,
	Pos,
};

// ±◊≤“›ƒÇ…íçà”
struct MesData 
{
	MesType type;
	int data[2];
};

class NetWark
{
public:
	static NetWark& GetInstance()
	{
		return *s_Instance;
	}

	bool Update(void);

	bool SendMes(MesData& data);
	void SendStanby(void);

	bool SetNetWorkMode(NetWorkMode mode);
	NetWorkMode GetNetWorkMode(void);

	ActiveState GetActive(void);

	int GetNetHandle(void);

	bool ConnectHost(IPDATA hostIP);

	IPDATA GetIP(void);

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

	NetWark();
	~NetWark();
	static std::unique_ptr<NetWark, NetWorkDeleter> s_Instance;
};

