#pragma once
#include<DxLib.h>

#define lpNetWork NetWark::GetInstance()

class NetWark
{
public:
	static NetWark& GetInstance()
	{
		static NetWark s_Instance;
		return s_Instance;
	}

	NetWark();
	~NetWark();

	IPDATA GetIP(void);
};

