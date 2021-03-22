#pragma once
#include "VECTOR2.h"

class MouseCtl
{
public:
	MouseCtl();
	~MouseCtl();

	bool GetClickTrg(void);			// �N���b�N�����u�Ԃ̏ꍇtrue
	bool GetClicking(void);			// �N���b�N���̏ꍇtrue
	const VECTOR2& GetPos(void);	// �N���b�N�ʒu�̎擾
	void UpDate(void);				// �}�E�X�̏�ԍX�V
private:
	VECTOR2 _pos;					// �}�E�X���W
	char _mouseData;				// ���݂̃}�E�X�̃N���b�N���
	char _mouseDataOld;				// 1�t���[���O�̃}�E�X�̃N���b�N���
};