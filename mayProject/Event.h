#pragma once
#include <iostream>
#include <utility>
#include <map>
#include <string>
#include "Menu.h"

class GameScene;
class MouseCtl;
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
	void UpDate(GameScene* game,Player* player, Menu* menu,Item* item, Monster* monster,Cards* cards, MouseCtl* mouse);
	void Draw(GameScene* game,Player* player, Menu* menu, Item* item);
	void SetEvent(const EVENT_STATE& state);// ゲームシーンからイベントを設定する
	void SetFateNum(const int& num);		// はじめからやり直すのに必要
	void SetReset(void);					// はじめからやり直すときにイベントの状態をリセットする
	bool GetEventMonsFlg(void)const;		// 特定敵イベントで戦うを選択したか取得する関数
	void SetEventMonsFlg(const bool& flag);	// はじめからやり直すのに必要
	bool GetEventMonsEndFlg(void)const;		// 特定敵イベントが終了したかを取得する関数
	void SetEventMonsEncountFlg(const bool& flag);	// 特定敵イベントの遭遇状態を設定する関数
	bool GetEventMonsEncountFlg(void)const;	// 特定敵イベントに遭遇したかを取得する関数
	void SetCautionFlg(const bool& flag);	// 強制戦闘の案内を設定する関数
	bool GetCautionFlg(void)const;			// 強制戦闘の案内を取得する関数
private:
	void Init(void);						// 初期化
	void pngInit(void);						// 画像関係初期化
	void Enemy(GameScene* game, Player* player, Monster* monster);
	void Yado(GameScene* game, Player* player,MouseCtl* mouse);
	void Syounin(GameScene* game, Player* player, Menu* menu,Item* item, MouseCtl* mouse);
	void Button(GameScene* game, Player* player, MouseCtl* mouse);
	void Chest(GameScene* game, Player* player, Menu* menu, Item* item, MouseCtl* mouse);
	void Drink(GameScene* game, Player* player, MouseCtl* mouse);
	void Trap(GameScene* game, Player* player, MouseCtl* mouse);
	void eventMons(GameScene* game, Monster* monster, Cards* cards, MouseCtl* mouse);

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

	bool _onceFlg;						// 強制戦闘時に敵情報を描画するときに使用
	bool _kyouseiButtleFlg;				// 強制戦闘案内画像の描画に使用

	// 宝箱関連
	int _chestOpen[4];					// 開けたかどうか
	int _chestBingo[4];					// 中身の当たり(1)/はずれ(0)
	VECTOR2 _chestPos[4];				// 宝箱の位置

	// ボタン
	VECTOR2 _buttonDrink[4];			// ボタンとドリンクの座標
	int _buttonNum;						// 現在地とイベント場所が一致したときにその値を保存する変数
	bool _buttonPush[2];				// 押したかどうか
	bool _buttonEventFlg;				// ボタン押下時にtrueにする

	// 飲み物
	int _drinkNum;						// 現在地とイベント場所が一致したときにその値を保存する変数
	bool _drinking[2];					// 飲んだかどうか
	bool _drinkEventFlg;				// 飲むことにしたときにtrueにする

	// 即死トラップ
	bool _trapFlg;						// 1度発動させたらそのままtrueにしておく
	bool _nowTrapFlg;					// 発動しながら1度以上死んだ場合にtrue

	// 特定敵
	bool _eventMonsFlg;					// 戦うを選択時にtrue			
	bool _eventMonsEncountFlg;			// 遭遇時にtrue
	bool _eventMonsEndFlg;				// 即死トラップ使用して倒した際にtrue

	std::map<int, ITEM> _chestItemMap;

	// イベント画像
	std::map<std::string, int> eventImages;
	// メッセージ用枠画像
	int _messagePNG;
	// 商人の持ち物の後ろ枠
	int _syouninWakuPNG;
	// アイテムボックスの黒枠
	int _itemBoxPNG;
	// 商人の選択中のアイテムがわかりやすくなる画像
	int _itemChoicePNG;
	// [0]:開いてない宝箱,[1]:宝箱から敵出現
	int _chestPNG[2];
	// 選択肢の文字画像
	int _sentakusiPNG[12];
	// 商品ページを移動するための矢印の画像
	int _yajirusiPNG;
	// SE
	int _soundSE[4];
};