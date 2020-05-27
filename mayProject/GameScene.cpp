#include "DxLib.h"
#include <string>
#include "SceneMng.h"
#include "GameCtl.h"
#include "TitleScene.h"
#include "GameClearScene.h"
#include "GameOverScene.h"
#include "GameScene.h"
#include "Item.h"
#include "Enemy_weak.h"
#include "SelectScene.h"

// static変数の実体<型>クラス名::変数名 = 初期化;
int GameScene::plPosX = 0;
int GameScene::plPosY = 0;
bool GameScene::monsterFlg = false;

#define PI 3.141592653589793

GameScene::GameScene()
{
	Init();
	_cards = new Cards();
	_player = new Player();
	_monster[0] = new Enemy_weak();
	_menu = new Menu();
	_item = new Item();
	_event = new Event();
}

GameScene::~GameScene()
{
	if (_cards != nullptr)
	{
		delete _cards;
		_cards = nullptr;
	}
	if (_player != nullptr)
	{
		delete _player;
		_player = nullptr;
	}
	if (_monster[0] != nullptr)
	{
		delete _monster[0];
		_monster[0] = nullptr;
	}
	if (_menu != nullptr)
	{
		delete _menu;
		_menu = nullptr;
	}
	if (_item != nullptr)
	{
		delete _item;
		_item = nullptr;
	}
	if (_event != nullptr)
	{
		delete _event;
		_event = nullptr;
	}

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
	SetFontSize(20);                             //サイズを20に変更
	SetFontThickness(1);                         //太さを1に変更
	ChangeFont("HGS創英角ﾎﾟｯﾌﾟ体");              //HGS創英角ﾎﾟｯﾌﾟ体に変更
	ChangeFontType(DX_FONTTYPE_ANTIALIASING_8X8);//アンチエイリアス

	// 画像登録
	pngInit();

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
	auto mapFileHandle = FileRead_open("csv/map.csv");
	if (mapFileHandle == NULL)
	{
		return false; //エラー時の処理
	}
	for (int i = 0; i < 10; i++)
	{
		FileRead_scanf(mapFileHandle, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", &_dungeonMap[i][0].second, &_dungeonMap[i][1].second, &_dungeonMap[i][2].second, &_dungeonMap[i][3].second, &_dungeonMap[i][4].second, &_dungeonMap[i][5].second, &_dungeonMap[i][6].second, &_dungeonMap[i][7].second, &_dungeonMap[i][8].second, &_dungeonMap[i][9].second);
	}
	//ファイルを閉じる
	FileRead_close(mapFileHandle);

	// ダンジョン関係
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

	// その他
	blinkFlg = false;
	_blinkCnt = 0;
	_monsTimeCnt = 30;
	_walkDirect = 0;
	_plDeadChangeWinColor = 255;
	_poisonCnt = 256;
	_onceFlg = false;
	_anounceFlg = false;
	kiri[0] = 0.0f;
	kiri[1] = -900.0f;
	bossPos = { 8,5 };

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

	// BGM
	_gameBGM = LoadSoundMem("sound/bgm/dangeon.mp3");
	_battleBGM = LoadSoundMem("sound/bgm/battle.mp3");
	//PlaySoundMem(_gameBGM, DX_PLAYTYPE_LOOP, true);
	return true;
}

void GameScene::pngInit(void)
{
	// ターンの数字
	std::string one = "image/one.png";
	std::string two = "image/two.png";
	std::string three = "image/three.png";

	// ターン画像のメモリへの読みこみ
	std::string number = "image/number/number.png";
	LoadDivGraph(number.c_str(), 6, 6, 1, 40, 50, _turnPNG);

	// プレイヤーのHPバー
	std::string hpbar_pl = "image/hpbar_pl.png";
	std::string hpbar_en = "image/hpbar_en.png";
	std::string hpbar_back = "image/hpbar_back.png";
	_hpBarPl = LoadGraph(hpbar_pl.c_str());
	_hpBarEn = LoadGraph(hpbar_en.c_str());
	_hpBarBack = LoadGraph(hpbar_back.c_str());

	// 扉
	std::string room_0 = "image/room_0.png";
	std::string room_1 = "image/room_1.png";
	std::string room_2 = "image/room_2.png";
	_room[0] = LoadGraph(room_0.c_str());
	_room[1] = LoadGraph(room_1.c_str());
	_room[2] = LoadGraph(room_2.c_str());

	// ダンジョン
	// 直進
	std::string dan_go = "image/dan_go.png";
	// 右折のみ
	std::string dan_right = "image/dan_right.png";
	// 左折のみ
	std::string dan_left = "image/dan_left.png";
	// 行き止まり
	std::string dan_stop = "image/dan_stop.png";
	// T字路
	std::string dan_T = "image/dan_T.png";
	// トの字型(直線と右への道)
	std::string dan_TONOJI_SR = "image/dan_TONOJI_SR.png";
	// トの字型(直線と左への道)
	std::string dan_TONOJI_SL = "image/dan_TONOJI_SL.png";

	roadPNG[0] = LoadGraph(dan_go.c_str());
	roadPNG[1] = LoadGraph(dan_right.c_str());
	roadPNG[2] = LoadGraph(dan_left.c_str());
	roadPNG[3] = LoadGraph(dan_stop.c_str());
	roadPNG[4] = LoadGraph(dan_T.c_str());
	roadPNG[5] = LoadGraph(dan_TONOJI_SR.c_str());
	roadPNG[6] = LoadGraph(dan_TONOJI_SL.c_str());

	for (int i = 7; i < 13; i++)
	{
		roadPNG[i] = LoadGraph(dan_stop.c_str());
	}
	roadPNG[13] = LoadGraph(dan_go.c_str());
	roadPNG[14] = LoadGraph(room_0.c_str());

	// 現在地用▲マーク
	std::string direct = "image/direct.png";
	directPNG = LoadGraph(direct.c_str());
	
	// マップチップ
	std::string mapchip = "image/mapchip/mapchip.png";
	std::string mapchip_stop = "image/mapchip/chip_3.png";
	std::string mapchip_danger = "image/mapchip/chip_danger.png";

	LoadDivGraph(mapchip.c_str(), 7, 7, 1, 50, 50, chipPNG);
	for (int i = 7; i < 13; i++)
	{
		chipPNG[i] = LoadGraph(mapchip_stop.c_str());
	}
	chipPNG[13] = LoadGraph(mapchip_danger.c_str());
	chipPNG[14] = LoadGraph(mapchip_danger.c_str());

	// 白
	std::string white = "image/white.png";
	_whitePNG = LoadGraph(white.c_str());

	// 敵出現時
	std::string info = "image/monster_info.png";
	_enemyInfoPNG = LoadGraph(info.c_str());

	// 取るの文字
	std::string getitem = "image/getItem.png";
	_getItemPNG = LoadGraph(getitem.c_str());

	// やりなおすの文字
	std::string trypng = "image/try.png";
	_retryPNG = LoadGraph(trypng.c_str());

	// あきらめるの文字
	std::string dead = "image/dead.png";
	_deadPNG = LoadGraph(dead.c_str());

	// 即死トラップで死亡時(敵遭遇前)の画像
	std::string messageDeath = "image/message_death.png";
	_messageDeathPNG = LoadGraph(messageDeath.c_str());

	// 即死トラップで死亡時(敵遭遇後)の画像
	std::string messageDeath2 = "image/message_death2.png";
	_messageDeathPNG2 = LoadGraph(messageDeath2.c_str());

	// ボス手前警告画像
	std::string emergency = "image/emergency.png";
	bossEmergencyPNG = LoadGraph(emergency.c_str());

	// 霧
	std::string kiri = "image/kiri.png";
	kiriPNG[0] = LoadGraph(kiri.c_str());

	std::string kiri2 = "image/kiri2.png";
	kiriPNG[1] = LoadGraph(kiri2.c_str());
}

unique_Base GameScene::Update(unique_Base own, const GameCtl& ctl)
{
	//if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_F2]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_F2]))
	//{
	//	return std::make_unique<TitleScene>();
	//}

	mouse = GetMouseInput();					 //マウスの入力状態取得
	GetMousePoint(&cursorPos.x, &cursorPos.y);	 //マウスの座標取得
	if (mouse & MOUSE_INPUT_LEFT && !(mouseOld & MOUSE_INPUT_LEFT)) {	 //マウスの左ボタンが押されていたら

		// プレイヤーが死亡していないとき
		if (_player->GetHP() > 0)
		{
			// 敵がいないときだけ押せる
			if (eventState != EVENT_STATE::ENEMY && eventState != EVENT_STATE::EVE_MONS)
			{
				_menu->MenuButton_NonEnemy();
			}
			else
			{
				// 敵がいるとき(アイテムだけ)
				if (!_player->GetSkillBackFlg())
				{
					// スキル選択画面が押されていないときだけ
					_menu->MenuButton_Enemy();
				}
			}
		}
		else
		{
			// レベルをそのままにはじめからやり直す
			if (cursorPos.x >= 50 && cursorPos.x <= 50 + 400 && cursorPos.y >= 225 && cursorPos.y <= 225 + 130)
			{
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
				_event->SetNowEvent(0);
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
				// 音量の設定
				ChangeVolumeSoundMem(255, _gameBGM);
			}

			// あきらめてゲームオーバー画面へ
			if (cursorPos.x >= 450 && cursorPos.x <= 450 + 400 && cursorPos.y >= 225 && cursorPos.y <= 225 + 130)
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
		if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_UP]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_UP]))
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

	// 歩行回数がゴールに到達したらクリア画面へ移行
	if (_changeToClear)
	{
		return std::make_unique<GameClearScene>();
	}

	doorWalk();
	changeBGM();
	plDead();
	_menu->Update(this, _player, _monster[0], _cards);
	_event->UpDate(this, _player, _menu, _item, _monster[0], _cards);
	enemyItemDrop();
	_monster[0]->update();

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

	// 霧処理
	if (kiri[0] < 900.0f)
	{
		kiri[0] += 0.5f;
	}
	else
	{
		kiri[0] = -900.0f;
	}

	if (kiri[1] < 900.0f)
	{
		kiri[1] += 0.5f;
	}
	else
	{
		kiri[1] = -900.0f;
	}

	Draw();
	mouseOld = mouse;
	cardEffect();

	_player->UpDate();

	// 5ターン分経過
	if (_player->GetConditionTurn() <= 0)
	{
		_player->SetCondition(CONDITION::FINE);
		_player->SetConditionTurn(0);
	}

	pl_TurnEndAfter();
	EventUpdate();

	// 逃走アイテムが使用されたら敵を消す
	if (_menu->GetEscapeFlg())
	{
		_monster[0]->SetEnemyState(ENEMY_STATE::NON);
		_menu->SetEscapeFlg(false);
	}

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

	//角度変数に毎回delta加算
	//radianに直してsinにいれる
	//出た値に上下させたい幅を*
	//auto delta = 1.0f;
	//_degree += 1.0f * 25.0f;
	//auto radian = _degree * PI / 180.0;
	//auto sin = sinf(radian);
	//auto move = sin * 25.0f;

	if (shakeFlg)
	{
		// 画面を揺らす
		shakeDraw();
	}
	else
	{
		DrawRotaGraph(450 - _walkDirect, 300, 1.0f, 0, roadPNG[_plNowPoint], false);
		_shakeTime = 0.0f;
		_shakeChangeFlg = false;
	}

	// 行き止まりでないかつバック処理でないときは前に動く動作
	if (_plNowPoint != 3 && !backFlg)
	{
		if (moveFlg)
		{
			// 歩いてるときに揺れてる感じが出したい
			_degree += 1.0f * 7.0f;
			auto radian = _degree * PI / 180.0;
			auto sin = sinf(radian);
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
			DrawRotaGraph(450 - _walkDirect, move + 300, _doorExpand, 0, roadPNG[_plNowPoint], false);
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
				DrawGraph(0, 0, _whitePNG, true);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
		}
	}
	else
	{
		openDoor = true;
		DrawRotaGraph(450, 300, 1.0f, 0, roadPNG[_plNowPoint], false);
	}

	// 何もなしと敵以外の処理
	if (eventState != EVENT_STATE::ENEMY)
	{
		_event->Draw(this, _player, _menu, _item);
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
			if (plPosX == bossPos.x && plPosY == bossPos.y)
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
		if (plPosX == bossPos.x && plPosY == bossPos.y)
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

	// 霧表現
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawGraph(0 + kiri[0], 0, kiriPNG[0], true);
	DrawGraph(0 + kiri[1], 0, kiriPNG[1], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// プレイヤー
	// HPバー関連画像サイズ
	int pgx = 100;
	int pgy = 25;
	DrawExtendGraph(750, 450, 750 + pgx, 450 + pgy, _hpBarBack, true);
	DrawExtendGraph(750, 450, 750 + pgx * _player->GetHPBar(), 450 + pgy, _hpBarPl, true);

	// 右下案内表示
	DrawFormatString(750, 425, 0xffffff, "体力:%d / %d", _player->GetHP(), _player->GetMaxHP());
	if (_player->GetConditionTurn() != 0)
	{
		DrawFormatString(750, 480, 0xffffff, "毒回復まで:%d", _player->GetConditionTurn());
	}
	if (_menu->GetPowUp() != 0)
	{
		DrawFormatString(750, 505, 0xffffff, "攻撃強化:+%d", _menu->GetPowUp());
	}

	if (_plNowMark.x == 0 && _plNowMark.y <= 2)
	{
		// Sのマークに後で変更する
		DrawGraph(0 * 50, 550 - (0 * 50), chipPNG[0], true);
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
			_mapChipDrawOffset.y = 50 * (plPosY - 2);
		}

		if (plPosX <= 2)
		{
			_mapChipDrawOffset.x = 0;
		}
		else
		{
			_mapChipDrawOffset.x = 50 * (plPosX - 2);
		}

		if (std::get<0>(*v).y + 25 + _mapChipDrawOffset.y >= 450 && std::get<0>(*v).x + 25 - _mapChipDrawOffset.x <= 150)
		{
			DrawRotaGraph(std::get<0>(*v).x + 25 - _mapChipDrawOffset.x, std::get<0>(*v).y + 25 + _mapChipDrawOffset.y, 1.0, std::get<2>(*v), chipPNG[std::get<1>(*v)], true);
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

	DrawRotaGraph(_plNowMark.x * 50 + 25, 550 - (_plNowMark.y * 50) + 25, 1.0f, _directRota, directPNG, true);

	if (plPosX == bossPos.x - 1 && plPosY == bossPos.y)
	{
		// ボスの警告用画像
		DrawGraph(250, 100, bossEmergencyPNG, true);
	}

	if (_monster[0]->GetEnemyState() == ENEMY_STATE::EXIST)
	{
		DrawGraph(550, 0, _enemyInfoPNG, true);

		// 敵がいるときのみ描画
		DrawGraph(650, 150, _turnPNG[_cards->GetTurn()], true);

		// 敵(HPバー関連)
		DrawExtendGraph(600, 80, 600 + pgx, 80 + pgy, _hpBarBack, true);
		DrawExtendGraph(600, 80, 600 + pgx * _monster[0]->GetHPBar(), 80 + pgy, _hpBarEn, true);

		// 戦闘中以外は邪魔なので非表示で
		_cards->Draw(_player,_menu);
		_player->Draw(_menu);
	}

	_menu->Draw(_player,_item,_monster[0]);

	// 敵からのドロップアイテム
	if (_anounceFlg)
	{
		DrawGraph(600, 200, _getItemPNG, true);

		if (_menu->GetCanHaveItem() == 0)
		{
			// 持ち物満タンだからもてないよ
			DrawFormatString(600, 180, 0xffffff, "所持品がいっぱいだ");
		}
	}

	// 戦闘中の毒の描画(プレイヤーがカードを使ったときにくらう毒のダメージ)
	if (eventState == EVENT_STATE::ENEMY && _cards->GetCardsSyurui() != CARDS_SYURUI::NON)
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
		DrawGraph(50, 225, _retryPNG, true);

		// あきらめる
		DrawGraph(450, 225, _deadPNG, true);

		// 特定敵に出会う前に即死トラップで死んだとき
		if (!_event->GetEventMonsEncountFlg() && !_event->GetEventMonsEndFlg() && eventState == EVENT_STATE::TRAP)
		{
			DrawGraph(300, 0, _messageDeathPNG, true);
		}
		else if (_event->GetEventMonsEndFlg() && !monsterFlg)
		{
			// 敵もろとも即死トラップで死んだとき
			DrawGraph(300, 0, _messageDeathPNG2, true);
		}
	}

	ScreenFlip();
}

void GameScene::MouseClick_Go(const GameCtl& ctl)
{
	// 進むボタン(X:750,Y:345)
	// メニュー画面表示中は進むボタンを押せないようにする
	if (!_menu->GetMenuFlg() && _menu->GetMenu() == MENU::NON && eventState == EVENT_STATE::NON)
	{
		// キーボード操作(画像拡大最中は処理しない)
		if (_plNowPoint == 0 && (_doorExpand == 1.0f || _doorExpand >= 1.5f))
		{
			if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_UP]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_UP]))
			{
				Key();
			}
		}

		if (_plNowPoint == 1 && (_doorExpand == 1.0f || _doorExpand >= 1.5f))
		{
			if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_RIGHT]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_RIGHT]))
			{
				Key();
			}
		}

		if (_plNowPoint == 2 && (_doorExpand == 1.0f || _doorExpand >= 1.5f))
		{
			if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_LEFT]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_LEFT]))
			{
				Key();
			}
		}

		// バック処理
		if (eventState == EVENT_STATE::NON)
		{
			// 現在地がスタート地点でなければバック処理できる
			if (plPosX > 0 || plPosY > 0)
			{
				if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_DOWN]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_DOWN]))
				{
					backFlg = true;
					Key();
				}
			}
		}

		// T字路テスト
		if (_plDirect == PL_DIRECTION::RIGHT)
		{
			if (_plNowPoint == 4 && (_doorExpand == 1.0f || _doorExpand >= 1.5f))
			{
				_rightFlg = false;
				_leftFlg = false;
				if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_RIGHT]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_RIGHT]))
				{
					_rightFlg = true;
					Key();
				}
				if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_LEFT]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_LEFT]))
				{
					_leftFlg = true;
					Key();
				}
			}
		}

		if (_plDirect == PL_DIRECTION::LEFT)
		{
			if (_plNowPoint == 4 && (_doorExpand == 1.0f || _doorExpand >= 1.5f))
			{
				_rightFlg = false;
				_leftFlg = false;
				if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_RIGHT]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_RIGHT]))
				{
					_leftFlg = true;
					Key();
				}
				if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_LEFT]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_LEFT]))
				{
					_rightFlg = true;
					Key();
				}
			}
		}

		if (_plDirect == PL_DIRECTION::UP)
		{
			if (_plNowPoint == 4 && (_doorExpand == 1.0f || _doorExpand >= 1.5f))
			{
				_rightFlg = false;
				_leftFlg = false;
				if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_RIGHT]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_RIGHT]))
				{
					_rightFlg = true;
					Key();
				}
				if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_LEFT]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_LEFT]))
				{
					_leftFlg = true;
					Key();
				}
			}
		}

		if (_plDirect == PL_DIRECTION::DOWN)
		{
			if (_plNowPoint == 4 && (_doorExpand == 1.0f || _doorExpand >= 1.5f))
			{
				_rightFlg = false;
				_leftFlg = false;
				if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_RIGHT]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_RIGHT]))
				{
					_rightFlg = true;
					Key();
				}
				if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_LEFT]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_LEFT]))
				{
					_leftFlg = true;
					Key();
				}
			}
		}

		// トの字型(直進と右への道)
		if (_plNowPoint == 5 && (_doorExpand == 1.0f || _doorExpand >= 1.5f))
		{
			_rightFlg = false;
			_leftFlg = false;
			if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_UP]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_UP]))
			{
				Key();
			}
			if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_RIGHT]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_RIGHT]))
			{
				_rightFlg = true;
				Key();
			}
		}

		// トの字型(直進と左への道)
		if (_plNowPoint == 6 && (_doorExpand == 1.0f || _doorExpand >= 1.5f))
		{
			_rightFlg = false;
			_leftFlg = false;
			if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_UP]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_UP]))
			{
				Key();
			}
			if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_LEFT]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_LEFT]))
			{
				_leftFlg = true;
				Key();
			}
		}
	}
}

void GameScene::EventUpdate(void)
{
	// 次のイベントへ更新 
	// 番号が一致しているときにしか入ってこないようにする
	//if (walkCnt == _eventNum[_event->GetNowEvent()] && _monster[0]->GetEnemyState() == ENEMY_STATE::DEATH)
	//{
	//	// 今は8以上ないから8以降はここに入り続けるので数字がバグる
	//	//_nowEvent++;
	//
	//	eventState = EVENT_STATE::NON;
	//
	//	// プレイヤーnext_level - 敵経験値
	//	// next_level <= 0ならnowNumを+1して、ステータス更新
	//	_player->SetNextLevel(_player->GetNextLevel() - _monster[0]->GetKeikenti());
	//
	//	// 所持金を増やす
	//	_player->SetMoney(_player->GetMoney() + _monster[0]->GetMoney());
	//}

	if (eventState == EVENT_STATE::ENEMY)
	{
		if (_monster[0]->GetEnemyState() == ENEMY_STATE::DEATH)
		{
			_event->SetEvent(eventState);
		}
	}

	// for文で回せそう	
	for (auto eve = static_cast<int>(EVENT_STATE::YADO); eve <= static_cast<int>(EVENT_STATE::MAX);)
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

void GameScene::pl_TurnEndAfter(void)
{
	// プレイヤーのターンが終了
	if (_cards->GetTurn() <= 0 && !shakeFlg)
	{
		// プレイヤーのターンが終了したらここに飛んできて、敵のターンに代わる
		// 敵が攻撃してくるのでプレイヤーのHPを減らす処理をする

		if (!_onceFlg)
		{
			// ダメージ音
			PlaySoundMem(_soundSE[6], DX_PLAYTYPE_BACK, true);

			_onceFlg = true;
			/*防御の仕組み*/
			float a = (float)_cards->GetGuard() * 10.0f;	//30%
			float b = (100.0f - a) / 100.0f;	//100%-30%=70%/100% = 0.7
			float c = (float)_monster[0]->GetAttack() * b;	//敵の与えてくるDamage量*0.7=7	
			int d = _menu->GetEquipGuard() + _player->GetDifense() - (int)c;

			// +になったらプレイヤーのHPが回復しちゃう事件
			if (d >= 0)
			{
				d = 0;
			}

			// ノーダメージ
			if (_menu->GetNonDamageFlg())
			{
				d = 0;
			}

			// バリア値
			if (_player->GetBarrierNum() > 0)
			{
				_player->SetBarrierNum(_player->GetBarrierNum() + d);
			}
			else
			{
				// dには0またはマイナス値が入っているので加算処理でok
				_player->SetHP(_player->GetHP() + d);
			}

			_menu->SetNonDamageFlg(false);

			// 表示ターン数を0にして、画面揺らしに移行する
			shakeFlg = true;
		}
		//if (!shakeFlg)
		//{
			// 攻撃を受けた後はターンを復活させる(敵によって増減)
			//_cards->SetTurn(_monster[0]->GetMaxTurn());
			//_cards->SetGuard(0);
			//_onceFlg = false;
		//}
	}
}

void GameScene::pl_Attack(void)
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

void GameScene::pl_Heal(void)
{
	/*回復の仕組み*/
	// 回復したら回復量を0に戻す
	// 最大HPを超えないように気を付ける
	float one = (float)_cards->GetHeal() / 10.0f; //0.2
	float two = _player->GetMaxHP() * one;        //最大HP * 0.2 = 最大HPの2割
	_player->SetHP(_player->GetHP() + (int)two);  //2割を足す
	_cards->SetHeal(0);
}

void GameScene::shakeDraw(void)
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

	DrawRotaGraph(450 + _shackPos.x, 300 + _shackPos.y, 1.0f, 0, roadPNG[_plNowPoint], false);

	if (_shakeTime >= 15.0f)
	{
		if (eventState == EVENT_STATE::ENEMY)
		{
			// 攻撃を受けた後はターンを復活させる
			_cards->SetTurn(_monster[0]->GetMaxTurn());
			_cards->SetGuard(0);
			_onceFlg = false;
		}
		shakeFlg = false;
		_blinkCnt = 0;
		// 描画輝度を戻す
		SetDrawBright(255, 255, 255);
	}

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
		DrawGraph(0, 0, _whitePNG, true);
	}
}

void GameScene::doorWalk(void)
{
	// 歩行音からドア音までのつなぎ用
	// 1:再生中 0:再生していない
	if (CheckSoundMem(_soundSE[1]) == 1 && _soundWalk)
	{
		_soundWalk = true;
		_seCnt = 20;
	}
	else if (CheckSoundMem(_soundSE[1]) == 0 && _soundWalk)
	{
		if (_seCnt >= 0)
		{
			_seCnt--;
		}
		else
		{
			// ドア音
			PlaySoundMem(_soundSE[2], DX_PLAYTYPE_BACK, true);
			_soundWalk = false;
		}
	}
}

void GameScene::changeBGM(void)
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

void GameScene::plDead(void)
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

void GameScene::enemyItemDrop(void)
{
	// 敵がアイテムを落としたときの処理
	if (_monster[0]->GetDropFlg())
	{
		// 持ち物いっぱいだったけど使ったり捨てたりして持てるようになった時
		if (mouse & MOUSE_INPUT_LEFT && !(mouseOld & MOUSE_INPUT_LEFT)) {			 //マウスの左ボタンが押されていたら
			if (cursorPos.x >= 600 && cursorPos.x <= 600 + 150 && cursorPos.y >= 200 && cursorPos.y <= 200 + 75)
			{
				if (_anounceFlg && _menu->GetCanHaveItem() != 0)
				{
					// クリック音
					PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);

					// 持ち物が満タンじゃなければ持てる
					_menu->Setitem(_item->GetPair(static_cast<int>(_monster[0]->GetDrop()) - 1).second, _item->GetPair(static_cast<int>(_monster[0]->GetDrop()) - 1).first);
					_anounceFlg = false;
				}
			}
		}

		if (!_onceFlg)
		{
			if (_menu->GetCanHaveItem() != 0)
			{
				// 持ち物が満タンじゃなければ持てる
				_menu->Setitem(_item->GetPair(static_cast<int>(_monster[0]->GetDrop()) - 1).second, _item->GetPair(static_cast<int>(_monster[0]->GetDrop()) - 1).first);
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

void GameScene::cardEffect(void)
{
	auto lambdaPoison = [&]() {
		// 毒音
		PlaySoundMem(_soundSE[7], DX_PLAYTYPE_BACK, true);

		// 体力の1/5ぐらい削ろうかな
		_player->SetHP(_player->GetHP() - (float)_player->GetMaxHP() * (1.0f / 5.0f));
		// 1ターンマイナス
		_player->SetConditionTurn(_player->GetConditionTurn() - 1);
	};

	if (_cards->GetCardsSyurui() == CARDS_SYURUI::ATTACK)
	{
		// 攻撃カード音
		PlaySoundMem(_soundSE[3], DX_PLAYTYPE_BACK, true);

		// スキルチャージが0より大きいときは減らしていく
		if (_player->GetSkillCharge() > 0)
		{
			_player->SetSkillCharge(_player->GetSkillCharge() - 1);
		}

		if (_player->GetCondition() == CONDITION::POISON)
		{
			lambdaPoison();
		}
		pl_Attack();
		_cards->SetCardsSyurui(CARDS_SYURUI::NON);
		blinkFlg = true;
	}

	if (_cards->GetCardsSyurui() == CARDS_SYURUI::HEAL)
	{
		// 回復カード音
		PlaySoundMem(_soundSE[5], DX_PLAYTYPE_BACK, true);

		//// スキルチャージが0より大きいときは減らしていく
		if (_player->GetSkillCharge() > 0)
		{
			_player->SetSkillCharge(_player->GetSkillCharge() - 1);
		}

		if (_player->GetCondition() == CONDITION::POISON)
		{
			lambdaPoison();
		}
		pl_Heal();
		_cards->SetCardsSyurui(CARDS_SYURUI::NON);
	}

	if (_cards->GetCardsSyurui() == CARDS_SYURUI::GUARD)
	{
		// 防御カード音
		PlaySoundMem(_soundSE[4], DX_PLAYTYPE_BACK, true);

		// スキルチャージが0より大きいときは減らしていく
		if (_player->GetSkillCharge() > 0)
		{
			_player->SetSkillCharge(_player->GetSkillCharge() - 1);
		}

		if (_player->GetCondition() == CONDITION::POISON)
		{
			lambdaPoison();
		}
		_cards->SetCardsSyurui(CARDS_SYURUI::NON);
	}
}

void GameScene::Direct(void)
{
	// 行き止まりだったら進行方向にたいしてバック処理
	if (_plNowPoint == 3 || backFlg)
	{
		if (_plDirectOld == PL_DIRECTION::DOWN && (_plNowPoint == 4 || _plNowPoint == 7 || _plNowPoint == 8 || _plNowPoint == 0))
		{
			if (_plDirect == PL_DIRECTION::RIGHT)
			{
				_plDirect = PL_DIRECTION::LEFT;
			}
			else if (_plDirect == PL_DIRECTION::LEFT)
			{
				_plDirect = PL_DIRECTION::RIGHT;
			}
			else if (_plDirect == PL_DIRECTION::UP)
			{
				_plDirect = PL_DIRECTION::DOWN;
			}
			else if (_plDirect == PL_DIRECTION::DOWN)
			{
				_plDirect == PL_DIRECTION::UP;
			}
		}
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
			plPosX++;
		}

		_plNowPoint = _dungeonMap[plPosY][plPosX].second;

		// 方向だけ転換させる--------

		// T字路
		if (_plNowPoint == 4)
		{
			backFlg = false;
			_plDirect = _plDirectOld;
			//_plNowPoint = _plOldPoint;
			if (_plOldPoint == 5)
			{
				_plDirect = PL_DIRECTION::RIGHT;
				_directRota = PI / 2;
			}
			else if (_plDirect == PL_DIRECTION::RIGHT)
			{
				_directRota = PI / 2;
			}
			else if (_plDirect == PL_DIRECTION::LEFT)
			{
				_plDirect = PL_DIRECTION::RIGHT;
				//_directRota = PI + PI / 2;
				_directRota = PI / 2;
			}
			else if (_plDirect == PL_DIRECTION::DOWN)
			{
				_directRota = PI;
			}
			else if (_plDirect == PL_DIRECTION::UP)
			{
				_directRota = 0;
			}
			return;
		}

		// トの字型(直進と右への道)
		if (_plNowPoint == 5)
		{
			backFlg = false;
			//_plDirect = _plDirectOld;
			if (_plDirectOld == PL_DIRECTION::UP)
			{
				_directRota = 0;
				_plDirect = _plDirectOld;
				_plOldPoint = 5;
			}
			else if (_plDirectOld == PL_DIRECTION::RIGHT)
			{
				_directRota = 0;
				_plDirect = PL_DIRECTION::UP;
				_plOldPoint = 5;
			}
			return;
		}
		// トの字型(直進と左への道)
		if (_plNowPoint == 6)
		{
			backFlg = false;
			_plDirect = _plDirectOld;
			if (_plDirect == PL_DIRECTION::UP)
			{
				_directRota = 0;
			}
			return;
		}

		if (_plDirect == PL_DIRECTION::UP)
		{
			if ((_plNowPoint == 0 || _plNowPoint == 5) && !_rightFlg)	// 直進
			{
				backFlg = false;
				return;
			}
			else if (_plNowPoint == 1)
			{
				_plDirectOld = _plDirect;
				backFlg = false;
				_plDirect = PL_DIRECTION::LEFT;	// 左曲がり
				_leftFlg = true;
				_directRota = PI + PI / 2;
				return;
			}
			else if (_plNowPoint == 2)
			{
				_plDirectOld = _plDirect;
				backFlg = false;
				_plDirect = PL_DIRECTION::RIGHT;	// 右曲がり
				_rightFlg = true;
				_directRota = PI / 2;
				return;
			}
		}

		if (_plDirect == PL_DIRECTION::DOWN)
		{
			if (_plNowPoint == 0)	// 直進
			{
				backFlg = false;
				return;
			}
			else if (_plNowPoint == 1)
			{
				//_plDirectOld = _plDirect;
				backFlg = false;
				_plDirect = PL_DIRECTION::RIGHT;	// 右曲がり
				_rightFlg = true;
				_directRota = PI / 2;
				return;
			}
			else if (_plNowPoint == 2)
			{
				//_plDirectOld = _plDirect;
				backFlg = false;
				_plDirect = PL_DIRECTION::LEFT;	// 左曲がり
				_leftFlg = true;
				_directRota = PI + PI / 2;
				return;
			}
		}

		if (_plDirect == PL_DIRECTION::RIGHT)
		{
			if (_plNowPoint == 0)	// 直進
			{
				backFlg = false;
				return;
			}
			else if (_plNowPoint == 1)	
			{
				_plDirectOld = _plDirect;
				backFlg = false;
				_plDirect = PL_DIRECTION::UP;
				_directRota = 0.0f;
				return;
			}
			else if (_plNowPoint == 2)	
			{
				_plDirectOld = _plDirect;
				backFlg = false;
				_plDirect = PL_DIRECTION::DOWN;
				_directRota = PI;
				return;
			}
		}

		if (_plDirect == PL_DIRECTION::LEFT)
		{
			if (_plNowPoint == 0)	// 直進
			{
				backFlg = false;
				return;
			}
			else if (_plNowPoint == 1)	
			{
				_plDirectOld = _plDirect;
				backFlg = false;
				_plDirect = PL_DIRECTION::DOWN;
				_leftFlg = false;
				_directRota = PI;
				return;
			}
			else if (_plNowPoint == 2)	
			{
				_plDirectOld = _plDirect;
				backFlg = false;
				_plDirect = PL_DIRECTION::UP;
				_leftFlg = false;
				_directRota = 0.0f;
				return;
			}
		}
	}
	//------------------------------------------

	// T字路
	if (_plNowPoint == 4)
	{
		//_rightFlg = false;
		//_leftFlg = false;
		if (_rightFlg && _plDirect != PL_DIRECTION::UP && _plDirect != PL_DIRECTION::DOWN)
		{
			_plDirectOld = _plDirect;
			_plOldPoint = _plNowPoint;
			_plDirect = PL_DIRECTION::DOWN;
			plPosY--;
			_directRota = PI;
			return;
		}

		if (_leftFlg && _plDirect != PL_DIRECTION::UP && _plDirect != PL_DIRECTION::DOWN)
		{
			_plDirectOld = _plDirect;
			_plOldPoint = _plNowPoint;
			_plDirect = PL_DIRECTION::UP;
			plPosY++;
			_directRota = 0;
			return;
		}

		if (_rightFlg && _plDirect == PL_DIRECTION::UP)
		{
			_plDirectOld = _plDirect;
			_plOldPoint = _plNowPoint;
			_plDirect = PL_DIRECTION::RIGHT;
			plPosX++;
			_directRota = PI / 2;
			return;
		}

		if (_leftFlg && _plDirect == PL_DIRECTION::UP)
		{
			_plDirectOld = _plDirect;
			_plOldPoint = _plNowPoint;
			_plDirect = PL_DIRECTION::LEFT;
			plPosX--;
			_directRota = PI + PI / 2;
			return;
		}

		if (_rightFlg && _plDirect == PL_DIRECTION::DOWN)
		{
			_plDirectOld = _plDirect;
			_plOldPoint = _plNowPoint;
			_plDirect = PL_DIRECTION::LEFT;
			plPosX--;
			_directRota = PI + PI / 2;
			return;
		}

		if (_leftFlg && _plDirect == PL_DIRECTION::DOWN)
		{
			_plDirectOld = _plDirect;
			_plOldPoint = _plNowPoint;
			_plDirect = PL_DIRECTION::RIGHT;
			plPosX++;
			_directRota = PI / 2;
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
			_plDirectOld = _plDirect;
			_plOldPoint = _plNowPoint;
			_plDirect = PL_DIRECTION::RIGHT;	// 右曲がり
			_rightFlg = true;
			plPosX++;
			_directRota =  PI / 2;
			return;
		}
		else if (_plNowPoint == 2 || _plNowPoint == 6)
		{
			_plDirectOld = _plDirect;
			_plOldPoint = _plNowPoint;
			_plDirect = PL_DIRECTION::LEFT;	// 左曲がり
			_leftFlg = true;
			plPosX--;
			_directRota = PI + PI / 2;
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
			_plDirect = PL_DIRECTION::LEFT;	// 左曲がり
			_leftFlg = true;
			_plDirectOld = _plDirect;
			_plOldPoint = _plNowPoint;
			plPosX--;
			_directRota = PI + PI / 2;
			return;
		}
		else if (_plNowPoint == 2)
		{
			_plDirectOld = _plDirect;
			_plOldPoint = _plNowPoint;
			_plDirect = PL_DIRECTION::RIGHT;	// 右曲がり
			_rightFlg = true;
			plPosX++;
			_directRota = PI / 2;
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
			_plDirectOld = _plDirect;
			_plOldPoint = _plNowPoint;
			_plDirect = PL_DIRECTION::DOWN;
			_rightFlg = false;
			plPosY--;
			_directRota = PI;
			return;
		}
		else if (_plNowPoint == 2 && _rightFlg)	// 左曲がり(上)
		{
			_plDirectOld = _plDirect;
			_plOldPoint = _plNowPoint;
			_plDirect = PL_DIRECTION::UP;
			_rightFlg = false;
			plPosY++;
			_directRota = 0.0f;
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
			_plDirectOld = _plDirect;
			_plOldPoint = _plNowPoint;
			_plDirect = PL_DIRECTION::DOWN;
			_leftFlg = false;
			plPosY--;
			_directRota = PI;
			return;
		}
		else if (_plNowPoint == 1 && _leftFlg)	// 右曲がり(上)
		{
			_plDirectOld = _plDirect;
			_plOldPoint = _plNowPoint;
			_plDirect = PL_DIRECTION::UP;
			_leftFlg = false;
			plPosY++;
			_directRota = 0.0f;
			return;
		}
		else if (_plNowPoint == 2 && _leftFlg)	// 左曲がり(下)
		{
			_plDirectOld = _plDirect;
			_plOldPoint = _plNowPoint;
			_plDirect = PL_DIRECTION::DOWN;
			_leftFlg = false;
			plPosY--;
			_directRota = PI;
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
	_soundWalk = true;
	moveFlg = true;
	// プレイヤーが状態異常のとき
	if (_player->GetCondition() == CONDITION::POISON)
	{
		// ダメージ音
		PlaySoundMem(_soundSE[7], DX_PLAYTYPE_BACK, true);

		// 体力の1/5ぐらい削ろうかな
		_player->SetHP(_player->GetHP() - (float)_player->GetMaxHP() * (1.0f / 5.0f));
		// 1ターンマイナス
		_player->SetConditionTurn(_player->GetConditionTurn() - 1);

		shakeFlg = true;
	}

	// 扉が開いていたら再設定
	if (openDoor)
	{
		Direct();
		_plNowPoint = _dungeonMap[plPosY][plPosX].second;
		if (!_dungeonMap[plPosY][plPosX].first)
		{
			_mapVec.emplace_back(VECTOR2(plPosX * 50, 550 - (plPosY * 50)),_plNowPoint, _directRota);
			// 通ったことのある道はフラグがtrueになる仕組み
			_dungeonMap[plPosY][plPosX].first = true;
		}

		// クリック音
		PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);

		_monster[0]->SetEnemyState(ENEMY_STATE::NON);
		//doorFlg = true;
		moveFlg = false;
		openDoor = false;
		_degree = 0.0f;
		_doorExpand = 1.0f;
	}

	if (plPosX == bossPos.x && plPosY == bossPos.y)
	{
		eventState = EVENT_STATE::ENEMY;
	}
	else
	{
		int num = _plNowPoint;

		// 特定敵イベント中はカウンターを止める
		if (num < 7 && !_event->GetEventMonsEncountFlg())
		{
			if (_monsTimeCnt > 0)
			{
				_monsTimeCnt--;
			}
			else
			{
				eventState = EVENT_STATE::ENEMY;
				_monsTimeCnt = 30;
			}
		}
		else
		{
			switch (num)	// 0~5
			{
			case 7:
				eventState = EVENT_STATE::YADO;
				break;
			case 8:
				eventState = EVENT_STATE::SYOUNIN;
				break;
			case 9:
				eventState = EVENT_STATE::BUTTON;
				break;
			case 10:
				eventState = EVENT_STATE::CHEST;
				//chestFate = GetRand(2);	// 0 ~ 2
				break;
			case 11:
				eventState = EVENT_STATE::DRINK;
				break;
			case 12:
				eventState = EVENT_STATE::TRAP;
				break;
			case 13:
				if (!_event->GetEventMonsEndFlg())
				{
					eventState = EVENT_STATE::EVE_MONS;
				}
				else
				{
					eventState = EVENT_STATE::NON;
					_plNowPoint = 0;
				}
				break;
			case 14:
				eventState = EVENT_STATE::GOAL;
				//openDoor = true;
				break;
			default:
				eventState = EVENT_STATE::NON;
				break;
			}
		}
	}

	// 敵の出現(特定敵イベントでもここに入るようにする)
	if (eventState == EVENT_STATE::ENEMY)
	{
		monsterFlg = true;
		if (_monster[0]->GetEnemyState() == ENEMY_STATE::NON)
		{
			//doorFlg = false;
			moveFlg = false;

			// ボスならこっち
			if (plPosX == bossPos.x && plPosY == bossPos.y)
			{
				auto ene = 5;
				_monster[0]->SetEnemyNum(ene, _player->GetNowLevel());		// これで敵の情報をセットしている(ボス用)
				_cards->SetTurn(_monster[0]->GetMaxTurn());
			}
			else
			{
				// 敵は0~4まで
				auto ene = GetRand(4);
				_monster[0]->SetEnemyNum(ene, _player->GetNowLevel());		// これで敵の情報をセットしている(ランダムにする)
				_cards->SetTurn(_monster[0]->GetMaxTurn());
			}
		}
	}

	// 宿屋・商人・ボタン・宝箱の出現
	if (eventState != EVENT_STATE::NON && eventState != EVENT_STATE::ENEMY)
	{
		//doorFlg = false;
		moveFlg = false;
	}
}