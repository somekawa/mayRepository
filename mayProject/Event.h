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
class Cards;

enum class EVENT_STATE {
	NON,
	ENEMY,		// 敵
	YADO,		// 宿屋
	SYOUNIN,	// 商人
	BUTTON,		// ボタン
	CHEST,		// 宝箱
	DRINK,		// 飲み物
	TRAP,		// 即死トラップ
	EVE_MONS,	// イベントモンスター
	GOAL,		// 出口
	MAX
};

class Event
{
public:
	Event();
	~Event();
	void UpDate(GameScene* game,Player* player, Menu* menu,Item* item, Monster* monster,Cards* cards);
	void Draw(GameScene* game,Player* player, Menu* menu, Item* item);
	void SetEvent(EVENT_STATE state);		// ゲームシーンからイベントを設定する
	void SetFateNum(int num);				// はじめからやり直すのに必要
	void SetReset(void);					// はじめからやり直すときにイベントの状態をリセットする
	bool GetEventMonsFlg(void);				// 特定敵イベントで戦うを選択したか取得する関数
	void SetEventMonsFlg(bool flag);		// はじめからやり直すのに必要
	bool GetEventMonsEndFlg(void);			// 特定敵イベントが終了したかを取得する関数
	void SetEventMonsEncountFlg(bool flag);	// 特定敵イベントの遭遇状態を設定する関数
	bool GetEventMonsEncountFlg(void);		// 特定敵イベントに遭遇したかを取得する関数
	void SetCautionFlg(bool flag);
	bool GetCautionFlg(void);
private:
	void Init(void);						// 初期化
	void pngInit(void);						// 画像関係初期化
	void Enemy(GameScene* game, Player* player, Monster* monster);
	void Yado(GameScene* game, Player* player);
	void Syounin(GameScene* game, Player* player, Menu* menu,Item* item);
	void Button(GameScene* game, Player* player);
	void Chest(GameScene* game, Player* player, Menu* menu, Item* item);
	void Drink(GameScene* game, Player* player);
	void Trap(GameScene* game, Player* player);
	void eventMons(GameScene* game, Monster* monster, Cards* cards);

	EVENT_STATE _event;					// イベント情報用変数

	// 宿屋・商人
	bool _healYadoFlg;					// 回復を宿屋で頼むときにtrue
	bool _nonMoneyFlg;					// 所持金が足りないときにtrue(宿屋と商人で使用)
	bool _buyFlg;						// 買い物をするとしたときにアイテムを表示させるためのフラグ
	bool _chooseFlg;					// 購入をおしたらtrueにする
	int _chooseNum;						// 選択中の商品の番号保存用(場所とかしかわからない)
	ITEM _itemInfo;						// アイテムの情報用
	bool _itemNextPage;					// 商品ページをめくったらtrueになる

	// ボタン・宝箱・飲み物イベントで使用
	bool _pushFlg;						// イベント発動時trueにする
	int _fateNum;						// イベントの内容を保存する変数

	bool _getFlg;						// アイテムをとるボタン
	bool _anounceFlg;					// アイテムがいっぱいのときにお知らせする

	bool _onceFlg = false;
	bool _kyouseiButtleFlg = false;

	// 宝箱関連
	int _chestOpen[4];					// 開けたかどうか
	int _chestBingo[4];					// 中身の当たり/はずれ
	VECTOR2 _chestPos[4];				// 宝箱の位置

	// ボタン
	int _buttonNum;						// 現在地とイベント場所が一致したときにその値を保存する変数
	bool _buttonPush[2];				// 押したかどうか
	VECTOR2 _buttonPos[2];				// ボタンの位置
	bool _buttonEventFlg;				// ボタン押下時にtrueにする

	// 飲み物
	int _drinkNum;						// 現在地とイベント場所が一致したときにその値を保存する変数
	bool _drinking[2];					// 飲んだかどうか
	VECTOR2 _drinkPos[2];				// 飲み物の位置
	bool _drinkEventFlg;				// 飲むことにしたときにtrueにする

	// 即死トラップ
	bool _trapFlg;						// 1度発動させたらそのままtrueにしておく
	bool _nowTrapFlg;					// 発動しながら1度以上死んだ場合にtrue

	// 特定敵
	bool _eventMonsFlg;					// 戦うを選択時にtrue			
	bool _eventMonsEncountFlg;			// 遭遇時にtrue
	bool _eventMonsEndFlg;				// 即死トラップ使用して倒した際にtrue

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
	int _sentakusiPNG[12];
	// 商品ページを移動するための矢印の画像
	int _yajirusiPNG;
	// 空の宝箱
	int _karaPNG;
	// 即死トラップの像
	int _zouPNG;
	// 特定敵画像
	int _eventMonsPNG;

	// SE
	int _soundSE[4];
};