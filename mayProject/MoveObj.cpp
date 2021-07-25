#include <DxLib.h>
#include "MoveObj.h"

bool MoveObj::operator()(const GameCtl& ctl,int& point, bool& RFlag, bool& LFlag, PL_DIRECTION& dir)
{
	switch (point)
	{
	case 0:	// ���i
		if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_W]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_W]))
		{
			return true;
		}
		break;
	case 1:	// �E�Ȃ���
		if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_D]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_D]))
		{
			return true;
		}
		break;
	case 2:	// ���Ȃ���
		if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_A]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_A]))
		{
			return true;
		}
		break;
	case 4:	// T��
		RFlag = false;
		LFlag = false;
		if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_D]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_D]))
		{
			if (dir == PL_DIRECTION::RIGHT || dir == PL_DIRECTION::UP || dir == PL_DIRECTION::DOWN)
			{
				RFlag = true;
				return true;
			}
			else if (dir == PL_DIRECTION::LEFT)
			{
				LFlag = true;
				return true;
			}
		}
		else if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_A]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_A]))
		{
			if (dir == PL_DIRECTION::RIGHT || dir == PL_DIRECTION::UP || dir == PL_DIRECTION::DOWN)
			{
				LFlag = true;
				return true;
			}
			else if (dir == PL_DIRECTION::LEFT)
			{
				RFlag = true;
				return true;
			}
		}
		else
		{
			// �����������s��Ȃ�
		}
		break;
	case 5:	// �g�̎�(���i�ƉE�ւ̓�)
		RFlag = false;
		LFlag = false;
		if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_W]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_W]))
		{
			return true;
		}
		else if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_D]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_D]))
		{
			RFlag = true;
			return true;
		}
		else
		{
			// �����������s��Ȃ�
		}
		break;
	case 6:	// �g�̎��^(���i�ƍ��ւ̓�)
		RFlag = false;
		LFlag = false;
		if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_W]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_W]))
		{
			return true;
		}
		else if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_A]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_A]))
		{
			LFlag = true;
			return true;
		}
		else
		{
			// �����������s��Ȃ�
		}
		break;
	default:
		break;
	}
	return false;
}