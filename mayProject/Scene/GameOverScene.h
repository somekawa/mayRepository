#pragma once
#include <map>
#include <string>
#include <memory>
#include "BaseScene.h"

class MouseCtl;

class GameOverScene :
	public BaseScene
{
public:
	GameOverScene();
	~GameOverScene();
	unique_Base Update(unique_Base own, const GameCtl& ctl) override;	
private:
	bool Init(void) override;		// ������
	void Draw(void) override;		// �`��
	void PngInit(void);				// �摜�p������

	std::map<std::string, int> _drawHandle;	// �`��摜

	VECTOR2 _btnPos;		// �{�^���̕`��ʒu
	VECTOR2 _btnSize;		// �{�^���̑傫��
	int _pngBlend;			// ���u�����h�Ŏg�p
	
	int _seClick;			// �N���b�N��
	bool _seFlg;			// SE�p�t���O
	int _overBGM;			// BGM

	std::unique_ptr<MouseCtl> _mouse;		// �}�E�X
};