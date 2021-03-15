#pragma once
#include <map>
#include <string>
#include <memory>
#include <functional>
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
enum class FOOTDIR {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class SelectScene :
	public BaseScene
{
public:
	SelectScene();
	~SelectScene();
	unique_Base Update(unique_Base own, const GameCtl& ctl);	
	static MODE modeSelect;				// ��Փx�I��
	static bool pushFlg;				// ��Փx�I���{�^���������ꂽ��true�ɂ���
private:
	bool Init(void);					// ������
	void pngInit(void);					// �摜�p������
	void Draw(void);					// �`��

	std::map<std::string, int> _drawHandle;	// �`��摜
	VECTOR2 _modePngSize;					// ��Փx�̕����T�C�Y
	VECTOR2 _toTitlePngSize;				// �^�C�g���֖߂�̕����T�C�Y

	int _pngLight;						// �摜�̖��邳�����p�ϐ�	
	bool _lightFlg;						// �摜�̖��邳�����p�t���O
	bool _toGameFlg;					// GameScene�ւ̈ڍs�p�t���O
	bool _toTitleFlg;					// TitleScene�ւ̈ڍs�p�t���O
	int _seClick;						// �N���b�N��

	// ����
	int _footPrints[2];					// �摜
	int _footSpeed;						// �ړ����x
	VECTOR2 _startPos;					// �i�s�����ɑ΂���X�^�[�g�n�_���W
	VECTOR2 _goalPos;					// �i�s�����ɑ΂���S�[���n�_���W
	VECTOR2 _drawFootVec;				// �摜�`����W
	bool _footPrintsFlg;				// 2�b���Ƃɍ��E�̑��ՂŐ؂�ւ���
	bool _footPrintsReverseX;			// �摜X�����]�p
	bool _footPrintsReverseY;			// �摜Y�����]�p
	bool _dirFlg;						// �S�[���n�_�ɂ����Ƃ��Ɏ��̕��������߂�
	float _footPrintsRota;				// �摜��]
	FOOTDIR _dir;						// ���݂̐i�s����
	FOOTDIR _olddir;					// 1�O�̐i�s����

	std::map<FOOTDIR, std::function<void(void)>> func_;

	std::unique_ptr<MouseCtl> _mouse;	// �}�E�X
};