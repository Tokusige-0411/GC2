#pragma once

enum class NetWorkMode
{
	Offline,
	Host,
	Gest,
	Max
};
enum class ActiveState
{
	Non,
	Wait,
	Init,
	Stanby,
	Play,
	Offline,
};

class NetWorkState
{
public:
	NetWorkState();
	virtual ~NetWorkState();
	virtual NetWorkMode GetMode(void) { return NetWorkMode::Offline; };
	bool Update(void);
	ActiveState GetActiveState(void);
	void SetActiveState(ActiveState state);
	int GetNetHandle(void);
	virtual bool ConnectHost(IPDATA hostIP);

private:
	virtual bool CheckNetWork(void) { return false; };

protected:
	const int portNum_ = 8086;
	ActiveState active_;
	int netHandle_ = 0;
};

