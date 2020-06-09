#pragma once
#include "GameCtl.h"
#include "GameScene.h"

struct MoveObj
{
	bool operator()(const GameCtl& ctl, int& point,bool& RFlag, bool& LFlag, PL_DIRECTION& dir);
};