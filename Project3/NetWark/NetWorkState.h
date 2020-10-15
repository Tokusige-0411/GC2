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
	bool Update(void);
	bool GetActive(void);
	int GetNetHandle(void);
	virtual bool ConnectHost(IPDATA hostIP);

private:
	virtual bool CheckNetWork(void) { return false; };

protected:
	const int portNum_ = 8086;
	bool active_;
	int netHandle_ = 0;
};

