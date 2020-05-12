#pragma once

// プレイヤーの状態
enum class CONDITION {
	FINE,	// 健康
	POISON	// 毒
};

class Player
{
public:
	Player();
	~Player();
	void SetHP(int hpNum);				// 現在の体力を設定する
	int GetHP(void);					// 現在の体力を取得する
	void SetMaxHP(int hpNum);			// 最大体力を設定する(体力増加剤で必要)
	int GetMaxHP(void);					// 最大体力を取得する
	float GetHPBar(void);				// 体力バー用の値計算
	int GetAttackDamage(void);			// 攻撃力を取得する
	void SetDifense(int num);			// 防御力を設定する(イベントで必要)
	int GetDifense(void);				// 防御力を取得する
	void SetNextLevel(int num);			// 次のレベルまでに必要な経験値を設定する
	int GetNextLevel(void);				// 次のレベルまでに必要な経験値を取得する
	int GetNowLevel(void);				// 現在のレベルを取得する
	void SetMoney(int num);				// 所持金を設定する
	int GetMoney(void);					// 現在の所持金を取得する
	void SetCondition(CONDITION con);	// 現在の状態を設定する
	CONDITION GetCondition(void);		// 現在の状態を取得する
	void SetConditionTurn(int num);		// 状態異常からの復帰時間を設定する
	int GetConditionTurn(void);			// 状態異常からの復帰時間を取得する
private:
	//int _plHP;						// 現在の体力
	//int _conditionTurnNum;				// 状態異常の時間
	//int _nowNum;						// 今のプレイヤーステータスの番号(レベルアップで更新する)
	//int _money;						// 所持金(敵を倒すと手に入り、物を購入すると減る)
};