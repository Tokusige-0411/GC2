#include<DxLib.h>
#include"Field.h"
#include"Vector2.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	ChangeWindowMode(true);
	SetGraphMode(840, 600, 16);

	if (DxLib_Init() == -1)
	{
		return 0;
	}

	SetDrawScreen(DX_SCREEN_BACK);

	Field field = Field({ 40, 80 });

	while(!ProcessMessage() || CheckHitKey(KEY_INPUT_ESCAPE))
	{
		ClsDrawScreen();

		field.Draw();

		ScreenFlip();
	}

	DxLib_End();
	return 0;
}
