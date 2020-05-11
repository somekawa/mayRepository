#include "DxLib.h"
#include "SceneMng.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	SceneMng::GetInstance().Run();
	DxLib_End();	// DX×²ÌŞ×Ø‚ÌI—¹ˆ—
	return 0;
}