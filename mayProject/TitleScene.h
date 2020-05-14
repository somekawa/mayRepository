#pragma once
#include "BaseScene.h"
#include "VECTOR2.h"

class TitleScene :
	public BaseScene
{
public:
	TitleScene();
	~TitleScene();
	// unique_Base own : ������Scene�|�C���^ ,  const GameCtl &ctl : GameCtl�̓ǂݎ���p
	unique_Base Update(unique_Base own, const GameCtl& ctl);	// �Z�[�u/���[�h����
private:
	bool Init(void);					// ������
	void Draw(void);					// �`��

	int _titlePNG;						// �^�C�g���摜
	int _startPNG;						//�uSTART�v�̕����摜

	int _pngLight;						// �摜�̖��邳�����p�ϐ�	
	bool _lightFlg;						// �摜�̖��邳�����p�t���O

	// �Q�[���V�[������^�C�g���֖߂��Ă����Ƃ��Ƀ{�^���̈ʒu�����������ŁA�܂��Q�[���V�[���ɓ���̂�h��
	int _oldMouse;

	int _seClick;						// �N���b�N��
	int _titleBGM;						// BGM
	bool flag = false;
};