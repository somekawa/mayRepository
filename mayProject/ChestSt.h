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
};