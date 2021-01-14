#include <DxLib.h>
#include "MouseCtl.h"

MouseCtl::MouseCtl()
{
	_mouseData = 0;
	_mouseDataOld = 0;
}

MouseCtl::~MouseCtl()
{
}

bool MouseCtl::GetClickTrg(void)
{
	//�}�E�X�̍��{�^����������Ă�����
	if (_mouseData & MOUSE_INPUT_LEFT && !(_mouseDataOld & MOUSE_INPUT_LEFT))
	{	
		return true;
	}

	return false;
}

bool MouseCtl::GetClicking(void)
{
	//�}�E�X�̍��{�^������������ςȂ�
	if (_mouseData & MOUSE_INPUT_LEFT && (_mouseDataOld & MOUSE_INPUT_LEFT))
	{
		return true;
	}

	return false;
}

const VECTOR2& MouseCtl::GetPos(void)
{
	return _pos;
}

void MouseCtl::UpDate(void)
{
	// �}�E�X�̍��W�擾
	GetMousePoint(&_pos.x, &_pos.y);

	// 1�t���[���O�̏�������
	_mouseDataOld = _mouseData;

	// �}�E�X�̃N���b�N��Ԏ擾
	_mouseData = GetMouseInput();
}