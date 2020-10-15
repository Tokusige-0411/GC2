#pragma once
#include<DxLib.h>
#include<memory>
#include"NetWorkState.h"

#define lpNetWork NetWark::GetInstance()

class NetWark
{
public:
	static NetWark& GetInstance()
	{
		return *s_Instance;
	}

	void Update(void);

	bool SetNetWorkMode(NetWorkMode mode);
	NetWorkMode GetNetWorkMode(void);

	bool GetActive(void);

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

	NetWark();
	~NetWark();
	static std::unique_ptr<NetWark, NetWorkDeleter> s_Instance;
};

