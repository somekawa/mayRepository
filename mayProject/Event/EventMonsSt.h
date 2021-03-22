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
	float exr;				// 拡大/縮小率
	VECTOR2 choicesPngSize;	// 選択肢画像のサイズ
	VECTOR2 offsetPos;		// 表示位置調整用
};