#pragma once
#include "VECTOR2.h"

class Event;
class Player;
class GameScene;
class MouseCtl;

struct INNSt
{
	INNSt();
	~INNSt();
	void Update(Event& eve, GameScene& game, Player& player, MouseCtl& mouse);
	void Draw(Event& eve,Player& player);
	float exr;				// �g��/�k����
	VECTOR2 choicesPngSize;	// �I�����摜�̃T�C�Y
	VECTOR2 offsetPos;		// �\���ʒu�����p
	int needMoney;			// ���x���ɂ���ĉ񕜔�p�̒������s���ۂɎg�p����
};