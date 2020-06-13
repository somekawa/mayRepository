#pragma once
#include "BaseScene.h"

class GameClearScene :
	public BaseScene
{
public:
	GameClearScene();
	~GameClearScene();
	// unique_Base own : ������Scene�|�C���^ ,  const GameCtl &ctl : GameCtl�̓ǂݎ���p
	unique_Base Update(unique_Base own, const GameCtl& ctl);
private:
	bool Init(void);					// ������
	void PngInit(void);					// �摜�p������
	void Draw(void);					// �`��
	void Particle(void);				// �p�[�e�B�N���p�֐�

	int _gameClearPNG;					//�uGame Clear�v�̕����摜
	int _backTitleButtonPNG;			//�u�^�C�g���ցv�̕����摜
	int _backPNG;						// �w�i�摜
	int _whitePNG;						// ���摜
	int _particlePNG;					// �p�[�e�B�N���摜

	int _pngBlend;						// ���Z�u�����h�Ŏg�p

	int _seClick;						// �N���b�N��
	int _clearBGM;						// BGM
	bool _seFlg;						// SE�p�t���O
};