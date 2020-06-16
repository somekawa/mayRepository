#pragma once
#include "BaseScene.h"
#include "VECTOR2.h"

class MouseCtl;

 //��Փx�I��
enum class MODE {
	NON ,
	NORMAL,
	HARD,
};

// ���Ղ̐i�s����
enum class ASIDIR {
	UP,
	DOWN,
	RIGHT,
	LEFT
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
	static bool pushFlg;				// ��Փx�I���{�^���������ꂽ��true�ɂ���
private:
	bool Init(void);					// ������
	void pngInit(void);					// �摜�p������
	void Draw(void);					// �`��

	int _titleBackPNG;					//�u�^�C�g���ցv�̕����摜
	int _backPNG;						// �w�i�摜
	int _normalPNG;						//�uNORMAL�v�̕����摜
	int _hardPNG;						//�uHARD�v�̕����摜

	int _pngLight;						// �摜�̖��邳�����p�ϐ�	
	bool _lightFlg;						// �摜�̖��邳�����p�t���O
	bool _toGameFlg;					// GameScene�ւ̈ڍs�p�t���O
	bool _toTitleFlg;					// TitleScene�ւ̈ڍs�p�t���O
	int _seClick;						// �N���b�N��

	// ����
	int _asiato[2];						// �摜
	VECTOR2 _startPos;					// �i�s�����ɑ΂���X�^�[�g�n�_���W
	VECTOR2 _goalPos;					// �i�s�����ɑ΂���S�[���n�_���W
	VECTOR2 _drawAsiVec;				// �摜�`����W
	bool _asiatoFlg;					// 2�b���Ƃɍ��E�̑��ՂŐ؂�ւ���
	bool _asiatoReverseX;				// �摜X�����]�p
	bool _asiatoReverseY;				// �摜Y�����]�p
	bool _dirFlg;						// �S�[���n�_�ɂ����Ƃ��Ɏ��̕��������߂�
	float _asiatoRota;					// �摜��]
	ASIDIR _dir;						// ���݂̐i�s����
	ASIDIR _olddir;						// 1�O�̐i�s����

	MouseCtl* mouse;
};