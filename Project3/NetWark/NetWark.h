#pragma once
#include<DxLib.h>
#include<memory>
#include<array>
#include<vector>
#include<chrono>
#include<thread>
#include<mutex>
#include<map>
#include<functional>
#include"NetWorkState.h"

#define lpNetWork NetWark::GetInstance()

// ү���ގ��
enum class MesType : unsigned char
{
	Non = 100,		// ���߂Ȃ�
	Stanby,			// ����޲
	Game_Start,		// �ްъJ�n
	TMX_Size,		// TMX�̻��ޏ��
	TMX_Data,		// TMX��CSV�ް����
	Pos,			// ��ڲ԰�̍��W
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

// ���M�ް�
union unionData
{
	unsigned int uiData;
	int iData;
	char cData[4];
};

using ArrayIP = std::array<IPDATA, 2>;						// IP���ڽ�i�[�z��
using MesPacket = std::vector<unionData>;					// �߹���ް�
using MesList = std::vector<MesPacket>;

class NetWark
{
public:
	static NetWark& GetInstance()
	{
		return *s_Instance;
	}

	void RunUpdata(void);									// Update�ʽگ�މ�
	void Update(void);										// �X�V
	void InitCloseNetWork(void);							// �ؒf��ȯ�ܰ���񏉊���

	void AddMesList(int id, MesList& list);

	bool SendMes(MesPacket& packet, MesType type);			// �ް�������ү���ޑ��M
	bool SendMes(MesType type);								// �ް����Ȃ�ү���ޑ��M
	void SendStanby(void);									// ����޲��񑗐M
	void SendStart(void);									// �ްѽ��ď�񑗐M

	bool SetNetWorkMode(NetWorkMode mode);					// ȯ�ܰ�Ӱ�ނ̐ݒ�
	NetWorkMode GetNetWorkMode(void);						// ȯ�ܰ�Ӱ�ގ擾

	ActiveState GetActive(void);							// ��è�޽ðĎ擾

	int GetNetHandle(void);									// ȯ�ܰ�����َ擾

	ActiveState ConnectHost(IPDATA hostIP);					// νĂւ̐ڑ�

	ArrayIP GetIP(void);									// IP���ڽ�擾



private:
	struct NetWorkDeleter
	{
		void operator() (NetWark* netWork) const
		{
			delete netWork;
		}
	};

	bool Init(void);
	void MakeTmx(MesPacket tmxData);										// TMX̧�ٍ쐬�֐�

	std::unique_ptr<NetWorkState> netState_;				// ȯ�ܰ���ԊǗ�
	bool revStanby_;										// ����޲��ԊǗ��׸�
	ArrayIP ipData_;										// IP���ڽ�i�[
	MesPacket revBox_;										// ��M���i�[�ϐ�
	int intSendCnt_;										// ���M�ް��̏��

	std::thread updata_;									// �ʽگ�މ����������ް�

	std::mutex revStanbyMtx_;								// revStanby��ۯ���������
	std::mutex mtx_;										// �ϐ���ۯ���������
										
	std::chrono::system_clock::time_point start;			// �ڑ��J�n����
	std::chrono::system_clock::time_point end;				// �ڑ��I������

	//std::map<int, MesList&> playerList_;				// �����Ă�����ڲ԰�����i�[����ꏊ
	//std::map<int, std::mutex> playerMtx_;
	//std::map<int, MesList&> playerList_;
	std::vector<std::reference_wrapper<MesList>> playerList_;

	NetWark();
	~NetWark();
	static std::unique_ptr<NetWark, NetWorkDeleter> s_Instance;
};

