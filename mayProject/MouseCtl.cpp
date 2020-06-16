#include <DxLib.h>
#include "MouseCtl.h"

MouseCtl::MouseCtl()
{
	mouseData = 0;
	mouseDataOld = 0;
}

MouseCtl::~MouseCtl()
{
}

bool MouseCtl::GetClickTrg(void)
{
	//�}�E�X�̍��{�^����������Ă�����
	if (mouseData & MOUSE_INPUT_LEFT && !(mouseDataOld & MOUSE_INPUT_LEFT))
	{	
		return true;
	}

	return false;
}

bool MouseCtl::GetClicking(void)
{
	//�}�E�X�̍��{�^������������ςȂ�
	if (mouseData & MOUSE_INPUT_LEFT && (mouseDataOld & MOUSE_INPUT_LEFT))
	{
		return true;
	}

	return false;
}

const VECTOR2& MouseCtl::GetPos(void)
{
	return pos;
}

void MouseCtl::UpDate(void)
{
	// �}�E�X�̍��W�擾
	GetMousePoint(&pos.x, &pos.y);

	// 1�t���[���O�̏�������
	mouseDataOld = mouseData;

	// �}�E�X�̃N���b�N��Ԏ擾
	mouseData = GetMouseInput();
}