#include<Dxlib.h>
#include"EffekseerForDXLib.h"
#include"EffectCtl.h"

void EffectCtl::Update(void)
{
	UpdateEffekseer2D();

	auto itl = std::remove_if(playList_.begin(), playList_.end(), [](int handle) {return !(IsEffekseer2DEffectPlaying(handle) == 0); });
	playList_.erase(itl, playList_.end());
}

void EffectCtl::Draw(int handle, const Vector2 pos)
{
	SetPosPlayingEffekseer2DEffect(handle, pos.x, pos.y, 0.0f);
	DrawEffekseer2D_Begin();
	DrawEffekseer2D_Draw(handle);
	DrawEffekseer2D_End();
}

bool EffectCtl::StopAll(void)
{
	for (auto handle : playList_)
	{
		StopEffekseer2DEffect(handle);
	}
	return false;
}

int EffectCtl::Play(std::string name)
{
	playList_.push_front(PlayEffekseer2DEffect(GetHandles(name)));
	return *(playList_.begin());
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
