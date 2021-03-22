#include "DxLib.h"
#include "TitleScene.h"
#include "SceneMng.h"
#include "../Common/GameCtl.h"

SceneMng::SceneMng() :screen_sizeX(900), screen_sizeY(600)
{
	SysInit();
}

SceneMng::~SceneMng()
{
}

bool SceneMng::SysInit(void)
{
	// ���я���
	SetGraphMode(screen_sizeX, screen_sizeY, 16);		// 65536�FӰ�ނɐݒ�
	ChangeWindowMode(true);								// true:window�@false:�ٽ�ذ�
	SetWindowText("CARDS MAZE");						// �Q�[���^�C�g��
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