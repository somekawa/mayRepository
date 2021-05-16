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
	// �V�X�e������
	SetGraphMode(screen_sizeX, screen_sizeY, 16);		// 65536�F���[�h�ɐݒ�
	ChangeWindowMode(true);								// true:window�@false:�t���X�N���[��
	SetWindowText("CARDS MAZE");						// �Q�[���^�C�g��

	// DX���C�u��������������
	if (DxLib_Init() == -1)
	{
		return false;
	}				
	SetDrawScreen(DX_SCREEN_BACK);						// �o�b�N�o�b�t�@�ɕ`��
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
		gameCtl->Update();
		activeScene = activeScene->Update(std::move(activeScene), *gameCtl);
	}
}