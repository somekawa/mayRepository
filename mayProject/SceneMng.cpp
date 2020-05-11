#include "DxLib.h"
#include "TitleScene.h"
#include "SceneMng.h"
#include "GameCtl.h"

#define SCREEN_SIZE_X 600					// ��ʉ𑜓x ��
#define SCREEN_SIZE_Y 500					// ��ʉ𑜓x �c

SceneMng::SceneMng()
{
	SysInit();
}

SceneMng::~SceneMng()
{
}

bool SceneMng::SysInit(void)
{
	// ���я���
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 16);		// 65536�FӰ�ނɐݒ�
	ChangeWindowMode(true);								// true:window�@false:�ٽ�ذ�
	SetWindowText("5������");					// �w�Дԍ��Ɩ��O�����Ă���
	if (DxLib_Init() == -1) return false;				// DXײ���؏���������	
	SetDrawScreen(DX_SCREEN_BACK);						// �ЂƂ܂��ޯ��ޯ̧�ɕ`��
	gameCtl = std::make_unique<GameCtl>();				// GameCtl�̃C���X�^���X
	return true;
}

void SceneMng::Run(void)
{
	// �֐��̃|�C���^�o�R�ŃA�N�Z�X
	// �Q�[�����[�v
	activeScene = std::make_unique<TitleScene>();
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// gameCtl���p���ĂȂ�
		gameCtl->Update();
		activeScene = activeScene->Update(std::move(activeScene), *gameCtl);
	}
}