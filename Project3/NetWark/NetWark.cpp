#include "NetWark.h"

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
