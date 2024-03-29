#pragma once
#include <DxLib.h>
#include <map>
#include <string>
#include <vector>
#include <utility>
#include <memory>
#include "BaseScene.h"
#include "../Common/VECTOR2.h"
#include "../Card/Cards.h"
#include "../Enemy/Monster.h"
#include "../Player.h"
#include "../Menu.h"
#include "../Item/Item.h"
#include "../Event/Event.h"

#define MAP_SIZE_X 10	// マップの横サイズ
#define MAP_SIZE_Y 10	// マップの縦サイズ

class MouseCtl;

// 道画像
enum class MAP {
	STRAIGHT,		// 直進
	RIGHT,			// 右折のみ
	LEFT,			// 左折のみ 
	STOP,			// 行き止まり(イベントもない)
	TJI,			// T字路
	T_SR,			// ト字型(直進と右への道)
	T_SL,			// ト字型(直線と左への道)
	INN,			// 宿
	MERCHANT,		// 商人
	BUTTON,			// ボタン
	CHEST,			// 宝箱
	DRINK,			// 飲み物
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

// マップ移動軌跡
struct MapMake {
	VECTOR2 pos;			// 現在座標
	int num;				// 現在位置番号
	float rota;				// 画像回転角度
	PL_DIRECTION dir;		// プレイヤーの向き
};

class GameScene :
	public BaseScene
{
public:
	GameScene();
	~GameScene();
	unique_Base Update(unique_Base own, const GameCtl& ctl) override;

	std::shared_ptr<MouseCtl> mouse;
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
	bool Init(void) override;			// 初期化
	void PngInit(void);					// 画像用初期化
	void Draw(void) override;			// 描画
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
	void MoveCard(void);				// カードを動かした際の処理
	void DungeonFog(void);				// 霧の効果
	void ButtleCaution(void);			// 強制戦闘の案内を出す
	void GameReset(void);				// ゲームオーバー時にやりなおすを選択したときに再設定する変数を集めた関数

	std::shared_ptr<Monster> _monster[1];
	std::shared_ptr<Cards> _cards;
	std::shared_ptr<Player> _player;
	std::shared_ptr<Menu> _menu;
	std::shared_ptr<Item> _item;
	std::unique_ptr<Event> _event;

	// 扉関係
	float _doorExpand;					// 扉の拡大用変数
	float _degree;						// 歩きモーション用
	int _doorOpenTiming;				// 歩いてから扉が開くまでの時間調整用
	float _lastDoorExpand;				// クリア前の光る扉の拡大時に使用
	int _lastmoveTime;					// 一定時間以下なら拡大を続ける時に使う
	bool _changeToClear;				// 一定時間以上でフラグがtrueになり、自動的にクリア画面に移行する

	// 画面揺らし関係
	bool _shakeChangeFlg;				// 一定値以上左右どちらかに揺れたら反対方向へ揺らすためのフラグ
	float _shakeTime;					// 揺らす時間
	VECTOR2 _shackPos;					// 揺れ幅

	// ダンジョン関係
	void Direct(void);					// 移動と向きを変える関数
	void Key(void);						// キー処理関数
	std::pair<bool, int> _dungeonMap[MAP_SIZE_Y][MAP_SIZE_X];	// マップ(通ったことのあるところはtrue)
	std::vector<MapMake> _mapVec;		// マップ移動軌跡の保存用変数
	int _plNowPoint;					// プレイヤーの現在地
	int _plOldPoint;					// プレイヤーの前回いた位置
	bool _leftFlg;						// 左折したらtrue
	bool _rightFlg;						// 右折したらtrue
	double _directRota;					// 自分アイコンの向き調整用
	PL_DIRECTION _plDirect;				// プレイヤーの向いている方向
	PL_DIRECTION _plDirectOld;			// プレイヤーの前回向いていた方向
	VECTOR2 _mapChipDrawOffset;			// マップチップの描画位置オフセット
	VECTOR2 _plNowMark;					// 自分アイコン描画位置調整用
	bool _allMapFlg;					// 全体マップ表示のときはtrue

	// その他
	int _blinkCnt;						// 攻撃を食らったときに起こる点滅
	int _monsTimeCnt;					// 敵登場歩数調整用
	int _walkDirect;					// 左折と右折で視点を寄せていくときに使用
	int _plDeadChangeWinColor;			// プレイヤー死亡時、画面の色を反転させるときに使う
	int _poisonCnt;						// 戦闘中の毒効果の紫色画面の時間(初期値は255より大きい必要がある)
	bool _onceFlg;						// 敵からのドロップアイテム表示に使用と敵の待機ターンが0の時に使用
	bool _anounceFlg;					// 敵からのドロップアイテムで持ち物がいっぱいの時に案内を出す
	float _fog[2];						// 霧
	VECTOR2 _bossPos;					// ボス登場位置
	bool _keyFlg;						// キーが押されたらtrueになり足音SEがなり終わったらfalseになる
	int _levelUpAnounceTime;			// レベルアップ時の案内表示時間
	int _forcedButtlePngMoveCnt;		// 「強制戦闘」の文字画像の移動処理用
	bool _turnEndOnceFlg;				// プレイヤーターン終了時にtrueにする
	VECTOR2 _bossEmergencyPos;			// ボス前の警告画像を出す座標
	int _mapChipSize;					// マップチップの1つ当たりのサイズ
	const int screen_sizeX;				// 画面解像度 横
	const int screen_sizeY;				// 画面解像度 縦
	bool _guideFlg;						// 一定時間キー操作が行われなかった場合にtrueになり、操作ガイドが表示されるようにする
	int _guideVisibleTime;				// キー操作が行われていない時間を計測
	int _guideMove;						// ガイド描画時に背景の枠を自由変形で動かすときに使う
	float _guideExrMove;				// ガイド描画時の拡大/縮小
	bool _buttleGuideFlg;				// 戦闘ガイドの表示/非表示切替
	bool _stopCardUpDate;				// カード情報のアップデート関数が呼べなくなるようにする(Draw関数の後にupdateが止まる必要がある為、使っている)

	std::map<int, EVENT_STATE> _eventStateMap;	// イベント番号とenum classを一致させてイベント処理を行いやすくする

	/*画像関係*/
	std::map<std::string, int> _drawHandle;		
	int _turnPNG[6];							// 敵ターンまでのカウント用画像
	int _room[3];								// 部屋(扉開閉込み)の画像
	int _roadPNG[static_cast<int>(MAP::MAX)];	// ダンジョン画像
	int _chipPNG[static_cast<int>(MAP::MAX)];	// マップチップ画像
	int _dungeonFogPNG[2];						// 霧画像

	/*音関係*/
	// SE関連
	int _soundSE[8];					// SE
	int _seCnt;							// 歩き音からドア音までの鳴る間隔
	bool _soundWalk;					// 歩き音からドア音につなぐときに必要なフラグ
	float _walkCnt;						// 歩く音とキーの入力受付タイミングの調整
	// BGM関連
	int _gameBGM;						// 通常BGM
	int _battleBGM;						// 戦闘BGM
	int _nowBGM;						// 現在再生中のBGM情報を保存する
};