#pragma once
#include<list>

enum class NetWorkMode
{
	Non,
	Offline,
	Host,
	Guest,
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

using NetHandleList = std::list<std::pair<int, unsigned int>>;

class NetWorkState
{
public:
	NetWorkState();
	virtual ~NetWorkState();
	virtual NetWorkMode GetMode(void) { return NetWorkMode::Offline; };
	bool Update(void);
	ActiveState GetActiveState(void);
	void SetActiveState(ActiveState state);
	NetHandleList& GetNetHandle(void);
	virtual ActiveState ConnectHost(IPDATA hostIP);

private:
	virtual bool CheckNetWork(void) { return true; };

protected:
	const int portNum_ = 8086;
	ActiveState active_;
	NetHandleList netHandleList_;
};

