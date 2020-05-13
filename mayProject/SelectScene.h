#pragma once
#include "BaseScene.h"
class SelectScene :
	public BaseScene
{
public:
	SelectScene();
	~SelectScene();
	// unique_Base own : ������Scene�|�C���^ ,  const GameCtl &ctl : GameCtl�̓ǂݎ���p
	unique_Base Update(unique_Base own, const GameCtl& ctl);	// �Z�[�u/���[�h����
private:
	bool Init(void);					// ������
	void Draw(void);					// �`��

	int _oldMouse;
	int _titleBackPNG;		//�u�^�C�g���ցv�̕����摜
	int _backPNG;			// �w�i�摜
	int _normalPNG;			
	int _hardPNG;

	int _pngLight;						// �摜�̖��邳�����p�ϐ�	
	bool _lightFlg;						// �摜�̖��邳�����p�t���O
};