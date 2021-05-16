#pragma once
#include <map>
#include <string>
#include <memory>
#include "BaseScene.h"

class MouseCtl;

class GameClearScene :
	public BaseScene
{
public:
	GameClearScene();
	~GameClearScene();
	unique_Base Update(unique_Base own, const GameCtl& ctl) override;
private:
	bool Init(void) override;			// ������
	void PngInit(void);					// �摜�p������
	void Draw(void) override;			// �`��
	void Particle(void);				// �p�[�e�B�N���p�֐�

	std::map<std::string, int> _drawHandle;	// �`��摜

	VECTOR2 _btnPos;					// �{�^���̕`��ʒu
	VECTOR2 _btnSize;					// �{�^���̑傫��
	int _pngBlend;						// ���Z�u�����h�Ŏg�p
	VECTOR2 _parOffset;					// �p�[�e�B�N���̕`��ʒu�I�t�Z�b�g

	int _seClick;						// �N���b�N��
	int _clearBGM;						// BGM
	bool _seFlg;						// SE�p�t���O

	std::unique_ptr<MouseCtl> _mouse;	// �}�E�X
};