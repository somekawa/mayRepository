#pragma once

class Event;
class Player;
class GameScene;
class MouseCtl;

struct DrinkSt
{
	DrinkSt();
	~DrinkSt();
	void Update(Event& eve, GameScene& game, Player& player, MouseCtl& mouse);
	void Draw(Event& eve, GameScene& game);
	float exr;				// �g��/�k����
	VECTOR2 choicesPngSize;	// �I�����摜�̃T�C�Y
	VECTOR2 offsetPos;		// �\���ʒu�����p
};