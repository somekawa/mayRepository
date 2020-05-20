#pragma once
#include <iostream>
#include <utility>
#include <string>
#include "Menu.h"

class GameScene;
class Player;
class Menu;
class Item;
class Monster;

enum class EVENT_STATE {
	NON,
	ENEMY,		// 敵
	YADO,		// 宿屋
	SYOUNIN,	// 商人
	BUTTON,		// ボタン
	CHEST,		// 宝箱
	DRINK,		// 飲み物
	TRAP,		// 即死トラップ
	MAX
};

class Event
{
public:
	Event();
	~Event();
	void UpDate(GameScene* game,Player* player, Menu* menu,Item* item, Monster* monster);
	void Draw(GameScene* game,Player* player, Menu* menu, Item* item);
	void SetEvent(EVENT_STATE state);	// ゲームシーンからイベントを設定する
	int GetNowEvent(void);				// 現在のイベント番号を取得する
	void SetNowEvent(int num);			// はじめからやり直すのに必要
	void SetFateNum(int num);			// はじめからやり直すのに必要
	void SetReset(void);			// 初めからやり直すときに宝箱の状態をリセットする
private:
	void Init(void);					// 初期化
	void pngInit(void);					// 画像関係初期化
	void Enemy(GameScene* game, Player* player, Monster* monster);
	void Yado(GameScene* game, Player* player);
	void Syounin(GameScene* game, Player* player, Menu* menu,Item* item);
	void Button(GameScene* game, Player* player);
	void Chest(GameScene* game, Player* player, Menu* menu, Item* item);
	void Drink(GameScene* game, Player* player);
	void Trap(GameScene* game, Player* player);

	EVENT_STATE _event;					// イベント情報用変数
	bool _healYadoFlg;					// 回復を宿屋で頼むときにtrue
	bool _nonMoneyFlg;					// 所持金が足りないときにtrue(宿屋と商人で使用)

	int _nowEvent;						// 現在のイベント

	bool _buyFlg;						// 買い物をするとしたときにアイテムを表示させるためのフラグ
	bool _chooseFlg;					// 購入をおしたらtrueにする
	int _chooseNum;						// 選択中の商品の番号保存用(場所とかしかわからない)
	ITEM _itemInfo;						// アイテムの情報用

	// ボタン・宝箱・飲み物イベントで使用
	bool _pushFlg;						// ボタンを押したらtrueにする
	int _fateNum;						// 0のときは良いことが起こる

	bool _getFlg;						// アイテムをとるボタン
	bool _anounceFlg;					// アイテムがいっぱいとのお知らせ

	bool _itemNextPage = false;

	// 宿屋の画像
	int _healHumanPNG;
	// 商人の画像
	int _syouninPNG;
	// メッセージ用枠画像
	int _messagePNG;
	// 商人の持ち物の後ろ枠
	int _syouninWakuPNG;
	// アイテムボックスの黒枠
	int _itemBoxPNG;
	// 商人の選択中のアイテムがわかりやすくなる画像
	int _itemChoicePNG;
	//「Sold Out」の文字画像
	int _soldOutPNG;
	// 宝箱の画像
	int _chestPNG[2];
	// 宝箱から取得できるアイテム画像
	int _chsetItemPNG;
	// 謎の瓶の画像
	int _drinkPNG;
	// 選択肢の文字画像
	int _sentakusiPNG[11];
	// 矢印の画像
	int yajirusiPNG;
	// 空の宝箱
	int karaPNG;
	// 即死トラップの像
	int zouPNG;

	// SE
	int _soundSE[6];
	int _seCnt;				// 歩行音からドア音までつなぐときに使う変数
	bool _soundWalk;		// 歩行音からドア音までつなぐときに使うフラグ

	int chestOpen[2];		// 開けたかどうか
	int chestBingo[2];		// 中身が当たり
	VECTOR2 chestPos[2];	// 宝箱の位置

	bool trapFlg = false;	// 即死トラップ用。一度発動させたらそのままにしておく
	bool nowTrapFlg = false;// 発動しながら一度以上死んだ場合
};