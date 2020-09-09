#pragma once
#include <map>
#include <string>
#include "BaseScene.h"

class MouseCtl;

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

	std::map<std::string, int> _drawHandle;	// �`��摜

	int _pngBlend;						// ���Z�u�����h�Ŏg�p

	int _seClick;						// �N���b�N��
	int _clearBGM;						// BGM
	bool _seFlg;						// SE�p�t���O

	MouseCtl* mouse;
};