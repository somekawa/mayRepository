#pragma once
#include "VECTOR2.h"
#include "ITEMCLASS.h"

// 敵の状態
enum class ENEMY_STATE {
	NON,	// いない
	EXIST,	// 存在する
	DEATH	// 存在していたが死亡した
};

class Monster
{
public:
	Monster();
	virtual ~Monster();
	virtual void update(void) = 0;
	virtual bool Init(void) = 0;
	virtual void Draw(void) = 0;
	virtual void BossDraw(void) = 0;
	virtual void Damage(int damageNum) = 0;
	virtual ENEMY_STATE GetEnemyState(void) = 0;
	virtual void SetEnemyState(ENEMY_STATE st) = 0;
	virtual void SetEnemyNum(int num,int plLv) = 0;
	virtual int GetAttack(void) = 0;
	virtual int GetMaxTurn(void) = 0;
	virtual float GetHPBar(void) = 0;	
	virtual int GetKeikenti(void) = 0;
	virtual int GetMoney(void) = 0;
	virtual bool GetDropFlg(void) = 0;
	virtual void SetDropFlg(bool flag) = 0;
	virtual ITEM GetDrop(void) = 0;
};