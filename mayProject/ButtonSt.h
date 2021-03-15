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
	float exr;				// 拡大/縮小率
	int getMoney;			// 入手できる金額
	VECTOR2 choicesPngSize;	// 選択肢画像のサイズ
	VECTOR2 offsetPos;		// 表示位置調整用
};