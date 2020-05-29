#pragma once
#include "Monster.h"
#include "ITEMCLASS.h"

#define MONSTER_CNT 6

class Cards;

class Enemy_weak :
	public Monster
{
public:
	Enemy_weak();
	~Enemy_weak();
	bool Init(void);					// 初期化
	void pngInit(void);					// 画像初期化
	void update(void);					// アップデート関数
	void Draw(void);					// 描画(雑魚敵とドロップアイテム)
	void BossDraw(void);				// 雑魚敵と描画する画像の大きさが違うので別関数
	void Damage(int damageNum,Cards* cards);	// プレイヤーから敵へのダメージ用関数
	ENEMY_STATE GetEnemyState(void);	// 現在の敵の状態を取得する関数
	void SetEnemyState(ENEMY_STATE st);	// イベント状態をNONに戻したいときに使う関数
	int GetAttack(void);				// 敵の攻撃力の取得用関数
	int GetMaxTurn(void);				// 最大待機ターン数の取得用関数
	void SetEnemyNum(int num, int plLv);// エンカウントしたときに敵の情報を設定する(プレイヤーレベルに合わせて変化)
	float GetHPBar(void);				// HPバー用の値計算用
	int GetKeikenti(void);				// 倒されたときに経験値を取得する関数
	int GetMoney(void);					// 倒されたときにお金を取得する関数
	bool GetDropFlg(void);				// ドロップアイテムを取得する関数
	void SetDropFlg(bool flag);			// ドロップアイテムの設定する関数
	ITEM GetDrop(void);					// ドロップアイテムの種類を取得する関数
	int GetEnemyNum(void);				// 敵の番号を取得する関数
private:
	int _enemyHP;						// 現在HP
	int _enemyMaxHP;					// 最大HP(通常の体力+プレイヤーレベルでの補正)
	int _enemyMaxTurn;					// 最大待機ターン数
	int _enemyNum;						// 敵番号保存用
	int _attackDamage;					// 敵の攻撃力保存用(通常の攻撃力+プレイヤーレベルでの補正)
	ENEMY_STATE _enemyState;			// 現在の敵の状態設定用

	int _bossFogCnt;					// ボスのスモーク調整用変数

	// ドロップアイテム関係
	bool _dropItem;						// ドロップアイテム用フラグ
	int _dropItemNum;					// ドロップアイテムの画像番号
	ITEM _dropItemSyurui;				// ドロップアイテムの種類

	int _se;							// SE

	// 画像関係
	int _enemyPNG[MONSTER_CNT];			// 雑魚画像
	int _bossPNG;						// ボス画像
	int _eneItemPNG[4];					// ドロップアイテム画像
	int fogPNG;						// ボス用スモーク画像
};