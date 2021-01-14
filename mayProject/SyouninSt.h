#pragma once

class Event;
class Player;
class GameScene;
class MouseCtl;
class Item;
class Menu;

struct SyouninSt
{
	SyouninSt();
	~SyouninSt();
	void Update(Event& eve, GameScene& game, Player& player, MouseCtl& mouse, Item& item, Menu& menu);
	void Draw(Event& eve, Player& player,Item& item,Menu& menu);
	float exr;		// ägëÂ/èkè¨ó¶
};

