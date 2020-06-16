#pragma once
#include "BaseScene.h"

class MouseCtl;

class GameOverScene :
	public BaseScene
{
public:
	GameOverScene();
	~GameOverScene();
	// unique_Base own : ������Scene�|�C���^ ,  const GameCtl &ctl : GameCtl�̓ǂݎ���p
	unique_Base Update(unique_Base own, const GameCtl& ctl);	// �Z�[�u/���[�h����
private:
	bool Init(void);		// ������
	void Draw(void);		// �`��
	void PngInit(void);		// �摜�p������

	int _gameOverPNG;		//�uGame Over�v�̕����摜
	int _titleBackPNG;		//�u�^�C�g���ցv�̕����摜
	int _bloodPNG;			// �����摜
	int _backPNG;			// �w�i�摜

	int _pngBlend;			// ���u�����h�Ŏg�p
	
	int _seClick;			// �N���b�N��
	bool _seFlg;			// SE�p�t���O
	int _overBGM;			// BGM

	MouseCtl* mouse;
};