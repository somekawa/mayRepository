#pragma once
#include "BaseScene.h"
class GameClearScene :
	public BaseScene
{
public:
	GameClearScene();
	~GameClearScene();
	// unique_Base own : ������Scene�|�C���^ ,  const GameCtl &ctl : GameCtl�̓ǂݎ���p
	unique_Base Update(unique_Base own, const GameCtl& ctl);	// �Z�[�u/���[�h����
private:
	bool Init(void);					// ������
	void Draw(void);					// �`��

	int _gameClearPNG;					//�uGame Clear�v�̕����摜
	int _backTitleButtonPNG;			//�u�^�C�g���ցv�̕����摜
	int _backPNG;						// �w�i�摜

	int _pngBlend;						// ���Z�u�����h�Ŏg�p

	int _seClick;						// �N���b�N��
	int _clearBGM;						// BGM

	bool flag = false;

	//int oldMouse;
};