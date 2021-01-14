#pragma once
#include <map>
#include <string>

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
class MouseCtl;

class Player
{
public:
	Player();
	~Player();
	void ClickUpDate(Monster* monster, Menu* menu, GameScene* game, Cards* cards);	// クリック時のみのアップデート関数
	void UpDate(void);						// 通常アップデート関数
	void Draw(Menu* menu);					// 描画
	void BattleDraw(Menu* menu);			// 戦闘中のみ必要な画像の描画
	void SkillDraw(void);					// スキル描画
	void SetHP(const int& hpNum);			// 現在の体力を設定する
	int GetHP(void)const;					// 現在の体力を取得する
	void SetMaxHP(const int& hpNum);		// 最大体力を設定する(体力増加剤で必要)
	int GetMaxHP(void)const;				// 最大体力を取得する
	float GetHPBar(void)const;				// 体力バー用の値計算
	int GetAttackDamage(void)const;			// 攻撃力を取得する
	void SetDifense(const int& num);		// 防御力を設定する(イベントで必要)
	int GetDifense(void)const;				// 防御力を取得する
	void SetNextLevel(const int& num);		// 次のレベルまでに必要な経験値を設定する
	int GetNextLevel(void)const;			// 次のレベルまでに必要な経験値を取得する
	int GetNowLevel(void)const;				// 現在のレベルを取得する
	void SetMoney(const int& num);			// 所持金を設定する
	int GetMoney(void)const;				// 現在の所持金を取得する
	void SetCondition(const CONDITION& con);// 現在の状態を設定する
	CONDITION GetCondition(void)const;		// 現在の状態を取得する
	void SetConditionTurn(const int& num);	// 状態異常からの復帰時間を設定する
	int GetConditionTurn(void)const;		// 状態異常からの復帰時間を取得する
	void SetSkillCharge(const int& num);	// スキルチャージ時間を減らす
	int GetSkillCharge(void)const;			// スキルチャージ時間を取得
	bool GetSkillBackFlg(void)const;		// スキル選択画面が表示されているかを取得する
	void SetBarrierNum(const int& num);		// スキルのバリア値を設定
	int GetBarrierNum(void)const;			// スキルのバリア値を取得
	void SetLevelUpAnounceFlg(const bool& flag);// レベルが上がったことを通知するフラグを設定する
	bool GetLevelUpAnounceFlg(void)const;	 // レベルが上がったことを通知するフラグを取得する

	static int saveData[9];				// ロードで始めるときに読み込んだデータを保存する配列
	static bool _loadFlg;				// ロードで始めるときにinitでステータスを読み込まないようにするとき必要
private:
	void Init(void);					// 初期化
	void PngInit(void);					// 画像初期化

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

	// スキル画像関係
	std::map<std::string, int> skillImages;
	std::map<std::string, int> _drawHandle;

	// アニメーション関係
	SKILL _skill = SKILL::NON;
	int _skillAnimSword[12];
	int _skillAnimGuard[10];
	int _skillAnimHeal[10];
	int _animCnt;						// アニメーションカウンタ
	int _animUpDateSpeedCnt;			// アニメーション更新速度調整カウンタ

	int _soundSE[6];					// SE

	MouseCtl* mouse;
};