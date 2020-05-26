#pragma once
#include <vector>
#include <tuple>
#include "BaseScene.h"
#include "VECTOR2.h"
#include "Cards.h"
#include "Monster.h"
#include "Player.h"
#include "Menu.h"
#include "Item.h"
#include "Event.h"

//#define EVENT_NUM 25

// イベント量の最大値
#define EVENT_MAXNUM 99

// 道画像
enum class MAP {
	STRAIGHT,		// 直進
	RIGHT,			// 右折のみ
	LEFT,			// 左折のみ 
	STOP,			// 行き止まり(イベントもない)
	TJI,			// T字路
	TONOJI_SR,		// ト字型(直進と右への道)
	TONOJI_SL,		// ト字型(直線と左への道)
	YADO,
	SHOUNIN,
	BUTTON,
	CHEST,
	DRINK,
	TRAP,			// 即死トラップ
	EVE_MONS,		// イベントモンスター(通路画像の予定)
	GOAL,			// ゴール
	MAX
};

// プレイヤーの進行方向
enum class PL_DIRECTION {
	UP,
	DOWN,
	LEFT,
	RIGHT,
};

class GameScene :
	public BaseScene
{
public:
	GameScene();
	~GameScene();
	// unique_Base own : 自分のSceneポインタ ,  const GameCtl &ctl : GameCtlの読み取り専用
	unique_Base Update(unique_Base own, const GameCtl& ctl);	// オブジェクトとシーンの管理

	// マウス関係
	VECTOR2 cursorPos;					// マウスカーソルの座標保存用変数
	int mouse;							// マウスの状態
	int mouseOld;

	EVENT_STATE eventState;				// イベントの状態管理用
	bool moveFlg;						// 歩きモーション管理用フラグ
	bool shakeFlg;						// 敵の待機ターンが0になったらtrueにして画面を揺らし始める
	bool blinkFlg;						// 敵の点滅用フラグ
	int walkCnt;						// 歩いた回数
	//int chestFate = -1;
	bool _backFlg = false;
	static int testx;			// プレイヤーの歩行時用変数x
	static int testy;			// プレイヤーの歩行時用変数y
	bool _openDoor;						// 扉を開ける
	static bool monsterFlg;
private:
	bool Init(void);					// 初期化
	void pngInit(void);					// 画像用初期化
	void Draw(void);					// 描画
	void MouseClick_Go(const GameCtl& ctl);		// マウスを左クリックして「進む」を押したときの処理
	void EventUpdate(void);				// イベントのアップデート関数
	void pl_TurnEndAfter(void);			// プレイヤーのターンが終わった後の処理
	void pl_Attack(void);				// プレイヤーの攻撃
	void pl_Heal(void);					// プレイヤーの回復
	void shakeDraw(void);				// 画面を少し揺らす
	void doorWalk(void);				// 足音とドア音のつなぎ
	void changeBGM(void);				// 通常BGMと戦闘BGMの切替
	void plDead(void);					// プレイヤー死亡時
	void enemyItemDrop(void);			// 敵からのアイテムドロップ処理
	void cardEffect(void);				// カードの効果

	Monster* _monster[1];
	Cards* _cards;
	Player* _player;
	Menu* _menu;
	Item* _item;
	Event* _event;

	// 扉関係
	//bool doorFlg;						// 拡大していない扉 表示管理用フラグ
	//bool _openDoor;						// 扉を開ける
	float _doorExpand;					// 扉の拡大用変数
	float _degree;						// 歩きモーション用
	int _doorOpenTiming;				// 歩いてから扉が開くまでの時間調整用
	float _lastDoorExpand;				// クリア前の光る扉の拡大時に使用
	int _lastmoveTime;					// 一定時間以下なら拡大を続ける時に使う
	bool _changeToClear;				// 一定時間以上でフラグがtrueになり、自動的にクリア画面に移行する

	// イベント関係
	//int walkCnt;						// 歩いた回数
	//int _goalCnt;						// ゴールまでに必要な歩数
	int _eventNum[EVENT_MAXNUM];			// イベントが発生する歩数
	int _bossEventNum;					// ボスの出てくるタイミング
	// イベント内容の管理
	int _eventChoiceNum;				// _eventChoiceNumOldの格納場所を変えるのに使う
	int _eventChoiceNumOld[3];			// 過去のイベント情報を3回分保存するのに使う

	// 画面を揺らし関係
	bool _shakeChangeFlg;				// 一定値以上左右どちらかに揺れたら反対方向へ揺らすためのフラグ
	float _shakeTime;					// 揺らす時間
	VECTOR2 _shackPos;					// 揺れ幅

	// 点滅関係
	int _blinkCnt;						// 攻撃を食らったときに起こる点滅
	//bool blinkFlg;					// 敵の点滅用フラグ

	// その他
	int _plDeadChangeWinColor;			// プレイヤー死亡時、画面の色を反転させるときに使う
	int _poisonCnt;						// 戦闘中の毒効果の紫色画面の時間(初期値は255より大きい必要がある)
	bool _onceFlg;						// 敵からのドロップアイテム表示に使用と敵の待機ターンが0の時に使用
	bool _anounceFlg;					// 敵からのドロップアイテムで持ち物がいっぱいの時に案内を出す

	//// スキル関係
	//int _skillCharge = 1;				// スキルチャージ
	//int _skillIconPNG;					// スキルアイコン(後で差し替える)
	//int _skillAnnouncePNG;				// スキル使用可能案内
	//bool _skillFlg = false;				// スキルが使用可能になったらtrue

	/*画像関係*/
	// 敵ターンまでのカウント用画像
	int _turnPNG[6];						
	// HPバー画像
	int _hpBarPl;
	int _hpBarEn;
	int _hpBarBack;
	// 進行度ゲージ画像
	int _gaugeBackPNG;
	int _gaugePNG;
	// 部屋(扉開閉込み)の画像
	int _room[3];		
	//「進む」の文字画像
	//int _walkPNG;	
	// 点滅用白画像
	int _whitePNG;
	// 敵情報画像
	int _enemyInfoPNG;
	//「取る」の文字画像
	int _getItemPNG;
	// 死亡時のやりなおすの画像
	int _retryPNG;
	// 死亡時のあきらめるの画像
	int _deadPNG;
	// 即死トラップで死亡時(敵遭遇前)の画像
	int _messageDeathPNG;
	// 即死トラップで死亡時(敵遭遇後)の画像
	int _messageDeathPNG2;
	// ボス手前警告画像
	int bossEmergencyPNG;
	// 霧
	int kiriPNG[2];
	float kiri1 = 0.0f;
	float kiri2 = -900.0f;


	// ダンジョン
	// 直進
	//int straightPNG;
	// 右折のみ
	//int rightPNG;
	// 左折のみ
	//int leftPNG;
	// 道(今は直進と右折のみ)
	int roadPNG[static_cast<int>(MAP::MAX)];
	int chipPNG[static_cast<int>(MAP::MAX)];

	/*音関係*/
	// SE関連
	int _soundSE[8];
	int _seCnt;		// 歩き音からドア音までの鳴る間隔
	bool _soundWalk;	// 歩き音からドア音につなぐときに必要なフラグ
	// BGM関連
	int _gameBGM;
	int _battleBGM;

	int testCnt = 0;

	//int num[3][3];		// マップ的なもの
	std::pair<bool, int> numkai[10][10];	// マップ(通ったことのあるところはtrue)
	int plNowPoint;		// プレイヤーの現在地
	int plOldPoint;
	//static int testx;			// プレイヤーの歩行時用変数x
	//static int testy;			// プレイヤーの歩行時用変数y
	//std::map<bool, std::pair<int, int>> mapTest;
	//std::map<int, int> mapTest;
	//VECTOR2 movePos[4];		// マップ移動軌跡
	std::vector<std::tuple<VECTOR2, int,float>> vec;	// マップ移動軌跡(位置とミニマップ用に現在の番号に画像回転)
	void TestDirect(void);
	void TestKey(void);
	PL_DIRECTION _plDirect = PL_DIRECTION::UP;
	PL_DIRECTION _plDirectOld = PL_DIRECTION::UP;
	bool leftFlg = false;
	bool rightFlg = false;
	int directPNG;
	float directRota = 0.0f;
	int monsTimeCnt = 3;

	VECTOR2 offset = {0,0};
	VECTOR2 plNowMark = { 0,0 };

	VECTOR2 bossPos = { 8,5 };
	//VECTOR2 bossPos = { 0,2 };
};