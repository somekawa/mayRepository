#pragma once

// プレイヤーの状態
enum class CONDITION {
	FINE,	// 健康
	POISON	// 毒
};

class Monster;
class Menu;
class GameScene;

class Player
{
public:
	Player();
	~Player();
	void ClickUpDate(Monster* monster, Menu* menu, GameScene* game);	// クリック時のみのアップデート関数
	void UpDate(void);					// 通常アップデート関数
	void Draw(Menu* menu);					// 描画
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
	void SetSkillCharge(void);			// スキルチャージ時間を減らす
	int GetSkillCharge(void);			// スキルチャージ時間を取得
private:
	void Init(void);					// 初期化
	void pngInit(void);					// 画像初期化
	// スキル関係
	int _skillCharge;					// スキルチャージ
	bool _skillFlg;						// スキルが使用可能になったらtrue
	bool _skillBackFlg;					// スキルアイコン押下時true
	int _skillIconPNG;					// スキルアイコン(後で差し替える)
	int _skillAnnouncePNG;				// スキル使用可能案内
	int _skillBackPNG;					// スキル用背景画像
	int _skillAttackIconPNG;			// 攻撃系スキル選択用アイコン
	int _skillCancelPNG;				//「やめる」の文字画像
	int _skillMuscleIconPNG;			// 力こぶのアイコン画像
	int _pngLight;						// アイコンの明るさ調整用変数	
	bool _lightFlg;						// アイコンの明るさ調整用フラグ
};