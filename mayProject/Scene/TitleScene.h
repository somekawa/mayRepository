#pragma once
#include <map>
#include <string>
#include <memory>
#include "BaseScene.h"
#include "../Common/VECTOR2.h"

class MouseCtl;

class TitleScene :
	public BaseScene
{
public:
	TitleScene();
	~TitleScene();
	unique_Base Update(unique_Base own, const GameCtl& ctl) override;
	static int titleBGM;				// BGM
private:
	bool Init(void) override;			// ������
	void PngInit(void);					// �摜�p������
	void Draw(void) override;			// �`��

	std::map<std::string, int> _drawHandle;	// �`��摜

	VECTOR2 _startPngSize;				// START�����摜�̃T�C�Y
	int _pngLight;						// �摜�̖��邳�����p�ϐ�	
	bool _lightFlg;						// �摜�̖��邳�����p�t���O

	int _seClick;						// �N���b�N��
	bool _seFlg;						// SE�p�t���O

	std::unique_ptr<MouseCtl> _mouse;	// �}�E�X
};