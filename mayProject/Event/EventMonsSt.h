#include "../Common/VECTOR2.h"
#pragma once

class Event;
class GameScene;
class Monster;
class Cards;
class MouseCtl;

struct EventMonsSt
{
	EventMonsSt();
	~EventMonsSt();
	void Update(Event& eve, GameScene& game, Monster& monster, Cards& cards, MouseCtl& mouse);
	void Draw(Event& eve);
	float exr;				// �g��/�k����
	VECTOR2 choicesPngSize;	// �I�����摜�̃T�C�Y
	VECTOR2 offsetPos;		// �\���ʒu�����p
};