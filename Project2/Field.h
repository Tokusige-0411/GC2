#pragma once
#include<memory>
#include<functional>
#include<list>
#include<vector>
#include<array>
#include"input/Controller.h"
#include"Puyo.h"
#include"OjamaPuyo.h"
#include"Vector2.h"
#include"PLAYER_NUM.h"

class NextPuyoCtl;
class PlayerUnit;
struct DropMode;
struct FallMode;
struct RensaMode;
struct PuyonMode;
struct MunyonMode;
struct WinMode;
struct LoseMode;
struct DrawMode;

enum class FieldState
{
	Drop,
	Puyon,
	Rensa,
	Fall,
	Munyon,
	Win,
	Lose,
	Draw,
	Max
};

enum class ResultF
{
	Win,
	Lose,
	Draw,
	Max
};

using SharedPuyo = std::shared_ptr<Puyo>;
using PairPuyo = std::pair<SharedPuyo, SharedPuyo>;

class Field
{
public:
	Field();
	Field(Vector2&& offset, Vector2&& size);
	~Field();
	int Update(int ojama);						// �ð�ނ̱����ް�
	void Draw(void);							// �ð�ޕ`��
	void OjamaDraw(void);
	void GuideDraw(void);
	void DrawField(void);						// �ð�ޕ`��p�֐�
	bool Init(void);							// ������
	int GetScreenID(void);						// ��ذ�ID�擾
	Vector2 GetOffset(void);					// �̾�Ĉʒu�擾
	bool InstancePuyo(void);					// �Ղ��ݽ�ݽ����
	bool SetEraseData(SharedPuyo& puyo);		// ������Ղ�̐ݒ������
	bool SetParmit(SharedPuyo& puyo);			// �Ղ����Parmit���ׂ�
	void InstanceOjama(void);					// �w����������ؽĂɒǉ�����
	void SetResult(ResultF result);				// ػ��Đݒ�
	const ResultF GetResult(void);				// ػ��ď��擾
	Vector2 ConvertGrid(Vector2 grid);			// ��د�ނ����W�Ɋ��Z����

private:
	friend class PlayerUnit;
	friend struct DropMode;
	friend struct FallMode;
	friend struct RensaMode;
	friend struct PuyonMode;
	friend struct MunyonMode;
	friend struct WinMode;
	friend struct LoseMode;
	friend struct DrawMode;

	void ChangeCont(void);												// ���۰װ��ς���

	const Vector2 stgGridSize_;											// �ð�ނ�Ͻ����
	Vector2 fieldSize_;													// �ð�ނ̕`�滲��
	Vector2 offset_;													// �`��ʒu�␳���W
	int blockSize_;														// 1Ͻ�̻���

	std::unique_ptr<Controller> controller_;							// ���͏��

	std::unique_ptr<NextPuyoCtl> nextCtl_;								// ȸ�ĂՂ�Ǘ��p
	std::vector<SharedPuyo> puyoVec_;									// �Ղ�̏��(��Xvector��)

	std::vector<SharedPuyo> dataBase_;									// �ð�ނ��ް�
	std::vector<SharedPuyo*> data_;										// �ð�ނɱ������邽�߂��ް���

	std::vector<SharedPuyo> eraseDataBase_;								// �폜�Ղ���ް�
	std::vector<SharedPuyo*> eraseData_;								// �폜�Ղ汸���ް�

	std::list<SharedPuyo> ojamaList_;									// �������ؽ�
	int rensaCnt_;														// ���A���߂�
	int rensaMax_;														// �ő�A����
	int erasePuyoCnt_;													// �������Ղ�̐�
	int ojamaCnt_;														// �Ұ�ޓI�ɂ͑���ɑ���Ղ�̌�
	int ojamaFlag_;														// ������܂��~���Ă������ǂ���

	ResultF result_;													// ����������������
	static bool gameEnd_;												// �ްт��I���������ǂ���

	std::unique_ptr<PlayerUnit> playerUnit_;							// �Ղ摀��֘A�׽

	FieldState fieldState_;												// ̨���ނ��ǂ̏�Ԃ�(�������A����)
	std::map<FieldState, std::function<bool(Field&)>> fieldMode_;		// ̨���ޏ��

	int player_;														// player����
	static int plCount_;												// ��ڲ԰�ԍ�

	int screenID_;														// ��ذݏ��

	static std::array<int , 2> changeKey_;								// ���۰װ�ύX��
	std::pair<int, int> changeTrg_;										// ���۰װ�ύX���̉�������
	ContType contType_;													// ���ő��삷�邩
	std::map<ContType, std::unique_ptr<Controller>> contMap_;			// ���۰װ��ϯ��

	int targetID_;

	PairPuyo guidePuyo;													// �����n�_�\���Ղ�
	std::list<SharedPuyo> guidePuyoList_;								// 
};

