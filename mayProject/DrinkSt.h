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
	float exr;				// 拡大/縮小率
	VECTOR2 choicesPngSize;	// 選択肢画像のサイズ
	VECTOR2 offsetPos;		// 表示位置調整用
};