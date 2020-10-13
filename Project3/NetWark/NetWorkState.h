#pragma once

enum class NetWorkMode
{
	Offline,
	Host,
	Gest,
	Max
};
class NetWorkState
{
public:
	NetWorkState();
	virtual ~NetWorkState();
	virtual NetWorkMode GetMode(void) { return NetWorkMode::Offline; };
};

