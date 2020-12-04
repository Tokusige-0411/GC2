#pragma once
#include <memory>
#include <map>
#include <vector>
#include <string>
#include "Vector2.h"
#include "imageMng.h"

// lp Ûİ¸ŞÎß²İÀ°
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
	// ºİ½Ä×¸À
	static ImageMng& GetInstance(void)
	{
		// imageMng‚ÌQÆ‚ğ•Ô‚µ‚½‚¢
		// imageMng‚ğ•Ô‚µ‚½‚¢
		// *‚Í½Ï°ÄÎß²İÀ°‚Ì’†g‚ğ•Ô‚·
		return *_sInstance;
	}

	// ·°(¡‰ñ‚Í•¶š—ñ)
	const VecInt& GetID(const ObjectID& key);									// ‚ ‚é‚©‚Ç‚¤‚©‚ğŠm”F
	const VecInt& GetID(const ObjectID& key, const std::string& fileName);		// 1–‡ŠG
	const VecInt& GetID(const ObjectID& key, const std::string& fileName,		// Á¯Ìß‰æ‘œ
						const Vector2 divSize, const Vector2 divCnt);
private:
	// ½Ï°ÄÎß²İÀ°‚ÍÃŞÌ«ÙÄ‚ÅÃŞ½Ä×¸À°‚ªŒÄ‚Î‚ê‚é
	// ÕÆ°¸Îß²İÀ°‚ÍŠÇ—‚µ‚Ä‚¢‚é‚¾‚¯(ÒİÊŞ°‚Å‚Í‚È‚¢)‚È‚Ì‚ÅÃŞ½Ä×¸À‚É±¸¾½‚Å‚«‚È‚¢
	// ¶½ÀÑÃŞØ°À°@ŠÖ”µÌŞ¼Şª¸Ä‚Ístruct‚Åì‚é
	// ŠÖ”µÌŞ¼Şª¸Ä‚Í‰½‚Å‚àì‚ê‚é
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

	// std::map<·°‚ÌŒ^, ·°‚©‚ç±¸¾½‚µ‚½”z—ñ‚ÌŒ^>
	// ²Ò°¼ŞÏ¯Ìß<‰æ‘œ‚Ì·°, ‰æ‘œ‚ğŠi”[‚µ‚½êŠ>
	std::map<ObjectID, std::vector<int>> imgMap;
};

