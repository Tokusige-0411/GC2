#include<DxLib.h>
#include"EffekseerForDXLib.h"
#include"SceneMng.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	lpSceneMng.Run();
	Effkseer_End();
	DxLib_End();
	return 0;
}
