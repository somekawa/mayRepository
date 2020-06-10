#pragma once

// プレイヤーの状態
enum class CONDITION {
	FINE,	// 健康
	POISON	// 毒
};

enum class SKILL {
	NON,
	SWORD,
	GUARD,
	HEAL
};

class Monster;
class Menu;
class GameScene;
class Cards;

class Player
{
public:
	Player();
	~Player();
	void ClickUpDate(Monster* monster, Menu* menu, GameScene* game, Cards* cards);	// クリック時のみのアップデート関数
	void UpDate(void);					// 通常アップデート関数
	void Draw(Menu* menu);				// 描画
	void BattleDraw(Menu* menu);		// 戦闘中のみ必要な画像の描画
	void SkillDraw(void);				// スキル描画
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
	void SetSkillCharge(int num);		// スキルチャージ時間を減らす
	int GetSkillCharge(void);			// スキルチャージ時間を取得
	bool GetSkillBackFlg(void);			// スキル選択画面が表示されているかを取得する
	void SetBarrierNum(int num);		// スキルのバリア値を設定
	int GetBarrierNum(void);			// スキルのバリア値を取得
	void SetLevelUpAnounceFlg(bool flag);// レベルが上がったことを通知するフラグを設定する
	bool GetLevelUpAnounceFlg(void);	 // レベルが上がったことを通知するフラグを取得する

	static int saveData[9];				// ロードで始めるときに読み込んだデータを保存する配列
	static bool loadFlg;				// ロードで始めるときにinitでステータスを読み込まないようにするとき必要
private:
	void Init(void);					// 初期化
	void pngInit(void);					// 画像初期化

	bool _levelUpAnounceFlg;			// レベルが上がったときにtrueになる

	// スキル関係
	int _skillCharge;					// スキルチャージ
	bool _skillFlg;						// スキルが使用可能になったらtrue
	bool _skillBackFlg;					// スキルアイコン押下時true
	int _pngLight;						// アイコンの明るさ調整用変数	
	bool _lightFlg;						// アイコンの明るさ調整用フラグ
	bool _seSkillOnceFlg;				// スキル発動可能時に一度だけSEを鳴らすときに使用する

	// バリア関係
	int _barrierMaxNum;					// バリアの最大値
	int _barrierNum;					// バリアの値(特定値*プレイヤーレベル)

	// 画像関係
	int _skillIconPNG;					// スキルアイコン
	int _skillAnnouncePNG;				// スキル使用可能案内
	int _skillBackPNG;					// スキル用背景画像
	int _skillAttackIconPNG;			// 攻撃系スキル選択用アイコン
	int _skillBarrierIconPNG;			// 防御系スキル選択用アイコン
	int _skillHealIconPNG;				// 回復系スキル選択用アイコン
	int _skillCancelPNG;				//「やめる」の文字画像
	int _skillMuscleIconPNG;			// 力こぶのアイコン画像
	int _barrierBarBackPNG;				// バリアバーの背景画像
	int _barrierBarPNG;					// バリアバー画像
	// HPバー画像
	int _hpBarPl;
	int _hpBarPlPoison;
	int _hpBarBack;

	// アニメーション関係
	SKILL _skill = SKILL::NON;
	int _skillAnimSword[12];
	int _skillAnimGuard[10];
	int _skillAnimHeal[10];
	int _animCnt;						// アニメーションカウンタ
	int _animUpDateSpeedCnt;			// アニメーション更新速度調整カウンタ

	int _soundSE[6];					// SE
};