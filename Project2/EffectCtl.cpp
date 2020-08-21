#include<Dxlib.h>
#include"EffekseerForDXLib.h"
#include"EffectCtl.h"

void EffectCtl::Update(void)
{
	UpdateEffekseer2D();

	auto itl = std::remove_if(playList_.begin(), playList_.end(), [](int handle) {return !IsEffekseer2DEffectPlaying(handle); });
	playList_.erase(itl, playList_.end());
}

void EffectCtl::Draw(void)
{
	DrawEffekseer2D();
}

bool EffectCtl::StopAll(void)
{
	for (auto handle : playList_)
	{
		StopEffekseer2DEffect(handle);
	}
	return false;
}

bool EffectCtl::Play(std::string name, const Vector2& pos)
{
	playList_.push_front(PlayEffekseer2DEffect(GetHandles(name)));
	SetPosPlayingEffekseer2DEffect(*(playList_.begin()), static_cast<float>(pos.x), static_cast<float>(pos.y), 0.0f);
	return true;
}

bool EffectCtl::Init(Vector2&& size)
{
	if (Effekseer_Init(8000) == -1)
	{
		return false;
	}

	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	Effekseer_Set2DSetting(size.x, size.y);

	SetUseZBuffer3D(TRUE);

	SetWriteZBuffer3D(TRUE);

	return true;
}

int EffectCtl::GetHandles(std::string name)
{
	handles_.try_emplace(name, LoadEffekseerEffect(("effect/" + name + ".efk").c_str(), 5.0f));
	return handles_[name];
}

EffectCtl::EffectCtl()
{
}

EffectCtl::~EffectCtl()
{
}
