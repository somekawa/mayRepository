#pragma once
#include <map>
#include <string>
#include <memory>
#include "BaseScene.h"
#include "VECTOR2.h"

class MouseCtl;

class TitleScene :
	public BaseScene
{
public:
	TitleScene();
	~TitleScene();
	// unique_Base own : ������Scene�|�C���^ ,  const GameCtl &ctl : GameCtl�̓ǂݎ���p
	unique_Base Update(unique_Base own, const GameCtl& ctl);	// �Z�[�u/���[�h����
	static int titleBGM;				// BGM
private:
	bool Init(void);					// ������
	void PngInit(void);					// �摜�p������
	void Draw(void);					// �`��

	std::map<std::string, int> _drawHandle;	// �`��摜

	VECTOR2 _startPngSize;				// START�����摜�̃T�C�Y
	int _pngLight;						// �摜�̖��邳�����p�ϐ�	
	bool _lightFlg;						// �摜�̖��邳�����p�t���O

	int _seClick;						// �N���b�N��
	bool _seFlg;						// SE�p�t���O

	std::unique_ptr<MouseCtl> _mouse;	// �}�E�X
};