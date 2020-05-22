#pragma once
#include "VECTOR2.h"
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

	int whitePNG;
	int partestPNG;

	void parTest(void);
	VECTOR2 parPos[3];
	float parEx[3];
	bool parExFlg[3];
	int _pngLight = 50;
	bool posRandFlg[3];

	//int oldMouse;
};