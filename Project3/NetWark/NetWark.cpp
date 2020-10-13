#include "NetWark.h"

std::unique_ptr<NetWark, NetWark::NetWorkDeleter> NetWark::s_Instance(new NetWark());

NetWark::NetWark()
{
}

NetWark::~NetWark()
{
}

IPDATA NetWark::GetIP(void)
{
	IPDATA data;
	GetMyIPAddress(&data);
	return data;
}
