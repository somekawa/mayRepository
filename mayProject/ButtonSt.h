#pragma once
#include "VECTOR2.h"

class Event;
class Player;
class GameScene;
class MouseCtl;

struct ButtonSt
{
	ButtonSt();
	~ButtonSt();
	void Update(Event& eve, GameScene& game, Player& player, MouseCtl& mouse);
	void Draw(Event& eve, GameScene& game);
	float exr;				// �g��/�k����
	int getMoney;			// ����ł�����z
	VECTOR2 choicesPngSize;	// �I�����摜�̃T�C�Y
	VECTOR2 offsetPos;		// �\���ʒu�����p
};