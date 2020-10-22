#include <DxLib.h>
#include "imageMng.h"

// 名前空間大事！
std::unique_ptr<ImageMng, ImageMng::ImageMngDeleter> ImageMng::_sInstance(new ImageMng());

// あるかどうかの確認をするだけ
const VecInt & ImageMng::GetID(const std::string & key)
{
	return GetID(key, key);
}

// 1枚絵
const VecInt & ImageMng::GetID(const std::string & key, const std::string & fileName)
{
	if (imgMap.find(key) == imgMap.end())
	{
		imgMap[key].resize(1);
		imgMap[key][0] = LoadGraph(fileName.c_str());
	}
	return imgMap[key];
}

// ﾁｯﾌﾟなど
const VecInt & ImageMng::GetID(const std::string & key, const std::string & fileName, const Vector2 divSize, const Vector2 divCnt)
{
	// ｲﾃﾚｰﾀｰ(反復子) 覆いかぶさっているもの
	// ビギンとエンド
	if(imgMap.find(key) == imgMap.end())
	{
		imgMap[key].resize(divCnt.x * divCnt.y);
		LoadDivGraph(fileName.c_str(), divCnt.x * divCnt.y, divCnt.x, divCnt.y, divSize.x, divSize.y, &imgMap[key][0]);
	}
	return imgMap[key];
}

ImageMng::ImageMng()
{
}

ImageMng::~ImageMng()
{
}
