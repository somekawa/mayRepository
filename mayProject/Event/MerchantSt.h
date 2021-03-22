#pragma once

class Event;
class Player;
class GameScene;
class MouseCtl;
class Item;
class Menu;

struct MerchantSt
{
	MerchantSt();
	~MerchantSt();
	void Update(Event& eve, GameScene& game, Player& player, MouseCtl& mouse, Item& item, Menu& menu);
	void Draw(Event& eve, Player& player,Item& item,Menu& menu);
	float exr;				// �g��/�k����
	VECTOR2 choicesPngSize;	// �I�����摜�̃T�C�Y
	VECTOR2 offsetPos;		// �\���ʒu�����p
};