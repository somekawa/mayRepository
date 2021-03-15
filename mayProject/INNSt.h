#pragma once
#include "VECTOR2.h"

class Event;
class Player;
class GameScene;
class MouseCtl;

struct INNSt
{
	INNSt();
	~INNSt();
	void Update(Event& eve, GameScene& game, Player& player, MouseCtl& mouse);
	void Draw(Event& eve,Player& player);
	float exr;				// 拡大/縮小率
	VECTOR2 choicesPngSize;	// 選択肢画像のサイズ
	VECTOR2 offsetPos;		// 表示位置調整用
	int needMoney;			// レベルによって回復費用の調整を行う際に使用する
};