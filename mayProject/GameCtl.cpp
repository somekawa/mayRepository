#include "DxLib.h"
#include "GameCtl.h"

GameCtl::GameCtl()
{
}

GameCtl::~GameCtl()
{
}

const KEY_ARRAY & GameCtl::GetCtl(KEY_TYPE type) const // ������const�͒��ɏ����Ă���̂����ׂ�const�Ƃ����Ӗ�
{
	if (type == KEY_TYPE_OLD)
	{
		return dataOld;
	}
	return data;					// �Q�ƂȂ̂ŁA���̂܂ܕԂ��đ��v�B
}

bool GameCtl::Update(void)
{
	dataOld = data;					// �I�y���[�^�̃I�[�o�[���[�h����Ă���
	GetHitKeyStateAll(&data[0]);	// data�̐擪�ɃA�N�Z�X������
	return true;
}