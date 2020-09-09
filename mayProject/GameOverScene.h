#pragma once
#include <map>
#include <string>
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

	std::map<std::string, int> _drawHandle;	// �`��摜

	int _pngBlend;			// ���u�����h�Ŏg�p
	
	int _seClick;			// �N���b�N��
	bool _seFlg;			// SE�p�t���O
	int _overBGM;			// BGM

	MouseCtl* mouse;
};