#pragma once

class Event;
class Player;
class GameScene;
class MouseCtl;
class Item;
class Menu;

struct ChestSt
{
	ChestSt();
	~ChestSt();
	void Update(Event& eve,GameScene& game, Player& player, Menu& menu, Item& item, MouseCtl& mouse);
	void Draw(Event& eve, Menu& menu);
	float exr;				// �g��/�k����
	VECTOR2 choicesPngSize;	// �I�����摜�̃T�C�Y
	VECTOR2 offsetPos;		// �\���ʒu�����p
};