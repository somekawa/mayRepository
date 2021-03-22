#include "DxLib.h"
#include <string>
#include "SceneMng.h"
#include "../Common/GameCtl.h"
#include "TitleScene.h"
#include "GameClearScene.h"
#include "GameOverScene.h"
#include "GameScene.h"
#include "../Item/Item.h"
#include "../Enemy/Enemy_weak.h"
#include "SelectScene.h"
#include "../MoveObj.h"
#include "../Common/MouseCtl.h"

// static変数の初期化(<型>クラス名::変数名 = 初期化値)
int GameScene::plPosX = 0;
int GameScene::plPosY = 0;
bool GameScene::monsterFlg = false;
bool GameScene::bossClearFlg = false;

#define PI 3.141592653589793f

GameScene::GameScene() :screen_sizeX(900), screen_sizeY(600)
{
	Init();
	_cards = std::make_shared<Cards>();
	_player = std::make_shared<Player>();
	_monster[0] = std::make_shared<Enemy_weak>();
	_menu = std::make_shared<Menu>();
	_item = std::make_shared<Item>();
	mouse = std::make_shared<MouseCtl>();
	_event = std::make_unique<Event>();
}

GameScene::~GameScene()
{
	// 音関係
	DeleteSoundMem(_gameBGM);
	DeleteSoundMem(_battleBGM);
	for (int i = 0; i < 8; i++)
	{
		DeleteSoundMem(_soundSE[i]);
	}

	// vectorの解放
	for (auto v = _mapVec.begin(); v != _mapVec.end(); ++v)
	{
		std::vector<std::tuple<VECTOR2, int, float>> arr;
		std::vector<std::tuple<VECTOR2, int, float>>().swap(arr);
	}
}

bool GameScene::Init(void)
{
	// 文字の調整
	SetFontSize(20);                             // サイズを20に変更
	SetFontThickness(1);                         // 太さを1に変更
	ChangeFont("HGS創英角ﾎﾟｯﾌﾟ体");              // HGS創英角ﾎﾟｯﾌﾟ体に変更
	ChangeFontType(DX_FONTTYPE_ANTIALIASING_8X8);// アンチエイリアス

	_mapChipSize = 50;

	// 画像登録
	PngInit();

	// 扉関係
	moveFlg = false;
	openDoor = false;
	_doorExpand = 1.0f;
	_degree = 0.0f;
	_doorOpenTiming = 20;
	_lastDoorExpand = 1.5f;
	_lastmoveTime = 0;
	_changeToClear = false;

	// イベント関係
	eventState = EVENT_STATE::NON;
	
	// 画面揺らし関係
	shakeFlg = false;
	_shakeChangeFlg = false;
	_shakeTime = 0.0f;
	_shackPos = { 0,0 };

	// マップ読み込み(配列は[y][x]の順になっている)
	int mapFileHandle;
	if (SelectScene::modeSelect == MODE::NORMAL)
	{
		mapFileHandle = FileRead_open("csv/map1.csv");
		if (mapFileHandle == NULL)
		{
			return false; // エラー
		}
		_bossPos = { 8,5 };			
		_bossEmergencyPos = { 7,5 };
	}
	else if (SelectScene::modeSelect == MODE::HARD)
	{
		mapFileHandle = FileRead_open("csv/map2.csv");
		if (mapFileHandle == NULL)
		{
			return false; // エラー
		}
		_bossPos = { 5,9 };
		_bossEmergencyPos = { 5,8 };
	}
	else
	{
		// ここにきたらエラー
		return false;
	}

	for (int i = 0; i < 10; i++)
	{
		FileRead_scanf(mapFileHandle, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", &_dungeonMap[i][0].second, &_dungeonMap[i][1].second, &_dungeonMap[i][2].second, &_dungeonMap[i][3].second, &_dungeonMap[i][4].second, &_dungeonMap[i][5].second, &_dungeonMap[i][6].second, &_dungeonMap[i][7].second, &_dungeonMap[i][8].second, &_dungeonMap[i][9].second);
	}
	//ファイルを閉じる
	FileRead_close(mapFileHandle);

	// ダンジョン関係
	bossClearFlg = false;
	backFlg = false;
	plPosX = 0;
	plPosY = 0;
	_plNowPoint = _dungeonMap[plPosY][plPosX].second;
	_plDirect = PL_DIRECTION::UP;
	_plDirectOld = PL_DIRECTION::UP;
	_leftFlg = false;
	_rightFlg = false;
	_directRota = 0.0f;
	_mapChipDrawOffset = { 0,0 };
	_plNowMark = { 0,0 };
	_allMapFlg = false;

	// その他
	blinkFlg = false;
	_blinkCnt = 0;
	_monsTimeCnt = 5;
	_walkDirect = 0;
	_plDeadChangeWinColor = 255;
	_poisonCnt = 256;
	_onceFlg = false;
	_anounceFlg = false;
	_fog[0] = 0.0f;
	_fog[1] = -900.5f;
	_keyFlg = false;
	_levelUpAnounceTime = 180;
	_forcedButtlePngMoveCnt = 0;
	_turnEndOnceFlg = false;
	_guideFlg = false;
	_guideVisibleTime = 0;
	_guideMove = 0;
	_guideExrMove = 0.0f;
	_buttleGuideFlg = false;

	//13以外 (7~12と14)
	for (int i = static_cast<int>(EVENT_STATE::INN); i <= static_cast<int>(EVENT_STATE::GOAL);)
	{
		if (i != static_cast<int>(EVENT_STATE::EVE_MONS))
		{
			_eventStateMap.try_emplace(i + 5, static_cast<EVENT_STATE>(i));
		}
		i++;
	}

	// あらかじめマップ用のメモリ領域を確保しておく
	_mapVec.reserve(100);

	// SE
	_soundSE[0] = LoadSoundMem("sound/se/click.mp3");			// クリック音
	_soundSE[1] = LoadSoundMem("sound/se/walk_short.mp3");		// 歩行音
	_soundSE[2] = LoadSoundMem("sound/se/door.mp3");			// ドア音
	_soundSE[3] = LoadSoundMem("sound/se/attackCard.mp3");		// 攻撃カード音
	_soundSE[4] = LoadSoundMem("sound/se/guardCard.mp3");		// 防御カード音
	_soundSE[5] = LoadSoundMem("sound/se/healCard.mp3");		// 回復カード音
	_soundSE[6] = LoadSoundMem("sound/se/damage.mp3");			// ダメージ
	_soundSE[7] = LoadSoundMem("sound/se/poison.mp3");			// 毒音
	_soundWalk = false;
	_walkCnt = 12.0f;

	// BGM
	_gameBGM = LoadSoundMem("sound/bgm/dangeon.mp3");
	_battleBGM = LoadSoundMem("sound/bgm/battle.mp3");
	PlaySoundMem(_gameBGM, DX_PLAYTYPE_LOOP, true);
	return true;
}

void GameScene::PngInit(void)
{
	// ターン画像のメモリへの読みこみ
	std::string number = "image/number/number.png";
	LoadDivGraph(number.c_str(), 6, 6, 1, 40, 50, _turnPNG);

	// 扉
	std::string room[3];
	room[0] = "image/room_0.png";
	room[1] = "image/room_1.png";
	room[2] = "image/room_2.png";
	for (int i = 0; i < 3; i++)
	{
		_room[i] = LoadGraph(room[i].c_str());
	}

	// ダンジョン
	std::string dungeon[7];
	dungeon[0] = "image/dan_go.png";			// 直進
	dungeon[1] = "image/dan_right.png";			// 右折のみ
	dungeon[2] = "image/dan_left.png";			// 左折のみ
	dungeon[3] = "image/dan_stop.png";			// 行き止まり
	dungeon[4] = "image/dan_T.png";				// T字路
	dungeon[5] = "image/dan_TONOJI_SR.png";		// トの字型(直線と右への道)
	dungeon[6] = "image/dan_TONOJI_SL.png";		// トの字型(直線と左への道)
	for (int i = 0; i < 7; i++)
	{
		_roadPNG[i] = LoadGraph(dungeon[i].c_str());
	}
	for (int i = 7; i < 13; i++)
	{
		_roadPNG[i] = LoadGraph(dungeon[3].c_str());
	}
	_roadPNG[13] = LoadGraph(dungeon[0].c_str());
	_roadPNG[14] = LoadGraph(room[0].c_str());

	_drawHandle.try_emplace("hpbar_en", LoadGraph("image/hpbar_en.png"));
	_drawHandle.try_emplace("hpbar_back", LoadGraph("image/hpbar_back.png"));
	_drawHandle.try_emplace("hpbar_waku", LoadGraph("image/hpbar_waku.png"));

	_drawHandle.try_emplace("direct", LoadGraph("image/direct.png"));
	_drawHandle.try_emplace("start_chip", LoadGraph("image/mapchip/start_chip.png"));
	_drawHandle.try_emplace("white", LoadGraph("image/white.png"));
	_drawHandle.try_emplace("monster_info", LoadGraph("image/monster_info.png"));
	_drawHandle.try_emplace("getItem", LoadGraph("image/getItem.png"));
	_drawHandle.try_emplace("try", LoadGraph("image/try.png"));
	_drawHandle.try_emplace("dead", LoadGraph("image/dead.png"));
	_drawHandle.try_emplace("message_death", LoadGraph("image/message_death.png"));
	_drawHandle.try_emplace("message_death2", LoadGraph("image/message_death2.png"));
	_drawHandle.try_emplace("emergency", LoadGraph("image/emergency.png"));
	_drawHandle.try_emplace("square", LoadGraph("image/square.png"));
	_drawHandle.try_emplace("forced_combat", LoadGraph("image/forced_combat.png"));
	_drawHandle.try_emplace("red_caution", LoadGraph("image/red_caution.png"));
	_drawHandle.try_emplace("buttleGuide", LoadGraph("image/buttleGuide.png"));
	// マップチップ
	std::string mapchip = "image/mapchip/mapchip.png";

	LoadDivGraph(mapchip.c_str(), 15, 15, 1, _mapChipSize, _mapChipSize, _chipPNG);

	// 霧
	std::string fog = "image/w_fog.png";
	_dungeonFogPNG[0] = LoadGraph(fog.c_str());

	std::string fog2 = "image/w_fog2.png";
	_dungeonFogPNG[1] = LoadGraph(fog2.c_str());
}

unique_Base GameScene::Update(unique_Base own, const GameCtl& ctl)
{
	mouse->UpDate();
	//マウスの左ボタンが押されていたら
	if (mouse->GetClickTrg())
	{	
		// プレイヤーが死亡していないとき
		if (_player->GetHP() > 0)
		{
			// 敵がいないときだけ押せる
			if (eventState != EVENT_STATE::ENEMY && eventState != EVENT_STATE::EVE_MONS && !_event->GetEventMonsFlg())
			{
				_menu->MenuButton_NonEnemy(mouse);
			}
			else
			{
				// 敵がいるとき(アイテムだけ)
				if (!_player->GetSkillBackFlg())
				{
					// スキル選択画面が押されていないときだけ
					_menu->MenuButton_Enemy(mouse);
				}
			}
		}
		else
		{
			// レベルをそのままにはじめからやり直す
			if (mouse->GetPos().x >= 50 && mouse->GetPos().x <= 50 + 400 && mouse->GetPos().y >= 225 && mouse->GetPos().y <= 225 + 130)
			{
				GameReset();
			}

			// あきらめてゲームオーバー画面へ
			if (mouse->GetPos().x >= 450 && mouse->GetPos().x <= 450 + 400 && mouse->GetPos().y >= 225 && mouse->GetPos().y <= 225 + 130)
			{
				DeleteSoundMem(_gameBGM);
				DeleteSoundMem(_battleBGM);
				return std::make_unique<GameOverScene>();
			}
		}

		_player->ClickUpDate(_monster[0], _menu, this, _cards);
	}

	// ゴール処理
	if (eventState == EVENT_STATE::GOAL)
	{
		// 扉描画時にUPキーで出口処理
		if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_W]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_W]))
		{
			openDoor = true;
		}
	}

	MouseClick_Go(ctl);

	// タイトルへ戻る
	if (_menu->GetMenu() == MENU::TO_TITLE)
	{
		return std::make_unique<TitleScene>();
	}

	// ゴールに到達したらクリア画面へ移行
	if (_changeToClear)
	{
		return std::make_unique<GameClearScene>();
	}

	Walk();
	ChangeBGM();
	Pl_Dead();
	_menu->Update(this, _player, _monster[0], _cards,mouse);
	_event->UpDate(this, _player, _menu, _item, _monster[0], _cards,mouse);
	EnemyItemDrop();
	_monster[0]->update();

	//	時間経過でレベルアップしていたときに立ててたフラグをfalseに戻す
	if (_player->GetLevelUpAnounceFlg())
	{
		if (_levelUpAnounceTime > 0)
		{
			_levelUpAnounceTime--;
		}
		else
		{		
			_player->SetLevelUpAnounceFlg(false);
			_levelUpAnounceTime = 180;
		}
	}

	// アイテム画面の時にはカードを動かせなくする
	if (_monster[0]->GetEnemyState() == ENEMY_STATE::EXIST && _menu->GetMenu() != MENU::ITEM)
	{
		// スキル選択画面では動かせなくする
		if (!_player->GetSkillBackFlg())
		{
			// 戦闘中以外は必要ない。
			_cards->Update();		// カードの情報

		}
	}

	DungeonFog();
	ButtleCaution();

	if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_F2]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_F2]))
	{
		// 全体マップを描画したり消したりする
		_allMapFlg = !_allMapFlg;
		if (_guideFlg)
		{
			_guideFlg = false;
			_guideVisibleTime = 0;
			_guideMove = 0;
		}
	}

	// 戦闘中、F4キーでガイドの表示が切り替えられる
	if (_monster[0]->GetEnemyState() == ENEMY_STATE::EXIST)
	{
		if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_F4]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_F4]))
		{
			_buttleGuideFlg = !_buttleGuideFlg;
		}
	}

	// 戦闘ガイドの表示/非表示時のアニメーション
	if (_buttleGuideFlg)
	{
		_guideExrMove < 1.0f ? _guideExrMove += 0.03f : _guideExrMove = 1.0f;
	}
	else
	{
		_guideExrMove > 0.0f ? _guideExrMove -= 0.03f : _guideExrMove = 0.0f;
	}

	Draw();
	CardEffect();
	_player->UpDate();

	// 5ターン分経過
	if (_player->GetConditionTurn() <= 0)
	{
		_player->SetCondition(CONDITION::FINE);
		_player->SetConditionTurn(0);
	}

	Pl_TurnEndAfter();
	EventUpdate();

	// 逃走アイテムが使用されたら敵を消す
	if (_menu->GetEscapeFlg())
	{
		_monster[0]->SetEnemyState(ENEMY_STATE::NON);
		_menu->SetEscapeFlg(false);
		eventState = EVENT_STATE::NON;
		_keyFlg = false;
	}

	// 自分のSceneのユニークポインタを返す
	return std::move(own);
}

void GameScene::Draw(void)
{
	ClsDrawScreen();

	// 死亡したら、色を反転させる
	if (_player->GetHP() <= 0)
	{
		SetDrawBlendMode(DX_BLENDMODE_INVSRC, _plDeadChangeWinColor);
	}

	if (shakeFlg)
	{
		// 画面を揺らす
		ShakeDraw();
	}
	else
	{
		DrawRotaGraph(450 - _walkDirect, 300, 1.0f, 0, _roadPNG[_plNowPoint], false);
		_shakeTime = 0.0f;
		_shakeChangeFlg = false;
	}

	// 行き止まりでないかつバック処理でないときは前に動く動作
	if (_plNowPoint != 3 && !backFlg)
	{
		if (moveFlg)
		{
			// 歩いてるときに揺れてる感じを出す
			_degree += 1.0f * 7.0f;
			auto radian = _degree * PI / 180.0f;
			auto sin = sinf(static_cast<float>(radian));
			auto move = 0.0f;

			// だんだん扉が近づく
			if (_doorExpand <= 1.5f)
			{
				_doorExpand += 0.005f;
				move = sin * 25.0f;
				_doorOpenTiming = 20;
				// 右折なら加算
				if (_plNowPoint == 1)
				{
					_walkDirect += 2;
				}
				else if (_plNowPoint == 2)	// 左折なら減算
				{
					_walkDirect -= 2;
				}
			}
			else
			{
				if (_doorOpenTiming <= 0)
				{
					move = 0.0f;
					openDoor = true;
				}
				else
				{
					_doorOpenTiming--;
				}
			}
			DrawRotaGraphF(450.0f - _walkDirect, move + 300.0f, _doorExpand, 0, _roadPNG[_plNowPoint], false);
		}

		if (openDoor)
		{
			// ゴール前の扉を光らせる
			if (eventState == EVENT_STATE::GOAL)
			{
				//最後はそこからさらにドアを拡大していって、時間経過でクリア画面に移行させる
				DrawRotaGraph(450, 300, _lastDoorExpand, 0, _room[2], false);
				_lastDoorExpand += 0.005f;
				if (_lastmoveTime <= 100)
				{
					_lastmoveTime++;
				}
				else
				{
					_changeToClear = true;
				}
				// だんだん画面全体を明るく
				SetDrawBlendMode(DX_BLENDMODE_ADD, 128 + _lastmoveTime);
				DrawGraph(0, 0, _drawHandle["white"], true);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
		}
	}
	else
	{
		openDoor = true;
		DrawRotaGraph(450, 300, 1.0f, 0, _roadPNG[_plNowPoint], false);
	}

	// 霧表現
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawGraphF(0.0f + _fog[0], 0.0f, _dungeonFogPNG[0], true);
	DrawGraphF(0.0f + _fog[1], 0.0f, _dungeonFogPNG[1], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 何もなしと敵以外の処理
	if (eventState != EVENT_STATE::ENEMY)
	{
		_event->Draw(this, _player, _menu, _item);
	}

	// マップ描画(敵出現時は描画しない)
	if (_allMapFlg && eventState != EVENT_STATE::ENEMY)
	{
		// 全体マップの表示
		AllMapDraw();
	}
	else if (!_allMapFlg && eventState != EVENT_STATE::ENEMY)
	{
		// 部分マップの表示
		SmallMapDraw();
	}

	// 敵に攻撃したら敵が点滅する
	if (blinkFlg)
	{
		if (_blinkCnt <= 40)
		{
			_blinkCnt++;
		}
		else
		{
			blinkFlg = false;
			_blinkCnt = 0;
		}

		if (_blinkCnt % 10 == 0)
		{
			// ボスならこっち
			if (plPosX == _bossPos.x && plPosY == _bossPos.y && !bossClearFlg)
			{
				_monster[0]->BossDraw();
			}
			else
			{
				if (!_event->GetEventMonsFlg())
				{
					// 通常の敵
					_monster[0]->Draw();
				}
			}
		}
	}
	else
	{
		// ボスじゃないとき
		if (plPosX == _bossPos.x && plPosY == _bossPos.y && !bossClearFlg)
		{
			if (eventState == EVENT_STATE::ENEMY && _monster[0]->GetEnemyState() != ENEMY_STATE::DEATH)
			{
				_monster[0]->BossDraw();
			}
		}
		else
		{
			if ((eventState == EVENT_STATE::NON) || (eventState == EVENT_STATE::ENEMY))
			{
				_monster[0]->Draw();
			}
		}
	}

	if (_turnEndOnceFlg)
	{
		// 攻撃エフェクト
		_monster[0]->EffectDraw();
	}

	_player->Draw(_menu);

	if (plPosX == _bossEmergencyPos.x && plPosY == _bossEmergencyPos.y)
	{
		// ボスの警告用画像
		DrawGraph(250, 100, _drawHandle["emergency"], true);
	}

	// 敵が存在するとき
	if (_monster[0]->GetEnemyState() == ENEMY_STATE::EXIST)
	{
		int posx = 600;
		int posy = 80;

		DrawGraph(posx-50, 0, _drawHandle["monster_info"], true);
		// 敵HPの描画
		DrawFormatString(posx+50, posy-20, 0x000000, "%d/%d", _monster[0]->GetNowHP(), _monster[0]->GetMaxHP());

		// 敵がいるときのみ描画
		DrawGraph(650, 150, _turnPNG[_cards->GetTurn()], true);

		// 敵(HPバー関連)
		DrawExtendGraph(posx, posy, posx + 110, posy + 33, _drawHandle["hpbar_back"], true);
		DrawExtendGraph(posx + 3, posy + 4, posx + 3 + static_cast<int>((105.0f * _monster[0]->GetHPBar())), posy + 4 + 25, _drawHandle["hpbar_en"], true);
		DrawExtendGraph(posx, posy, posx + 110, posy + 33, _drawHandle["hpbar_waku"], true);

		// 戦闘中以外は邪魔なので非表示で
		_cards->Draw(_player,_menu);
		_player->BattleDraw(_menu);

		// 一定時間操作がないときにはガイドの矢印を描画する
		if (_buttleGuideFlg)
		{
			DrawRotaGraph(50 + 400, posy + 20 + 224, _guideExrMove, 0.0f, _drawHandle["buttleGuide"], true);
			DrawGraph(200, 0, _drawHandle["square"], true);
			DrawFormatString(230, 20, 0x000000, "F4キー:\n戦闘ガイドを非表示");
		}
		else
		{
			if (_guideExrMove > 0.0f)
			{
				DrawRotaGraph(50 + 400, posy + 20 + 224, _guideExrMove, 0.0f, _drawHandle["buttleGuide"], true);
			}
			DrawGraph(200, 0, _drawHandle["square"], true);
			DrawFormatString(230, 20, 0x000000, "F4キー:\n戦闘ガイドを表示");
		}

		// 戦闘中に移動操作ガイドが描画されないように設定する
		if (_guideFlg)
		{
			_guideFlg = false;
			_guideVisibleTime = 0;
			_guideMove = 0;
		}
	}

	_menu->Draw(_player,_item,_monster[0]);

	// 敵からのドロップアイテム
	if (_anounceFlg)
	{
		DrawGraph(600, 200, _drawHandle["getItem"], true);

		if (_menu->GetCanHaveItem() == 0)
		{
			// 持ち物満タンだからもてないよ
			DrawFormatString(600, 180, 0xffffff, "所持品がいっぱいだ");
		}
	}

	// レベル上がったよ
	if (_player->GetLevelUpAnounceFlg())
	{	
		DrawGraph(0, 300, _drawHandle["square"], true);
		// 指定秒数の間、描画する
		DrawFormatString(30, 320, 0x000000, "レベルが%dになった", _player->GetNowLevel());
		DrawFormatString(30, 340, 0x000000, "HPの最大が%dになった",_player->GetMaxHP());
		DrawFormatString(30, 360, 0x000000, "攻撃力が%dになった", _player->GetAttackDamage());
	}

	// 戦闘中の毒の描画(プレイヤーがカードを使ったときにくらう毒のダメージ)
	if (eventState == EVENT_STATE::ENEMY && _cards->GetCardsType() != CARDS_TYPE::NON)
	{
		if (_player->GetCondition() == CONDITION::POISON)
		{
			// 描画輝度を紫のみにセット
			SetDrawBright(255, 0, 255);
			_poisonCnt = 0;
		}
	}
	if (_poisonCnt <= 255)
	{
		_poisonCnt += 5;
		// 描画輝度を元に戻す
		SetDrawBright(255,_poisonCnt, 255);
	}

	// 描画ブレンドモードをノーブレンドにする
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (_player->GetHP() <= 0)
	{
		// レベルを引継ぎ、はじめからやり直す
		DrawGraph(50, 225, _drawHandle["try"], true);

		// あきらめる
		DrawGraph(450, 225, _drawHandle["dead"], true);

		// 特定敵に出会う前に即死トラップで死んだとき
		if (!_event->GetEventMonsEncountFlg() && !_event->GetEventMonsEndFlg() && eventState == EVENT_STATE::TRAP)
		{
			DrawGraph(300, 0, _drawHandle["message_death"], true);
		}
		else if (_event->GetEventMonsEndFlg() && !monsterFlg)
		{
			// 敵もろとも即死トラップで死んだとき
			DrawGraph(300, 0, _drawHandle["message_death2"], true);
		}
	}

	_player->SkillDraw();

	// ガイド表示
	if (_guideFlg)
	{
		_guideMove < 50 ? _guideMove++: _guideMove = 50;
		// 読みこんだグラフィックを自由変形描画(左上,右上,右下,左下)
		DrawModiGraph(0, 150-_guideMove, 250, 150-_guideMove, 250, 150+_guideMove, 0, 150+_guideMove, _drawHandle["square"], true);
		DrawFormatString(30, 120, 0x000000, "～キー操作ガイド～");
		DrawFormatString(30, 140, 0x000000, "WASD: 移動");
		DrawFormatString(30, 160, 0x000000, "F2  : マップ表示切替");
	}

	if (_event->GetCautionFlg())
	{
		// 強制戦闘の案内時に描画する
		DrawGraph(0, 0, _drawHandle["red_caution"], true);
		DrawGraph(_forcedButtlePngMoveCnt, 250, _drawHandle["forced_combat"], true);
	}

	ScreenFlip();
}

void GameScene::AllMapDraw(void)
{
	VECTOR2 mapOffset = { 200,0 };
	// Sのマーク
	DrawGraph(0 * _mapChipSize + mapOffset.x, (screen_sizeY - _mapChipSize) - (0 * _mapChipSize) + mapOffset.y, _drawHandle["start_chip"], false);
	for (auto v = _mapVec.begin(); v != _mapVec.end(); ++v)
	{
		DrawRotaGraph((*v).pos.x + _mapChipSize/2 + mapOffset.x, (*v).pos.y + _mapChipSize/2 + mapOffset.y, 1.0, (*v).rota, _chipPNG[(*v).num], true);
	}
	DrawRotaGraph(plPosX * _mapChipSize + _mapChipSize/2 + mapOffset.x, (screen_sizeY - _mapChipSize) - (plPosY * _mapChipSize) + _mapChipSize/2 + mapOffset.y, 1.0f, _directRota, _drawHandle["direct"], true);
}

void GameScene::SmallMapDraw(void)
{
	if (_plNowMark.x == 0 && _plNowMark.y <= 2)
	{
		// Sのマーク
		DrawGraph(0 * _mapChipSize, (screen_sizeY - _mapChipSize) - (0 * _mapChipSize), _drawHandle["start_chip"], true);
	}
	// 現在地を保存していく
	for (auto v = _mapVec.begin(); v != _mapVec.end(); ++v)
	{
		if (plPosY <= 2)
		{
			_mapChipDrawOffset.y = 0;
		}
		else
		{
			_mapChipDrawOffset.y = _mapChipSize * (plPosY - 2);
		}

		if (plPosX <= 2)
		{
			_mapChipDrawOffset.x = 0;
		}
		else
		{
			_mapChipDrawOffset.x = _mapChipSize * (plPosX - 2);
		}

		if ((*v).pos.y + _mapChipSize/2 + _mapChipDrawOffset.y >= 450 && (*v).pos.x + _mapChipSize/2 - _mapChipDrawOffset.x <= 150)
		{
			DrawRotaGraph((*v).pos.x + _mapChipSize/2 - _mapChipDrawOffset.x, (*v).pos.y + _mapChipSize/2 + _mapChipDrawOffset.y, 1.0, (*v).rota, _chipPNG[(*v).num], true);
		}
	}

	// 現在地
	if (plPosY <= 2)
	{
		_plNowMark.y = plPosY;
	}
	else
	{
		_plNowMark.y = 2;
	}
	if (plPosX <= 2)
	{
		_plNowMark.x = plPosX;
	}
	else
	{
		_plNowMark.x = 2;
	}

	DrawRotaGraph(_plNowMark.x * _mapChipSize + _mapChipSize/2, (screen_sizeY - _mapChipSize) - (_plNowMark.y * _mapChipSize) + _mapChipSize/2, 1.0f, _directRota, _drawHandle["direct"], true);
}

void GameScene::ShakeDraw(void)
{
	// バトル中ならば0
	if (eventState == EVENT_STATE::ENEMY)
	{
		_cards->SetTurn(0);
	}

	// 画面を揺らす
	_shakeTime += 0.5f;
	if (!_shakeChangeFlg)
	{
		if (_shackPos.x >= 0 && _shackPos.x <= 20)
		{
			_shackPos.x += 5;

			if (_shackPos.x > 20)
			{
				_shakeChangeFlg = true;
			}
		}
	}

	if (_shakeChangeFlg)
	{
		if (_shackPos.x > 0)
		{
			_shackPos.x -= 5;
		}
		else
		{
			_shakeChangeFlg = false;
		}
	}

	DrawRotaGraph(450 + _shackPos.x, 300 + _shackPos.y, 1.0f, 0, _roadPNG[_plNowPoint], false);

	_blinkCnt++;
	// 画面全体を点滅
	if (_blinkCnt % 10 == 0)
	{
		if (eventState == EVENT_STATE::ENEMY || eventState == EVENT_STATE::CHEST)
		{
			// 描画輝度を赤のみにセット
			SetDrawBright(255, 0, 0);
		}

		if (eventState == EVENT_STATE::BUTTON)
		{
			// 描画輝度を黄のみにセット
			SetDrawBright(255, 255, 0);
		}

		if (_player->GetCondition() == CONDITION::POISON && eventState != EVENT_STATE::BUTTON)
		{
			// 描画輝度を紫のみにセット
			SetDrawBright(255, 0, 255);
		}
		DrawGraph(0, 0, _drawHandle["white"], true);
	}

	if (_shakeTime >= 15.0f)
	{
		// 通常戦闘時
		if (eventState == EVENT_STATE::ENEMY)
		{
			// 攻撃を受けた後はターンを復活させる
			_cards->SetTurn(_monster[0]->GetMaxTurn());
			_cards->SetGuard(0);
			_monster[0]->SetAnimCnt(0);
			_turnEndOnceFlg = false;
		}

		// イベント戦闘時
		if (eventState == EVENT_STATE::EVE_MONS)
		{
			_cards->SetTurn(3);
			_cards->SetGuard(0);
			_monster[0]->SetAnimCnt(0);
			_turnEndOnceFlg = false;
		}

		shakeFlg = false;
		_blinkCnt = 0;
		// 描画輝度を戻す
		SetDrawBright(255, 255, 255);
	}
}

void GameScene::MouseClick_Go(const GameCtl& ctl)
{
	// メニュー画面表示中は進むボタンを押せないようにする
	if (!_menu->GetMenuFlg() && _menu->GetMenu() == MENU::NON && eventState == EVENT_STATE::NON)
	{
		// バック処理
		if (eventState == EVENT_STATE::NON)
		{
			if (_player->GetHP() > 0)
			{
				// 現在地がスタート地点でなければバック処理できる
				if (plPosX > 0 || plPosY > 0 && !_keyFlg)
				{
					if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_S]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_S]))
					{
						_keyFlg = true;
						backFlg = true;
					}
				}
			}
		}

		// 移動操作(画像拡大最中は入力を受け付けない)(死亡していないとき)
		if (!_keyFlg && _player->GetHP() > 0)
		{
			// 関数オブジェクト
			auto obj = MoveObj();
			if (obj(ctl, _plNowPoint, _rightFlg, _leftFlg, _plDirect))
			{
				_keyFlg = true;
				_guideFlg = false;
				_guideVisibleTime = 0;
				_guideMove = 0;
				_soundWalk = true;
				moveFlg = true;
			}
			else
			{
				// MoveObjがfalseで一定時間経過した場合、ガイドの表示をする
				if (eventState == EVENT_STATE::NON)
				{
					if (++_guideVisibleTime > 180 && !_guideFlg)
					{
						_guideFlg = true;
					}
				}
			}
		}
	}
}

void GameScene::EventUpdate(void)
{
	if (eventState == EVENT_STATE::ENEMY)
	{
		if (_monster[0]->GetEnemyState() == ENEMY_STATE::DEATH)
		{
			_event->SetEvent(eventState);
		}
	}

	for (auto eve = static_cast<int>(EVENT_STATE::INN); eve <= static_cast<int>(EVENT_STATE::MAX);)
	{
		if (eve == static_cast<int>(eventState))
		{
			_event->SetEvent(eventState);
			return;
		}
		else if(eve == static_cast<int>(EVENT_STATE::MAX))
		{
			// 敵かもしれないからnonにはできない
			return;
		}
		eve++;
	}
}

void GameScene::Pl_TurnEndAfter(void)
{
	// プレイヤーのターンが終了
	if (_cards->GetTurn() <= 0 && !shakeFlg)
	{
		// プレイヤーのターンが終了したらここに飛んできて、敵のターンに代わる
		// 敵が攻撃してくるのでプレイヤーのHPを減らす処理をする
		if (!_turnEndOnceFlg)
		{
			// 敵の種類によっては毒にかかる
			if (_monster[0]->GetEnemyNum() == 0 && _player->GetBarrierNum() <= 0)
			{
				int poison = GetRand(4);	// 0~4
				// 0 かつ 状態が毒でないときは、毒にかかる
				if (poison == 0 && _player->GetCondition() == CONDITION::FINE)			
				{
					// 毒音
					PlaySoundMem(_soundSE[7], DX_PLAYTYPE_BACK, true);
					_player->SetCondition(CONDITION::POISON);
				}
			}

			// ダメージ音
			PlaySoundMem(_soundSE[6], DX_PLAYTYPE_BACK, true);
			_turnEndOnceFlg = true;

			/*防御の仕組み*/
			//float a = (float)_cards->GetGuard() * 10.0f;	//30%
			//float b = (100.0f - a) / 100.0f;				//100%-30%=70%/100% = 0.7
			//float c = (float)_monster[0]->GetAttack() * b;//敵の与えてくるDamage量*0.7=7	
			//int d = _menu->GetEquipGuard() + _player->GetDifense() - (int)c;

			int damage = _menu->GetEquipGuard() + _player->GetDifense() - (int)((float)_monster[0]->GetAttack() * ((100.0f - (float)_cards->GetGuard() * 10.0f) / 100.0f));

			// +になったらプレイヤーのHPが回復しちゃう事件
			if (damage >= 0)
			{
				damage = 0;
			}

			// ノーダメージ
			if (_menu->GetNonDamageFlg())
			{
				damage = 0;
			}

			// バリア値
			if (_player->GetBarrierNum() > 0)
			{
				_player->SetBarrierNum(_player->GetBarrierNum() + damage);
			}
			else
			{
				// damageには0またはマイナス値が入っているので加算処理でok
				_player->SetHP(_player->GetHP() + damage);
			}

			_menu->SetNonDamageFlg(false);

			// 表示ターン数を0にして、画面揺らしに移行する
			shakeFlg = true;
		}
	}
}

void GameScene::Pl_Attack(void)
{
	// 敵にダメージを与えたら、ダメージ量を0に戻す
	// ダメージ計算(カード威力+基本攻撃力+装備による攻撃力プラス値+パワーアップ効果(ないときは0))
	_monster[0]->Damage(_cards->GetDamage() + _player->GetAttackDamage() + _menu->GetEquipDamage() + _menu->GetPowUp(),_cards);
	_cards->SetDamage(0);

	// 攻撃するごとに-1されていく
	if (_menu->GetPowUp() > 0)
	{
		_menu->SetPowUp(_menu->GetPowUp() - 1);
	}
	else
	{
		_menu->SetPowUp(0);
	}
}

void GameScene::Pl_Heal(void)
{
	/*回復の仕組み*/
	//float one = (float)_cards->GetHeal() / 10.0f; //0.2
	//float two = _player->GetMaxHP() * one;        //最大HP * 0.2 = 最大HPの2割
	_player->SetHP(_player->GetHP() + (int)(_player->GetMaxHP() * ((float)_cards->GetHeal() / 10.0f)));
	_cards->SetHeal(0);
}

void GameScene::Pl_Dead(void)
{
	// プレイヤーが死亡したときに画面の色を徐々に反転させる
	if (_player->GetHP() <= 0)
	{
		if (_plDeadChangeWinColor >= 30)
		{
			_plDeadChangeWinColor--;
			// ゲームBGMなら
			if (CheckSoundMem(_battleBGM) == 1)
			{
				// 音量の設定(だんだん小さく)
				ChangeVolumeSoundMem(_plDeadChangeWinColor, _battleBGM);
			}

			// ゲームBGMなら(だんだん小さく)
			if (CheckSoundMem(_gameBGM) == 1)
			{
				// 音量の設定
				ChangeVolumeSoundMem(_plDeadChangeWinColor, _gameBGM);
			}
		}
	}
}

void GameScene::Walk(void)
{
	if (_keyFlg && !openDoor)
	{
		_walkCnt -= 0.1f;
	}

	// 1:再生中 0:再生していない
	if (_soundWalk)
	{
		PlaySoundMem(_soundSE[1], DX_PLAYTYPE_BACK, true);
		_soundWalk = false;
		_walkCnt = 12.0f;
	}

	if (CheckSoundMem(_soundSE[1]) == 1)
	{
		_keyFlg = true;
	}
	else if (_keyFlg && _walkCnt <= 0.0f)
	{
		_keyFlg = false;
		// ここで次の道へ移動させたい
		openDoor = true;
		Key();
	}
}

void GameScene::ChangeBGM(void)
{
	// 戦闘BGMへの切替
	if (_monster[0]->GetEnemyState() == ENEMY_STATE::EXIST)
	{
		if (CheckSoundMem(_gameBGM) == 1)
		{
			StopSoundMem(_gameBGM);
			PlaySoundMem(_battleBGM, DX_PLAYTYPE_LOOP, true);
		}
	}
	else
	{
		// ゲームBGMへの切替
		if (CheckSoundMem(_battleBGM) == 1)
		{
			StopSoundMem(_battleBGM);
			PlaySoundMem(_gameBGM, DX_PLAYTYPE_LOOP, true);
		}
	}
}

void GameScene::EnemyItemDrop(void)
{
	// 敵がアイテムを落としたときの処理
	if (_monster[0]->GetDropFlg())
	{
		// 持ち物いっぱいだったけど使ったり捨てたりして持てるようになった時
		if (mouse->GetClickTrg()) 
		{	
			if (mouse->GetPos().x >= 600 && mouse->GetPos().x <= 600 + 150 && mouse->GetPos().y >= 200 && mouse->GetPos().y <= 200 + 75)
			{
				if (_anounceFlg && _menu->GetCanHaveItem() != 0)
				{
					// クリック音
					PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);

					// 持ち物が満タンじゃなければ持てる
					_menu->SetItem(_item->GetPair(static_cast<int>(_monster[0]->GetDrop()) - 1).second, _item->GetPair(static_cast<int>(_monster[0]->GetDrop()) - 1).first);
					_anounceFlg = false;
				}
			}
		}

		if (!_onceFlg)
		{
			if (_menu->GetCanHaveItem() != 0)
			{
				// 持ち物が満タンじゃなければ持てる
				_menu->SetItem(_item->GetPair(static_cast<int>(_monster[0]->GetDrop()) - 1).second, _item->GetPair(static_cast<int>(_monster[0]->GetDrop()) - 1).first);
				_anounceFlg = false;
				_onceFlg = true;
			}
			else
			{
				_anounceFlg = true;
				_onceFlg = true;
			}
		}
	}
}

void GameScene::CardEffect(void)
{
	auto lambdaEffect = [&](int seNum) {
		// カードごとの音
		PlaySoundMem(_soundSE[seNum], DX_PLAYTYPE_BACK, true);

		// スキルチャージが0より大きいときは減らしていく
		if (_player->GetSkillCharge() > 0)
		{
			_player->SetSkillCharge(_player->GetSkillCharge() - 1);
		}
		_cards->SetCardsType(CARDS_TYPE::NON);
	};

	auto lambdaPoison = [&]() {
		// 毒音
		PlaySoundMem(_soundSE[7], DX_PLAYTYPE_BACK, true);

		// 体力の1/6ぐらい削ろうかな
		_player->SetHP(_player->GetHP() - static_cast<int>(static_cast<float>(_player->GetMaxHP()) * (1.0f / 6.0f)));
		// 1ターンマイナス
		_player->SetConditionTurn(_player->GetConditionTurn() - 1);
	};

	if (_cards->GetCardsType() == CARDS_TYPE::ATTACK)
	{
		if (_player->GetCondition() == CONDITION::POISON)
		{
			lambdaPoison();
		}
		Pl_Attack();
		lambdaEffect(3);
		blinkFlg = true;
	}

	if (_cards->GetCardsType() == CARDS_TYPE::INN)
	{
		if (_player->GetCondition() == CONDITION::POISON)
		{
			lambdaPoison();
		}
		lambdaEffect(5);
		Pl_Heal();
	}

	if (_cards->GetCardsType() == CARDS_TYPE::GUARD)
	{
		if (_player->GetCondition() == CONDITION::POISON)
		{
			lambdaPoison();
		}
		lambdaEffect(4);
	}
}

void GameScene::Direct(void)
{
	// 行き止まりまたは、バックキー押下時にバック処理
	if (_plNowPoint == 3 || backFlg)
	{
		_rightFlg = false;
		_leftFlg = false;
		if (_plDirect == PL_DIRECTION::UP)
		{
			plPosY--;
		}
		else if (_plDirect == PL_DIRECTION::DOWN)
		{
			plPosY++;
		}
		else if (_plDirect == PL_DIRECTION::RIGHT)
		{
			plPosX--;
		}
		else if (_plDirect == PL_DIRECTION::LEFT)
		{
			if (_event->GetEventMonsFlg())
			{
				plPosX--;
			}
			else
			{
				plPosX++;
			}
		}

		_plNowPoint = _dungeonMap[plPosY][plPosX].second;

		// 方向だけ転換させる
		for (auto v = _mapVec.begin(); v != _mapVec.end(); ++v)
		{
			// 現在地を見て、登録している方向と回転角度を代入する
			if (plPosX * _mapChipSize == (*v).pos.x  && (screen_sizeY - _mapChipSize) - (plPosY * _mapChipSize) == (*v).pos.y)
			{
				_directRota = (*v).rota;
				_plDirect = (*v).dir;
				backFlg = false;
				return;
			}
		}
	}

	auto lambda = [&](PL_DIRECTION dir,double rota) {
		_plDirectOld = _plDirect;
		_plOldPoint = _plNowPoint;
		_plDirect = dir;
		_directRota = rota;
	};

	// T字路
	if (_plNowPoint == 4)
	{
		if (_rightFlg && _plDirect != PL_DIRECTION::UP && _plDirect != PL_DIRECTION::DOWN)
		{
			lambda(PL_DIRECTION::DOWN, PI);
			plPosY--;
			return;
		}

		if (_leftFlg && _plDirect != PL_DIRECTION::UP && _plDirect != PL_DIRECTION::DOWN)
		{
			lambda(PL_DIRECTION::UP, 0);
			plPosY++;
			return;
		}

		if (_rightFlg && _plDirect == PL_DIRECTION::UP)
		{
			lambda(PL_DIRECTION::RIGHT, PI / 2);
			plPosX++;
			return;
		}

		if (_leftFlg && _plDirect == PL_DIRECTION::UP)
		{
			lambda(PL_DIRECTION::LEFT, PI + PI / 2);
			plPosX--;
			return;
		}

		if (_rightFlg && _plDirect == PL_DIRECTION::DOWN)
		{
			lambda(PL_DIRECTION::LEFT, PI + PI / 2);
			plPosX--;
			return;
		}

		if (_leftFlg && _plDirect == PL_DIRECTION::DOWN)
		{
			lambda(PL_DIRECTION::RIGHT, PI / 2);
			plPosX++;
			return;
		}
	}

	if (_plDirect == PL_DIRECTION::UP)
	{
		if ((_plNowPoint == 0 || _plNowPoint == 5 || _plNowPoint == 6) && !_rightFlg && !_leftFlg)	// 直進
		{
			_plOldPoint = _plNowPoint;
			_plDirectOld = _plDirect;
			plPosY++;
			return;
		}
		else if (_plNowPoint == 1 || _plNowPoint == 5)
		{
			lambda(PL_DIRECTION::RIGHT, PI / 2);
			_rightFlg = true;
			plPosX++;
			return;
		}
		else if (_plNowPoint == 2 || _plNowPoint == 6)
		{
			lambda(PL_DIRECTION::LEFT, PI + PI / 2);
			_leftFlg = true;
			plPosX--;
			return;
		}
	}

	if (_plDirect == PL_DIRECTION::DOWN)
	{
		if (_plNowPoint == 0)	// 直進
		{
			_plOldPoint = _plNowPoint;
			plPosY--;
			return;
		}
		else if (_plNowPoint == 1)
		{
			lambda(PL_DIRECTION::LEFT, PI + PI / 2);
			_leftFlg = true;
			plPosX--;
			return;
		}
		else if (_plNowPoint == 2)
		{
			lambda(PL_DIRECTION::RIGHT, PI / 2);
			_rightFlg = true;
			plPosX++;
			return;
		}
	}

	if (_plDirect == PL_DIRECTION::RIGHT)
	{
		if (_plNowPoint == 0)	// 直進
		{
			_plOldPoint = _plNowPoint;
			plPosX++;
			return;
		}
		else if (_plNowPoint == 1 && _rightFlg)	// 右曲がり(下)
		{
			lambda(PL_DIRECTION::DOWN, PI);
			_rightFlg = false;
			plPosY--;
			return;
		}
		else if (_plNowPoint == 2 && _rightFlg)	// 左曲がり(上)
		{
			lambda(PL_DIRECTION::UP, 0.0f);
			_rightFlg = false;
			plPosY++;
			return;
		}
	}

	if (_plDirect == PL_DIRECTION::LEFT)
	{
		if (_plNowPoint == 0)	// 直進
		{
			_plOldPoint = _plNowPoint;
			plPosX--;
			return;
		}
		else if (_plDirectOld == PL_DIRECTION::DOWN)
		{
			lambda(PL_DIRECTION::DOWN, PI);
			_leftFlg = false;
			plPosY--;
			return;
		}
		else if (_plNowPoint == 1 && _leftFlg)	// 右曲がり(上)
		{
			lambda(PL_DIRECTION::UP, 0.0f);
			_leftFlg = false;
			plPosY++;
			return;
		}
		else if (_plNowPoint == 2 && _leftFlg)	// 左曲がり(下)
		{
			lambda(PL_DIRECTION::DOWN, PI);
			_leftFlg = false;
			plPosY--;
			return;
		}
	}
}

void GameScene::Key(void)
{
	// ドロップアイテム処理
	if (_monster[0]->GetDropFlg())
	{
		_monster[0]->SetDropFlg(false);
		_onceFlg = false;
		_anounceFlg = false;
	}

	_walkDirect = 0;
	moveFlg = true;

	if (openDoor)
	{
		// プレイヤーが状態異常のとき
		if (_player->GetCondition() == CONDITION::POISON)
		{
			// ダメージ音
			PlaySoundMem(_soundSE[7], DX_PLAYTYPE_BACK, true);

			// 体力の1/6削る
			_player->SetHP(_player->GetHP() - static_cast<int>(static_cast<float>(_player->GetMaxHP()) * (1.0f / 6.0f)));
			// 1ターンマイナス
			_player->SetConditionTurn(_player->GetConditionTurn() - 1);

			shakeFlg = true;
		}

		Direct();
		_plNowPoint = _dungeonMap[plPosY][plPosX].second;
		if (!_dungeonMap[plPosY][plPosX].first)
		{
			if ((_plNowPoint >= static_cast<int>(MAP::INN) && _plNowPoint <= static_cast<int>(MAP::DRINK)) 
				|| _plNowPoint == static_cast<int>(MAP::EVE_MONS) || _plNowPoint == static_cast<int>(MAP::GOAL))
			{
				// イベントアイコン(即死トラップ以外)のときはマップチップを回転させずに保存する
				_mapVec.emplace_back(MapMake{ VECTOR2(plPosX * _mapChipSize,
				(screen_sizeY - _mapChipSize) - (plPosY * _mapChipSize)), 
				_plNowPoint, 0.0f,_plDirect });
			}
			else
			{
				// 通常道と即死トラップの保存
				_mapVec.emplace_back(MapMake{ VECTOR2(plPosX * _mapChipSize,
				(screen_sizeY - _mapChipSize) - (plPosY * _mapChipSize)),
				_plNowPoint, static_cast<float>(_directRota), _plDirect });
			}
			// 通ったことのある道はフラグがtrueになる仕組み
			_dungeonMap[plPosY][plPosX].first = true;
		}

		_monster[0]->SetEnemyState(ENEMY_STATE::NON);
		moveFlg = false;
		openDoor = false;
		_degree = 0.0f;
		_doorExpand = 1.0f;
	}

	if (plPosX == _bossPos.x && plPosY == _bossPos.y)
	{
		eventState = EVENT_STATE::ENEMY;
	}
	else
	{
		int num = _plNowPoint;

		// 行き止まりか他イベントの場所に入り込んだ時には特定敵と強制戦闘
		if (_event->GetEventMonsEncountFlg())
		{
			if (num == 3 && !_event->GetEventMonsEndFlg())
			{
				eventState = EVENT_STATE::EVE_MONS;
				_event->SetEventMonsFlg(true);
				_monster[0]->SetEnemyNum(6, 0);
				_cards->SetTurn(3);
				return;
			}
			if (num >= 7 && num <= 11)
			{
				_event->SetEventMonsFlg(true);
			}
		}

		// 通常移動マスじゃないときと特定敵イベント中はカウンターを止める
		if (num < 7 && !_event->GetEventMonsEncountFlg())
		{
			if (_monsTimeCnt > 0)
			{
				_monsTimeCnt--;
			}
			else
			{
				eventState = EVENT_STATE::ENEMY;
				_monsTimeCnt = GetRand(5) + 4;
			}
		}
		else
		{
			eventState = _eventStateMap[num];
			if (num == 13)
			{
				if (!_event->GetEventMonsEndFlg())
				{
					eventState = EVENT_STATE::EVE_MONS;
				}
				else
				{
					_event->SetEventMonsEncountFlg(false);
					eventState = EVENT_STATE::NON;
					_plNowPoint = 0;
				}
			}
		}
	}

	// 敵の出現(特定敵イベントでもここに入るようにする)
	if (eventState == EVENT_STATE::ENEMY)
	{
		monsterFlg = true;
		if (_monster[0]->GetEnemyState() == ENEMY_STATE::NON)
		{
			moveFlg = false;

			if (plPosX == _bossPos.x && plPosY == _bossPos.y && !bossClearFlg)
			{
				// ボスならこっち
				_monster[0]->SetEnemyNum(5, _player->GetNowLevel());		// これで敵の情報をセットしている(ボス用)
				_cards->SetTurn(_monster[0]->GetMaxTurn());
			}
			else
			{
				// 敵は0~4まで
				auto ene = GetRand(4);
				_monster[0]->SetEnemyNum(ene, _player->GetNowLevel());		// これで敵の情報をセットしている(ランダム)
				_cards->SetTurn(_monster[0]->GetMaxTurn());
			}
		}
	}

	// 宿屋・商人・ボタン・宝箱の出現
	if (eventState != EVENT_STATE::NON && eventState != EVENT_STATE::ENEMY)
	{
		moveFlg = false;
	}
}

void GameScene::DungeonFog(void)
{
	// 霧処理
	if (_fog[0] <= 900.0f)
	{
		_fog[0] += 0.5f;
	}
	else
	{
		_fog[0] = -900.0f;
	}

	if (_fog[1] <= 900.0f)
	{
		_fog[1] += 0.5f;
	}
	else
	{
		_fog[1] = -900.0f;
	}
}

void GameScene::ButtleCaution(void)
{
	if (_event->GetCautionFlg())
	{
		if (_forcedButtlePngMoveCnt <= 250)
		{
			_forcedButtlePngMoveCnt += 20;
		}
		else if (_forcedButtlePngMoveCnt > 250 && _forcedButtlePngMoveCnt <= 300)
		{
			_forcedButtlePngMoveCnt++;
		}
		else if (_forcedButtlePngMoveCnt > 300 && _forcedButtlePngMoveCnt <= 900)
		{
			_forcedButtlePngMoveCnt += 20;
		}
		else
		{
			_forcedButtlePngMoveCnt = 0;
			_event->SetCautionFlg(false);
		}
	}
}

void GameScene::GameReset(void)
{
	_monster[0]->SetAnimCnt(0);
	_onceFlg = false;
	_turnEndOnceFlg = false;
	_walkCnt = 12.0f;

	// 場所をスタート地点に戻す
	plPosX = 0;
	plPosY = 0;
	_plNowPoint = _dungeonMap[plPosY][plPosX].second;
	_directRota = 0.0f;
	_plDirect = PL_DIRECTION::UP;
	_plDirectOld = PL_DIRECTION::UP;
	_leftFlg = false;
	_rightFlg = false;

	_cards->SetTurn(3);
	shakeFlg = false;

	// イベント状態の初期化
	_event->SetEventMonsEncountFlg(false);
	_event->SetEventMonsFlg(false);
	eventState = EVENT_STATE::NON;
	_monster[0]->SetEnemyState(ENEMY_STATE::NON);
	_event->SetEvent(EVENT_STATE::NON);
	_event->SetFateNum(-1);

	// 宝箱の状態をリセットする
	_event->SetReset();

	// HPは最大まで回復してあげる
	_player->SetHP(_player->GetMaxHP());
	// 毒状態で死亡したときなどは元に戻す
	_player->SetCondition(CONDITION::FINE);
	_player->SetConditionTurn(0);

	_plDeadChangeWinColor = 255;
	// 音量の設定(最大に戻す)
	ChangeVolumeSoundMem(255, _gameBGM);
	ChangeVolumeSoundMem(255, _battleBGM);
}