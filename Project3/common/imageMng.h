#pragma once
#include <memory>
#include <map>
#include <vector>
#include <string>
#include "Vector2.h"
#include "imageMng.h"

// lp �ݸ��߲���
#define lpImageMng ImageMng::GetInstance()
#define IMAGE_ID(KEY) (ImageMng::GetInstance().GetID(KEY))

enum class ObjectID
{
	Player,
	Bomb,
	Enemy,
	Fire,
	Map,
	Max
};

using VecInt = std::vector<int>;

class ImageMng
{
public:
	// �ݽ�׸�
	static ImageMng& GetInstance(void)
	{
		// imageMng�̎Q�Ƃ�Ԃ�����
		// imageMng��Ԃ�����
		// *�ͽϰ��߲����̒��g��Ԃ�
		return *_sInstance;
	}

	// ��(����͕�����)
	const VecInt& GetID(const ObjectID& key);									// ���邩�ǂ������m�F
	const VecInt& GetID(const ObjectID& key, const std::string& fileName);		// 1���G
	const VecInt& GetID(const ObjectID& key, const std::string& fileName,		// ���߉摜
						const Vector2 divSize, const Vector2 divCnt);
private:
	// �ϰ��߲�������̫�Ă��޽�׸�����Ă΂��
	// �ư��߲����͊Ǘ����Ă��邾��(���ް�ł͂Ȃ�)�Ȃ̂��޽�׸��ɱ����ł��Ȃ�
	// ������ذ���@�֐���޼ު�Ă�struct�ō��
	// �֐���޼ު�Ă͉��ł�����
	struct ImageMngDeleter
	{
		void operator() (ImageMng* imageMng) const
		{
			delete imageMng;
		}
	};
	ImageMng();
	~ImageMng();

	static std::unique_ptr<ImageMng, ImageMngDeleter> _sInstance;

	// std::map<���̌^, �����籸�������z��̌^>
	// �Ұ��ϯ��<�摜�̷�, �摜���i�[�����ꏊ>
	std::map<ObjectID, std::vector<int>> imgMap;
};

