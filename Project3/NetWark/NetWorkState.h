#pragma once
#include<list>
#include<mutex>

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

struct PlayerHandle
{
	int handle;
	unsigned int playerID;
	int netState;
};

using NetHandleList = std::list<PlayerHandle>;

class NetWorkState
{
public:
	NetWorkState();
	virtual ~NetWorkState();
	virtual NetWorkMode GetMode(void) { return NetWorkMode::Offline; };
	bool Update(void);
	ActiveState GetActiveState(void);
	void SetActiveState(ActiveState state);
	NetHandleList GetNetHandle(void);
	std::mutex& GetHandleMtx(void);
	void ResetNetHandle(void);
	virtual ActiveState ConnectHost(IPDATA hostIP);

private:
	virtual bool CheckNetWork(void) { return true; };

protected:
	const int portNum_ = 8086;
	ActiveState active_;
	NetHandleList netHandleList_;
	std::mutex handleMtx_;
};

