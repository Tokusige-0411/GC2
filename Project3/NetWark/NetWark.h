#pragma once
#include<DxLib.h>
#include<memory>

#define lpNetWork NetWark::GetInstance()

class NetWark
{
public:
	static NetWark& GetInstance()
	{
		return *s_Instance;
	}
	IPDATA GetIP(void);

private:
	struct NetWorkDeleter
	{
		void operator() (NetWark* netWork) const
		{
			delete netWork;
		}
	};
	NetWark();
	~NetWark();
	static std::unique_ptr<NetWark, NetWorkDeleter> s_Instance;
};

