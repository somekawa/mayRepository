#include <DxLib.h>
#include "MoveObj.h"

bool MoveObj::operator()(const GameCtl& ctl,int& point, bool& RFlag, bool& LFlag, PL_DIRECTION& dir)
{
	switch (point)
	{
	case 0:	// 直進
		if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_W]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_W]))
		{
			return true;
		}
		break;
	case 1:	// 右曲がり
		if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_D]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_D]))
		{
			return true;
		}
		break;
	case 2:	// 左曲がり
		if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_A]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_A]))
		{
			return true;
		}
		break;
	case 4:	// T字
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
			// 何も処理を行わない
		}
		break;
	case 5:	// トの字(直進と右への道)
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
			// 何も処理を行わない
		}
		break;
	case 6:	// トの字型(直進と左への道)
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
			// 何も処理を行わない
		}
		break;
	default:
		break;
	}
	return false;
}