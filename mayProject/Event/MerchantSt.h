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
	float exr;				// 拡大/縮小率
	VECTOR2 choicesPngSize;	// 選択肢画像のサイズ
	VECTOR2 offsetPos;		// 表示位置調整用
};