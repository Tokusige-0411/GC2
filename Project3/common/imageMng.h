#pragma once
#include <memory>
#include <map>
#include <vector>
#include <string>
#include "Vector2.h"
#include "imageMng.h"

// lp ﾛﾝｸﾞﾎﾟｲﾝﾀｰ
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
	// ｺﾝｽﾄﾗｸﾀ
	static ImageMng& GetInstance(void)
	{
		// imageMngの参照を返したい
		// imageMngを返したい
		// *はｽﾏｰﾄﾎﾟｲﾝﾀｰの中身を返す
		return *_sInstance;
	}

	// ｷｰ(今回は文字列)
	const VecInt& GetID(const ObjectID& key);									// あるかどうかを確認
	const VecInt& GetID(const ObjectID& key, const std::string& fileName);		// 1枚絵
	const VecInt& GetID(const ObjectID& key, const std::string& fileName,		// ﾁｯﾌﾟ画像
						const Vector2 divSize, const Vector2 divCnt);
private:
	// ｽﾏｰﾄﾎﾟｲﾝﾀｰはﾃﾞﾌｫﾙﾄでﾃﾞｽﾄﾗｸﾀｰが呼ばれる
	// ﾕﾆｰｸﾎﾟｲﾝﾀｰは管理しているだけ(ﾒﾝﾊﾞｰではない)なのでﾃﾞｽﾄﾗｸﾀにｱｸｾｽできない
	// ｶｽﾀﾑﾃﾞﾘｰﾀｰ　関数ｵﾌﾞｼﾞｪｸﾄはstructで作る
	// 関数ｵﾌﾞｼﾞｪｸﾄは何でも作れる
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

	// std::map<ｷｰの型, ｷｰからｱｸｾｽした配列の型>
	// ｲﾒｰｼﾞﾏｯﾌﾟ<画像のｷｰ, 画像を格納した場所>
	std::map<ObjectID, std::vector<int>> imgMap;
};

