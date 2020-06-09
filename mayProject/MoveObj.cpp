#include <DxLib.h>
#include "MoveObj.h"

bool MoveObj::operator()(const GameCtl& ctl,int& point, bool& RFlag, bool& LFlag, PL_DIRECTION& dir)
{
	// ���i
	if (point == 0)
	{
		if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_W]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_W]))
		{
			return true;
		}
	}

	// �E�Ȃ���
	if (point == 1)
	{
		if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_D]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_D]))
		{
			return true;
		}
	}

	// ���Ȃ���
	if (point == 2)
	{
		if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_A]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_A]))
		{
			return true;
		}
	}

	// T��
	if (point == 4)
	{
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
		if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_A]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_A]))
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
	}

	// �g�̎�(���i�ƉE�ւ̓�)
	if (point == 5)
	{
		RFlag = false;
		LFlag = false;
		if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_W]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_W]))
		{
			return true;
		}
		if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_D]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_D]))
		{
			RFlag = true;
			return true;
		}
	}

	// �g�̎��^(���i�ƍ��ւ̓�)
	if (point == 6)
	{
		RFlag = false;
		LFlag = false;
		if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_W]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_W]))
		{
			return true;
		}
		if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_A]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_A]))
		{
			LFlag = true;
			return true;
		}
	}

	return false;
}