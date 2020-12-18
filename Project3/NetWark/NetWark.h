#pragma once
#include<DxLib.h>
#include<memory>
#include<utility>
#include<array>
#include<vector>
#include<chrono>
#include<thread>
#include<mutex>
#include<map>
#include<functional>
#include<list>
#include"NetWorkState.h"
#include"../Scene/SceneMng.h"

#define BIT_NUM 2
#define INT_BYTE_CNT 4
#define lpNetWork NetWark::GetInstance()
#define COUNT_DOWN_MAX 15000
#define GAME_START_TIME 10000
#define UNIT_ID_NUM 5

// ү���ގ��
enum class MesType : unsigned char
{
	Non = 100,			// ���߂Ȃ�
	Count_Down_Room,	// �ڑ���������
	ID,					// ��ڲ԰ID
	Stanby_Host,		// ����޲
	Stanby_Guest,		// �ްъJ�n
	Count_Down_Game,	// �ްъJ�n����
	TMX_Size,			// TMX�̻��ޏ��
	TMX_Data,			// TMX��CSV�ް����
	Pos,				// ��ڲ԰�̍��W
	Set_Bomb,			// ��ѐݒu
	Deth,				// ���S���
	Result,				// ���ʏ��
	Lost,				// �ؒf���
	Max,
};

enum class StartState
{
	Wait,
	Countdown,
	GameStart,
	Max
};

// �ײ��Ăɒ���
// ͯ�ް���
struct MesHeader 
{
	MesType type;				// ү���ގ��
	unsigned char next;			// ���̏�񂪂��邩�ǂ���
	unsigned short sendID;		// ���Ԗڂɑ������ް���
	unsigned int length;		// �ް����̒���
};

// ͯ�ް�����̋��p��
union Header
{
	MesHeader mes;
	unsigned int data[2];
};

union TimeUnion
{
	std::chrono::system_clock::time_point time;
	unsigned int data[2];
};

// ���M�ް�
union unionData
{
	unsigned int uiData;
	int iData;
	char cData[4];
};

using ArrayIP = std::array<IPDATA, 2>;						// IP���ڽ�i�[�z��
using MesPacket = std::vector<unionData>;					// �߹���ް�
using MesPair = std::pair<MesType, MesPacket>;
using MesPacketList = std::vector<MesPair>;
using PairInt = std::pair<int, int>;

class NetWark
{
public:
	static NetWark& GetInstance()
	{
		return *s_Instance;
	}

	void RunUpdata(void);												// Update�ʽگ�މ�
	void Update(void);													// �X�V
	void InitCloseNetWork(void);										// �ؒf��ȯ�ܰ���񏉊���
	void EndNetWork(void);

	void AddMesList(int id, MesPacketList& list, std::mutex& mutex);	// 

	bool SendMesAll(MesPacket packet, MesType type, int handle);		// νĂ��݂�Ȃ��ް��𑗂�Ƃ��Ɏg��
	bool SendMes(MesPacket& packet, MesType type, int handle);
	bool SendMes(MesPacket& packet, MesType type);						// �ް�������ү���ޑ��M
	bool SendMes(MesType type);											// �ް����Ȃ�ү���ޑ��M
	void SendStanby(void);												// ����޲��񑗐M
	void SendStart(void);												// �ްѽ��ď�񑗐M
	void SendCountRoom(void);											// �ڑ��ҋ@���ԑ��M
	void SendCountGame(void);
	void SendPlayerID(void);											// �e�Q�X�g��PlayerID���M
	void SendResult(void);
	void SendLost(int handle, int playerID);

	bool SetNetWorkMode(NetWorkMode mode);								// ȯ�ܰ�Ӱ�ނ̐ݒ�
	NetWorkMode GetNetWorkMode(void);									// ȯ�ܰ�Ӱ�ގ擾

	ActiveState GetActive(void);										// ��è�޽ðĎ擾

	int GetNetHandleList(void);											// ȯ�ܰ�����َ擾

	ActiveState ConnectHost(IPDATA hostIP);								// νĂւ̐ڑ�

	ArrayIP GetIP(void);												// IP���ڽ�擾

	PairInt GetPlayerInf(void);
	void SetPlayerInf(int max);

	time_point GetConnectTime(void);	
	void SetConnectTime(time_point time);
	bool GetConnectFlag(void);
	void SetConnectFlag(bool flag);

	time_point GetStartTime(void);
	StartState GetStartState(void);
	void SetStartState(StartState state);

	MesPacket GetResult(void);
	void SetResult(unsigned int playerID);

private:
	struct NetWorkDeleter
	{
		void operator() (NetWark* netWork) const
		{
			delete netWork;
		}
	};

	bool Init(void);												// ������

	void MakeTmx(MesPacket tmxData);								// TMX̧�ٍ쐬�֐�

	std::unique_ptr<NetWorkState> netState_;						// ȯ�ܰ���ԊǗ�
	bool revStanby_;												// ����޲��ԊǗ��׸�
	ArrayIP ipData_;												// IP���ڽ�i�[
	MesPacket revBox_;												// ��M���i�[�ϐ�
	int intSendCnt_;												// ���M�ް��̏��

	std::thread updata_;											// �ʽگ�މ����������ް�

	std::mutex revStanbyMtx_;										// revStanby��ۯ���������
	std::mutex mtx_;												// �ϐ���ۯ���������
										
	std::chrono::system_clock::time_point start;					// �ڑ��J�n����
	std::chrono::system_clock::time_point end;						// �ڑ��I������

	time_point countStartTime_;										// �z�X�g���ڑ����n�߂�����
	bool connectFlag_;												// �ڑ����ł������ǂ���
	time_point gameStartTime_;										// �ްѽ��Ă��鎞��
	StartState startState_;											// ���Ă܂ł̽ðĊǗ�
	PairInt playerInf_;												// <������PlayerID, Player�̑��l��>
	MesPacket result_;												// �ްь��ʏ��

	std::vector<std::pair<MesPacketList&, std::mutex&>> playerMesList_;		// �����Ă�����ڲ԰�����i�[����ꏊ

	NetWark();
	~NetWark();
	static std::unique_ptr<NetWark, NetWorkDeleter> s_Instance;
};

