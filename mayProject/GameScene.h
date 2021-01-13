#pragma once
#include <DxLib.h>
#include <map>
#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include "BaseScene.h"
#include "VECTOR2.h"
#include "Cards.h"
#include "Monster.h"
#include "Player.h"
#include "Menu.h"
#include "Item.h"
#include "Event.h"

class MouseCtl;

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
	unique_Base Update(unique_Base own, const GameCtl& ctl);

	// マウス関係
	MouseCtl* mouse;

	EVENT_STATE eventState;				// イベントの状態管理用
	bool moveFlg;						// 歩きモーション管理用フラグ
	bool shakeFlg;						// 敵の待機ターンが0になったらtrueにして画面を揺らし始める
	bool blinkFlg;						// 敵の点滅用フラグ
	bool backFlg;						// 来た道へ戻る処理
	bool openDoor;						// 扉を開ける
	static int plPosX;					// プレイヤーの歩行時用変数x
	static int plPosY;					// プレイヤーの歩行時用変数y
	static bool monsterFlg;				// 敵とのエンカウント用フラグ
	static bool bossClearFlg;			// ボス撃破でtrueにする
private:
	bool Init(void);					// 初期化
	void PngInit(void);					// 画像用初期化
	void Draw(void);					// 描画
	void AllMapDraw(void);				// 全体マップの描画
	void SmallMapDraw(void);			// 部分マップの描画
	void ShakeDraw(void);				// 画面を少し揺らす
	void MouseClick_Go(const GameCtl& ctl);		// 左クリックして「進む」を押したときの処理
	void EventUpdate(void);				// イベントのアップデート関数
	void Pl_TurnEndAfter(void);			// プレイヤーのターンが終わった後の処理
	void Pl_Attack(void);				// プレイヤーの攻撃
	void Pl_Heal(void);					// プレイヤーの回復
	void Pl_Dead(void);					// プレイヤー死亡時
	void Walk(void);					// 足音とキー入力の受付処理をする
	void ChangeBGM(void);				// 通常BGMと戦闘BGMの切替
	void EnemyItemDrop(void);			// 敵からのアイテムドロップ処理
	void CardEffect(void);				// カードの効果
	void DungeonFog(void);				// 霧の効果
	void ButtleCaution(void);			// 強制戦闘の案内を出す
	void GameReset(void);				// ゲームオーバー時にやりなおすを選択したときに再設定するものを集めた関数

	Monster* _monster[1];
	Cards* _cards;
	Player* _player;
	Menu* _menu;
	Item* _item;
	Event* _event;
	//std::unique_ptr<Monster*> _monster;
	//std::unique_ptr<Cards*> _cards;
	//std::unique_ptr<Player*> _player;
	//std::unique_ptr<Menu*> _menu;
	//std::unique_ptr<Item*> _item;
	//std::unique_ptr<Event*> _event;

	// 扉関係
	float _doorExpand;					// 扉の拡大用変数
	float _degree;						// 歩きモーション用
	int _doorOpenTiming;				// 歩いてから扉が開くまでの時間調整用
	float _lastDoorExpand;				// クリア前の光る扉の拡大時に使用
	int _lastmoveTime;					// 一定時間以下なら拡大を続ける時に使う
	bool _changeToClear;				// 一定時間以上でフラグがtrueになり、自動的にクリア画面に移行する

	// 画面を揺らし関係
	bool _shakeChangeFlg;				// 一定値以上左右どちらかに揺れたら反対方向へ揺らすためのフラグ
	float _shakeTime;					// 揺らす時間
	VECTOR2 _shackPos;					// 揺れ幅

	// ダンジョン関係
	void Direct(void);			// 移動と向きを変える関数
	void Key(void);				// キー処理関数
	std::pair<bool, int> _dungeonMap[10][10];				// マップ(通ったことのあるところはtrue)
	std::vector<std::tuple<VECTOR2, int, float, PL_DIRECTION>> _mapVec;	// マップ移動軌跡 <現在座標,現在位置番号,画像回転角度,プレイヤーの向き>
	int _plNowPoint;			// プレイヤーの現在地
	int _plOldPoint;			// プレイヤーの前回いた位置
	bool _leftFlg;				// 左折したらtrue
	bool _rightFlg;				// 右折したらtrue
	double _directRota;			// 自分アイコンの向き調整用
	PL_DIRECTION _plDirect;		// プレイヤーの向いている方向
	PL_DIRECTION _plDirectOld;	// プレイヤーの前回向いていた方向
	VECTOR2 _mapChipDrawOffset;	// マップチップの描画位置オフセット
	VECTOR2 _plNowMark;			// 自分アイコン描画位置調整用
	bool _allMapFlg;			// 全体マップ表示のときはtrue

	// その他
	int _blinkCnt;						// 攻撃を食らったときに起こる点滅
	int _monsTimeCnt;					// 敵登場歩数調整用
	int _walkDirect;					// 左折と右折で視点を寄せていくときに使用
	int _plDeadChangeWinColor;			// プレイヤー死亡時、画面の色を反転させるときに使う
	int _poisonCnt;						// 戦闘中の毒効果の紫色画面の時間(初期値は255より大きい必要がある)
	bool _onceFlg;						// 敵からのドロップアイテム表示に使用と敵の待機ターンが0の時に使用
	bool _anounceFlg;					// 敵からのドロップアイテムで持ち物がいっぱいの時に案内を出す
	float _dunFog[2];					// 霧
	VECTOR2 _bossPos;					// ボス登場位置
	bool _keyFlg;						// キーが押されたらtrueになり足音SEがなり終わったらfalseになる
	int _levelUpAnounceTime;			// レベルアップ時の案内表示時間
	int _kyouseiButtlePngMoveCnt;		// 「強制戦闘」の文字画像の移動処理用
	bool _turnEndOnceFlg;				// プレイヤーターン終了時にtrueにする
	VECTOR2 _bossemErgencyPos;			// ボス前の警告画像を出す座標
	int mapChipSize_;					// マップチップの1つ当たりのサイズ
	const int screen_sizeX;				// 画面解像度 横
	const int screen_sizeY;				// 画面解像度 縦
	bool _guideFlg;						// 一定時間キー操作が行われなかった場合にtrueになり、操作ガイドが表示されるようにする
	int _guideVisibleTime;				// キー操作が行われていない時間を計測
	int _guideMove;						// ガイド描画時に背景の枠を自由変形で動かすときに使う

	std::map<int, EVENT_STATE> _eventStateMap;

	/*画像関係*/
	std::map<std::string, int> _drawHandle;

	// 敵ターンまでのカウント用画像
	int _turnPNG[6];
	// 部屋(扉開閉込み)の画像
	int _room[3];
	// ダンジョン関連
	int _roadPNG[static_cast<int>(MAP::MAX)];
	int _chipPNG[static_cast<int>(MAP::MAX)];
	// 霧
	int _dungeonFogPNG[2];

	/*音関係*/
	// SE関連
	int _soundSE[8];	// SE
	int _seCnt;			// 歩き音からドア音までの鳴る間隔
	bool _soundWalk;	// 歩き音からドア音につなぐときに必要なフラグ
	float _walkCnt;		// 歩く音とキーの入力受付タイミングの調整
	// BGM関連
	int _gameBGM;		// 通常BGM
	int _battleBGM;		// 戦闘BGM
};