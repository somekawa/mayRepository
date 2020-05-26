#pragma once
#include "BaseScene.h"

 //��Փx�I��(�����̓C�x���g�����Bcsv�̐�����(-1�܂Ŋ܂߂�)�ƈ�v������)
enum class MODE {
	NON = 0,
	NORMAL = 26,
	HARD = 41,
};

class SelectScene :
	public BaseScene
{
public:
	SelectScene();
	~SelectScene();
	// unique_Base own : ������Scene�|�C���^ ,  const GameCtl &ctl : GameCtl�̓ǂݎ���p
	unique_Base Update(unique_Base own, const GameCtl& ctl);	
	static MODE modeTest;
private:
	bool Init(void);					// ������
	void pngInit(void);					// �摜�p������
	void Draw(void);					// �`��

	int _oldMouse;						// 1�t���[���O�̃N���b�N���ۑ��p

	int _titleBackPNG;					//�u�^�C�g���ցv�̕����摜
	int _backPNG;						// �w�i�摜
	int _normalPNG;						//�uNORMAL�v�̕����摜
	int _hardPNG;						//�uHARD�v�̕����摜

	int _pngLight;						// �摜�̖��邳�����p�ϐ�	
	bool _lightFlg;						// �摜�̖��邳�����p�t���O
	bool _toGameFlg;					// GameScene�ւ̈ڍs�p�t���O
	bool _toTitleFlg;					// TitleScene�ւ̈ڍs�p�t���O
	int _seClick;						// �N���b�N��
};