#include<DxLib.h>
#include"main.h"
#include"Myself.h"
#include"TicketMachine.h"

// ½¸Ø°Ý»²½Þx800, y600

int WINAPI WinMain(HINSTANCE , HINSTANCE , LPSTR, int)
{
	Myself::GetInstance().Run();
	DxLib_End();
	return 0;
}

