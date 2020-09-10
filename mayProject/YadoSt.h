#pragma once

class Event;
class Player;
class GameScene;
class MouseCtl;

struct YadoSt
{
	YadoSt();
	~YadoSt();
	void Update(Event& eve, GameScene& game, Player& player, MouseCtl& mouse);
	void Draw(Event& eve,Player& player);
	float exr;
};