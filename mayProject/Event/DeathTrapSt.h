#pragma once

class Event;
class Player;
class GameScene;
class MouseCtl;

struct DeathTrapSt
{
	DeathTrapSt();
	~DeathTrapSt();
	void Update(Event& eve, GameScene& game, Player& player, MouseCtl& mouse);
	void Draw(Event& eve);
	float exr;				// �g��/�k����
	VECTOR2 choicesPngSize;	// �I�����摜�̃T�C�Y
	VECTOR2 offsetPos;		// �\���ʒu�����p
};