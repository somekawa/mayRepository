#include "DxLib.h"
#include "Scene/SceneMng.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	SceneMng::GetInstance().Run();
	DxLib_End();	// DXײ���؂̏I������
	return 0;
}