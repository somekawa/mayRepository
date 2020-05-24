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
int GameScene::testx = 2;
int GameScene::testy = 0;
bool GameScene::monsterFlg = false;

#define PI 3.141592653589793

//struct {
//	char name[256];		// 名前
//	char setumei[256];	// 効果の説明
//	int  png;			// 画像
//	int itemCostMoney;	// 金額
//}item[static_cast<int>(ITEM::MAX)];

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
	for (auto v = vec.begin(); v != vec.end(); ++v)
	{
		std::vector<std::tuple<VECTOR2, int, float>> arr;
		std::vector<std::tuple<VECTOR2, int, float>>().swap(arr);
	}
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
			// クリアではないとき
			//if (walkCnt != _goalCnt)
			//{
				// 敵がいないときだけ押せる
				if (eventState != EVENT_STATE::ENEMY)
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

				//MouseClick_Go();
			//}
		}
		else
		{
			// レベルをそのままにはじめからやり直す
			if (cursorPos.x >= 50 && cursorPos.x <= 50 + 400 && cursorPos.y >= 225 && cursorPos.y <= 225 + 130)
			{
				// 場所をスタート地点に戻す
				testx = 0;
				testy = 0;
				plNowPoint = numkai[testy][testx].second;
				directRota = 0.0f;
				_plDirect = PL_DIRECTION::UP;
				_plDirectOld = PL_DIRECTION::UP;
				leftFlg = false;
				rightFlg = false;

				// イベント状態の初期化
				_event->SetEventMonsEncountFlg(false);
				_event->SetEventMonsFlg(false);
				walkCnt = 0;
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

		_player->ClickUpDate(_monster[0],_menu,this);
		//// スキル使用可能時のマウスクリック位置とアイコン(円)との当たり判定
		//if (_skillFlg)
		//{
		//	// 782,564(アイコン描画位置)
		//	float a = cursorPos.x - 782;
		//	float b = cursorPos.y - 564;
		//	float c = sqrt(a * a + b * b);
		//
		//	// 当たり判定(当たっているとき)
		//	if (c <= 34)
		//	{
		//		// スキルはターン消費なしで行える動作
		//
		//		// 攻撃系(基礎攻撃力*10+武器威力で一定のダメージを与えられる)
		//		_monster[0]->Damage(_player->GetAttackDamage() * 10 + _menu->GetEquipDamage());
		//
		//		// フラグと回数を元に戻す
		//		_skillFlg = false;
		//		_skillCharge = 1;
		//	}
		//}

	}

	// ゴール処理
	if (eventState == EVENT_STATE::GOAL)
	{
		// 扉描画時にUPキーで出口処理
		if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_UP]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_UP]))
		{
			_openDoor = true;
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
	_menu->Update(this,_player, _monster[0],_cards);
	_event->UpDate(this, _player, _menu, _item, _monster[0],_cards);
	enemyItemDrop();
	
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

	Draw();	
	mouseOld = mouse;
	cardEffect();

	_player->UpDate();
	//// スキルが使用可能な時にフラグを立てて、当たり判定を行う
	//if (_skillCharge <= 0)
	//{
	//	_skillFlg = true;
	//}

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
	//doorFlg = true;
	moveFlg = false;
	_openDoor = false;
	_doorExpand = 1.0f;
	_degree = 0.0f;
	_doorOpenTiming = 20;
	_lastDoorExpand = 1.5f;
	_lastmoveTime = 0;
	_changeToClear = false;

	// イベント関係
	walkCnt = 0;
	eventState = EVENT_STATE::NON;
	_eventChoiceNum = 0;
	for (int i = 0; i <= 2; i++)
	{
		_eventChoiceNumOld[i] = 0;
	}
	//_eventChoiceNumOld[0] = 0;
	//_eventChoiceNumOld[1] = 0;
	//_eventChoiceNumOld[2] = 0;

	int FileHandle;
	if (SelectScene::modeTest == MODE::NORMAL)
	{
		//ファイルを読み込む
		FileHandle = FileRead_open("csv/eventData_NORMAL.csv");
		if (FileHandle == NULL)
		{
			return false; //エラー時の処理
		}
	}
	else if (SelectScene::modeTest == MODE::HARD)
	{
		//ファイルを読み込む
		FileHandle = FileRead_open("csv/eventData_HARD.csv");
		if (FileHandle == NULL)
		{
			return false; //エラー時の処理
		}
	}

	//for (int i = 0; i < EVENT_NUM; i++)
	//{
	//	FileRead_scanf(FileHandle, "%d", &_eventNum[i]);
	//}

	for (int i = 0; i < static_cast<int>(SelectScene::modeTest); i++)
	{
		FileRead_scanf(FileHandle, "%d", &_eventNum[i]);
		// -1が末尾であるという設定にしている
		if (_eventNum[i] == -1)
		{
			_bossEventNum = _eventNum[i - 1];
			//ファイルを閉じる
			FileRead_close(FileHandle);
		}
	}

	// ボスイベントの設定(最後のイベントのときに出るようにする)
	//_bossEventNum = _eventNum[static_cast<int>(SelectScene::modeTest) - 1];
	//_goalCnt = _bossEventNum + 2;
	//
	//ファイルを閉じる
	//FileRead_close(FileHandle);
	//
	// ボスイベントの設定(最後のイベントのときに出るようにする)
	//_bossEventNum = _eventNum[EVENT_NUM-1];
	
	//_goalCnt = _bossEventNum + 2;		// ボス撃破後した+2でゴール

	// 画面揺らし関係
	shakeFlg = false;
	_shakeChangeFlg = false;
	_shakeTime = 0.0f;
	_shackPos = { 0,0 };

	// アイテム関係
	//ファイルを読み込む
	//auto FileHandle_item = FileRead_open("csv/itemData.csv");
	//if (FileHandle_item == NULL)
	//{
	//	return -1; //エラー時の処理
	//}
	//
	//std::string a = "image/";
	//std::string b = ".png";
	//
	//for (int i = 0; i <= 4; i++)
	//{
	//	// 画像名 説明 費用
	//	FileRead_scanf(FileHandle_item, "%[^,],%[^,],%d", item[i].name, item[i].setumei, &item[i].itemCostMoney);
	//	auto d = a + item[i].name + b;
	//	char* e = item[0].name;
	//	char* f = item[1].name;
	//	char* g = item[2].name;
	//
	//	item[i].png = LoadGraph(d.c_str());
	//}
	//
	////ファイルを閉じる
	//FileRead_close(FileHandle_item);
	//
	//// これで、itemがpotionならこの画像というのができる。はず。
	////item_pair[0].first = itempotionPNG;
	////item_pair[0].second = ITEM::POTION;
	////_itemPos[0] = { 290,100 };
	////
	////item_pair[1].first = itempotionPNG;
	////item_pair[1].second = ITEM::POTION;
	////_itemPos[1] = { 390,100 };
	////
	//// x : 234234をつくる
	////0%3=0+2=2
	////1%3=1+2=3
	////2%3=2+2=4
	////3%3=0+2
	////4%3=1+2
	//// y : 111222333をつくる
	////0/3=0+1=1
	////1/3=0+1=1
	////2/3=0+1=1
	////3/3=1+1=2
	//
	//// これで、itemがpotionならこの画像というのができる。はず。
	//// 商人の持ち物的な
	//for (int i = 0; i <= 3; i++)
	//{
	//	item_pair[i].first = item[i].png;
	//	item_pair[i].second = static_cast<ITEM>(static_cast<int>(ITEM::POTION) + i);
	//	_itemPos[i] = { (((i % 3) + 2) * 100) + 90,((i / 3) + 1) * 100 };
	//}
	//
	//// 敵の魂的な
	//auto c = a + item[4].name + b;
	//item[4].png = LoadGraph(c.c_str());
	//item_pair[4].first = item[4].png;
	//item_pair[4].second = ITEM::ENEMY_1;
	//
	//// 装備関係
	//_equipDamage = 0;
	//_equipGuard = 0;

	// 点滅関係
	_blinkCnt = 0;
	blinkFlg = false;

	// その他
	_onceFlg = false;
	_anounceFlg = false;
	_plDeadChangeWinColor = 255;
	_poisonCnt = 256;

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


	// 本当は[0][0]に1が入るみたいにする
	// [0][0]に1 [0][1]に3が入るテストを行う
	//ファイルを読み込む
	// num[y][x]
	// int num[3][3];
	auto testFileHandle = FileRead_open("csv/test.csv");
	if (testFileHandle == NULL)
	{
		return false; //エラー時の処理
	}

	for (int i = 0; i < 10; i++)
	{
		FileRead_scanf(testFileHandle, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", &numkai[i][0].second, &numkai[i][1].second, &numkai[i][2].second, &numkai[i][3].second, &numkai[i][4].second, &numkai[i][5].second, &numkai[i][6].second, &numkai[i][7].second, &numkai[i][8].second, &numkai[i][9].second);
	}

	//ファイルを閉じる
	FileRead_close(testFileHandle);

	// 3が入るから、y方向に1ずれてx方向は移動なし
	// つまり、配列は[y][x]の順になっている
	//int aa = num[0][1];

	testx = 0;
	testy = 0;
	plNowPoint = numkai[testy][testx].second;

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

	// ゲージ
	std::string gauge_back = "image/gauge_back.png";
	_gaugeBackPNG = LoadGraph(gauge_back.c_str());
	std::string gauge = "image/gauge.png";
	_gaugePNG = LoadGraph(gauge.c_str());

	// 扉
	//std::string door = "image/door.png";
	//LoadDivGraph(door.c_str(), 2, 2, 1, 204, 292, doorPNG);
	std::string room_0 = "image/room_0.png";
	std::string room_1 = "image/room_1.png";
	std::string room_2 = "image/room_2.png";
	_room[0] = LoadGraph(room_0.c_str());
	_room[1] = LoadGraph(room_1.c_str());
	_room[2] = LoadGraph(room_2.c_str());

	// ダンジョン
	// 直進
	std::string dan_go = "image/dan_go.png";
	//straightPNG = LoadGraph(dan_go.c_str());
	// 右折のみ
	std::string dan_right = "image/dan_right.png";
	//rightPNG = LoadGraph(dan_right.c_str());
	// 左折のみ
	std::string dan_left = "image/dan_left.png";
	//leftPNG = LoadGraph(dan_left.c_str());
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

	//roadPNG[5] = LoadGraph(dan_stop.c_str());


	// 現在地用▲マーク
	std::string direct = "image/direct.png";
	directPNG = LoadGraph(direct.c_str());
	
	// マップチップ
	std::string mapchip = "image/mapchip/mapchip.png";
	std::string mapchip_stop = "image/mapchip/chip_3.png";
	std::string mapchip_danger = "image/mapchip/chip_danger.png";

	//chipPNG[0] = LoadGraph(chip_go.c_str());

	LoadDivGraph(mapchip.c_str(), 7, 7, 1, 50, 50, chipPNG);
	for (int i = 7; i < 13; i++)
	{
		chipPNG[i] = LoadGraph(mapchip_stop.c_str());
	}

	chipPNG[13] = LoadGraph(mapchip_danger.c_str());
	chipPNG[14] = LoadGraph(mapchip_danger.c_str());

	// 進むの文字
	//std::string walk = "image/walk.png";
	//_walkPNG = LoadGraph(walk.c_str());

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

	//// スキルアイコン
	//std::string skillicon = "image/skillicon.png";
	//_skillIconPNG = LoadGraph(skillicon.c_str());
	//
	//// スキル使えるというアナウンス
	//std::string chargeAnnounce = "image/chargeAnnounce.png";
	//_skillAnnouncePNG = LoadGraph(chargeAnnounce.c_str());
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

	// 扉
	// 敵が出ないときは扉を出す
	//if (doorFlg || _monster[0]->GetEnemyState() == ENEMY_STATE::DEATH)
	//{
		//DrawRotaGraph(300 + 204 / 2, 100 + 292 / 2, 1.0f, 0, doorPNG[0], false);
		//DrawRotaGraph(450, 300, 1.0f, 0, _room[0], false);
	//}

	if (shakeFlg)
	{
		// 画面を揺らす
		shakeDraw();
	}
	else
	{
		//if (walkCnt != _goalCnt)
		//{
			//DrawRotaGraph(450, 300, 1.0f, 0, _room[0], false);
			//DrawRotaGraph(450, 300, 1.0f, 0, straightPNG, false);
			//DrawRotaGraph(450-testCnt, 300, 1.0f, 0, rightPNG, false);
			//DrawRotaGraph(450 - testCnt, 300, 1.0f, 0, roadPNG[1], false);
			DrawRotaGraph(450 - testCnt, 300, 1.0f, 0, roadPNG[plNowPoint], false);
		//}
		_shakeTime = 0.0f;
		_shakeChangeFlg = false;
	}

	// 行き止まりでないかつバック処理でないときは前に動く動作
	if (plNowPoint != 3 && !_backFlg)
	{
		if (moveFlg)
		{
			//doorFlg = false;

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
				if (plNowPoint == 1)
				{
					testCnt += 2;
				}
				else if (plNowPoint == 2)	// 左折なら減算
				{
					testCnt -= 2;
				}
			}
			else
			{
				if (_doorOpenTiming <= 0)
				{
					move = 0.0f;
					_openDoor = true;
				}
				else
				{
					_doorOpenTiming--;
				}
			}

			//DrawRotaGraph(450, move + 300, _doorExpand, 0, _room[0], false);
			//DrawRotaGraph(450, move + 300, _doorExpand, 0,straightPNG, false);
			//DrawRotaGraph(450 - testCnt, move + 300, _doorExpand, 0,rightPNG, false);
			//DrawRotaGraph(450 - testCnt, move + 300, _doorExpand, 0, roadPNG[1], false);
			DrawRotaGraph(450 - testCnt, move + 300, _doorExpand, 0, roadPNG[plNowPoint], false);
		}

		if (_openDoor)
		{
			//DrawRotaGraph(340 + 204 / 2, 100 + 292 / 2, 1.5f, 0, doorPNG[1], false);

			// ゴール前の扉を光らせる
			if (/*walkCnt == _goalCnt ||*/ eventState == EVENT_STATE::GOAL)
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
			else
			{
				//DrawRotaGraph(450, 300, 1.5f, 0, _room[1], false);
				//DrawRotaGraph(450, 300, 1.5f, 0, straightPNG, false);
				//DrawRotaGraph(450, 300, 1.5f, 0, rightPNG, false);
			}
		}
	}
	else
	{
		_openDoor = true;
		DrawRotaGraph(450, 300, 1.0f, 0, roadPNG[plNowPoint], false);
	}

	// 何もなしと敵以外の処理
	if (/*eventState != EVENT_STATE::NON &&*/ eventState != EVENT_STATE::ENEMY)
	{
		_event->Draw(this, _player, _menu, _item);
	}

	//// 何もなしの処理
	//if (eventState == EVENT_STATE::NON)
	//{
	//	// 進む
	//	DrawGraph(750, 345, _walkPNG, true);
	//}

	// ゲージ
	//DrawExtendGraph(800, 25, 800 + 50, 25 + 300, _gaugePNG, true);
	//DrawExtendGraph(800, 25, 800 + 50, 25 + 300 * (1.0f - ((float)walkCnt / (float)(_goalCnt))), _gaugeBackPNG, true);

	// テスト表示
	//DrawFormatString(0, 240, GetColor(255, 255, 255),"現在のレベルは%dです",_player->GetNowLevel());
	//DrawFormatString(0, 260, GetColor(255, 255, 255), "次のレベルまでの経験値は%dです", _player->GetNextLevel());
	//DrawFormatString(0, 280, GetColor(255, 255, 255), "所持金は%d円です", _player->GetMoney());
	//DrawFormatString(0, 300, GetColor(255, 255, 255), "毒回復まであと%dです", _player->GetConditionTurn());
	//DrawFormatString(0, 320, GetColor(255, 255, 255), "強化はあと%dです", _menu->GetPowUp());

	//_monster[0]->Draw();

	// 敵に攻撃したら敵が点滅する
	// それ以外は通常表示
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
			if (walkCnt == _bossEventNum)
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
		if (/*walkCnt != _bossEventNum*/testx != bossPos.x && testy != bossPos.y)
		{
			if ((eventState == EVENT_STATE::NON) || (eventState == EVENT_STATE::ENEMY))
			{
				_monster[0]->Draw();
			}
		}
		else
		{
			if (eventState == EVENT_STATE::ENEMY && _monster[0]->GetEnemyState() != ENEMY_STATE::DEATH)
			{
				_monster[0]->BossDraw();
			}
		}
	}

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

	if (_monster[0]->GetEnemyState() == ENEMY_STATE::EXIST)
	{
		DrawGraph(550, 0, _enemyInfoPNG, true);

		// 敵がいるときのみ描画
		DrawGraph(650, 150, _turnPNG[_cards->GetTurn()], true);

		// テスト表示
		//DrawFormatString(0, 270, GetColor(255, 255, 255), "待機ターンはあと%dです", _cards->GetTurn());

		// 敵(HPバー関連)
		DrawExtendGraph(600, 80, 600 + pgx, 80 + pgy, _hpBarBack, true);
		DrawExtendGraph(600, 80, 600 + pgx * _monster[0]->GetHPBar(), 80 + pgy, _hpBarEn, true);

		// 戦闘中以外は邪魔なので非表示で
		_cards->Draw(_player,_menu);		// カードの情報	と カードのDraw

		_player->Draw(_menu);

		//if (_skillCharge != 0)
		//{
		//	// スキルチャージ時間
		//	DrawFormatString(750, 530, 0xffffff, "スキルまで:%d", _skillCharge);
		//}
		//else
		//{
		//	// 782,564
		//	DrawRotaGraph(750 + 32, 530 + 32, 1.0f, 0, _skillIconPNG, true);
		//	DrawGraph(820, 530, _skillAnnouncePNG, true);
		//}

		//// カード情報の表示
		//// GetHealとかの数字はまだusecardの関数に入る前なので使えない
		//if (_cards->GetCardsPos().y > 0 && _cards->GetCardsPos().y <= 300)
		//{
		//	std::string moji;
		//	if (static_cast<int>(_cards->GetCardMem()) == 1 || static_cast<int>(_cards->GetCardMem()) == 2 || static_cast<int>(_cards->GetCardMem()) == 3)
		//	{
		//		moji = "攻撃";
		//		//auto a = (static_cast<int>(_cards->GetCardMem()) + (static_cast<int>(_cards->GetCardMem()) - 1)) * (static_cast<int>(_cards->GetCardMem()) + (static_cast<int>(_cards->GetCardMem()) - 1));
		//		DrawFormatString(_cards->GetCardsPos().x - 80, _cards->GetCardsPos().y + 90, GetColor(255, 255, 255), "%s:敵に%dのダメージ", moji.c_str(), (static_cast<int>(_cards->GetCardMem()) + (static_cast<int>(_cards->GetCardMem()) - 1)) * (static_cast<int>(_cards->GetCardMem()) + (static_cast<int>(_cards->GetCardMem()) - 1)) + _player->GetAttackDamage() + _menu->GetEquipDamage() + _menu->GetPowUp());
		//	}
		//	if (static_cast<int>(_cards->GetCardMem()) == 4 || static_cast<int>(_cards->GetCardMem()) == 5 || static_cast<int>(_cards->GetCardMem()) == 6)
		//	{
		//		moji = "防御";
		//		DrawFormatString(_cards->GetCardsPos().x - 80, _cards->GetCardsPos().y + 90, GetColor(255, 255, 255), "%s:ダメージ%d割の軽減", moji.c_str(), (((static_cast<int>(_cards->GetCardMem()) % 4)+1) + (static_cast<int>(_cards->GetCardMem()) % 4)));
		//	}
		//	if (static_cast<int>(_cards->GetCardMem()) == 7 || static_cast<int>(_cards->GetCardMem()) == 8 || static_cast<int>(_cards->GetCardMem()) == 9)
		//	{
		//		moji = "回復";
		//		DrawFormatString(_cards->GetCardsPos().x - 80, _cards->GetCardsPos().y + 90, GetColor(255, 255, 255), "%s:体力%d割の回復", moji.c_str(), (((static_cast<int>(_cards->GetCardMem()) % 7) + 1) + (static_cast<int>(_cards->GetCardMem()) % 7)));
		//	}
		//}
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
		if (!_event->GetEventMonsEncountFlg() && !_event->GetEventMonsEndFlg())
		{
			DrawGraph(300, 0, _messageDeathPNG, true);
		}
		else if (_event->GetEventMonsEndFlg() && !monsterFlg)
		{
			// 敵もろとも即死トラップで死んだとき
			DrawGraph(300, 0, _messageDeathPNG2, true);
		}
	}

	// マップチップ描画テスト
	// 直進用チップ
	//DrawGraph(0, 500, chipPNG[0], true);
	// ただ、これだと1つのチップが動くだけ(自分の現在地用になら使える)
	// 過去に通った道をmapとかなんとかでためていく
	// どっちか一致してたら作ってくれないのかよ
	// 今まで歩いたxとyと一致しなかったらつくるようにする。

	// 現在地
	//DrawGraph(testx*50,550 - (testy*50), chipPNG[0], true);
	//int offset = 50;
	// 最初の1歩(スタート地点)
	//DrawGraph(0, 550, chipPNG[0], true);
	if (plNowMark.x == 0 && plNowMark.y <= 2)
	{
		// Sのマークに後で変更する
		DrawGraph(0 * 50, 550 - (0 * 50), chipPNG[0], true);
	}
	// 現在地を保存していく
	for (auto v = vec.begin(); v != vec.end(); ++v)
	{
		if (testy <= 2)
		{
			offset.y = 0;
			//DrawRotaGraph(std::get<0>(*v).x + 25, std::get<0>(*v).y + 25, 1.0, std::get<2>(*v), chipPNG[std::get<1>(*v)], true);
		}
		else
		{
			offset.y = 50 * (testy - 2);
			//DrawRotaGraph(std::get<0>(*v).x + 25, std::get<0>(*v).y + 25 + (offset.y), 1.0, std::get<2>(*v), chipPNG[std::get<1>(*v)], true);
		}

		if (testx <= 2)
		{
			offset.x = 0;
		}
		else
		{
			offset.x = 50 * (testx - 2);
		}

		if (std::get<0>(*v).y + 25 + offset.y >= 450 && std::get<0>(*v).x + 25 - offset.x <= 150)
		{
			DrawRotaGraph(std::get<0>(*v).x + 25 - offset.x, std::get<0>(*v).y + 25 + offset.y, 1.0, std::get<2>(*v), chipPNG[std::get<1>(*v)], true);
		}
		//DrawRotaGraph((*v).first.x+25, (*v).first.y+25,1.0,0, chipPNG[(*v).second], true);
		//DrawRotaGraph(std::get<0>(*v).x + 25, std::get<0>(*v).y + 25, 1.0, std::get<2>(*v), chipPNG[std::get<1>(*v)], true);
	}

	// 現在地
	if (testy <= 2)
	{
		plNowMark.y = testy;
		//DrawRotaGraph(testx * 50 + 25, 550 - (testy * 50) + 25, 1.0f, directRota, directPNG, true);
	}
	else
	{
		plNowMark.y = 2;
		//DrawRotaGraph(testx * 50 + 25, 550 - (2 * 50) + 25, 1.0f, directRota, directPNG, true);
	}
	if (testx <= 2)
	{
		plNowMark.x = testx;
		//DrawRotaGraph(testx * 50 + 25, 550 - (testy * 50) + 25, 1.0f, directRota, directPNG, true);
	}
	else
	{
		plNowMark.x = 2;
		//DrawRotaGraph(testx * 50 + 25, 550 - (2 * 50) + 25, 1.0f, directRota, directPNG, true);
	}

	DrawRotaGraph(plNowMark.x * 50 + 25, 550 - (plNowMark.y * 50) + 25, 1.0f, directRota, directPNG, true);

	ScreenFlip();
}

void GameScene::MouseClick_Go(const GameCtl& ctl)
{
	// 進むボタン(X:750,Y:345)
	// メニュー画面表示中は進むボタンを押せないようにする
	if (!_menu->GetMenuFlg() && _menu->GetMenu() == MENU::NON)
	{
		//if (cursorPos.x >= 750 && cursorPos.x <= 750 + 150 && cursorPos.y >= 345 && cursorPos.y <= 345 + 75)
		//{

		// キーボード操作(画像拡大最中は処理しない)
		if (plNowPoint == 0 && (_doorExpand == 1.0f || _doorExpand >= 1.5f))
		{
			if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_UP]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_UP]))
			{
				TestKey();
				//_plDirect = PL_DIRECTION::UP;
			}
		}

		if (plNowPoint == 1 && (_doorExpand == 1.0f || _doorExpand >= 1.5f))
		{
			if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_RIGHT]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_RIGHT]))
			{
				TestKey();
				//_plDirect = PL_DIRECTION::RIGHT;
			}
		}

		if (plNowPoint == 2 && (_doorExpand == 1.0f || _doorExpand >= 1.5f))
		{
			if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_LEFT]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_LEFT]))
			{
				TestKey();
				//_plDirect = PL_DIRECTION::LEFT;
			}
		}

		// バック処理
		if (eventState == EVENT_STATE::NON)
		{
			if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_DOWN]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_DOWN]))
			{
				_backFlg = true;
				TestKey();
				//if (eventState != EVENT_STATE::ENEMY && eventState != EVENT_STATE::NON)
				//{
					//eventState = EVENT_STATE::NON;
					//_event->SetEvent(EVENT_STATE::NON);
				//}
			}
		}

		// T字路テスト
		if (_plDirect == PL_DIRECTION::RIGHT)
		{
			if (plNowPoint == 4 && (_doorExpand == 1.0f || _doorExpand >= 1.5f))
			{
				rightFlg = false;
				leftFlg = false;
				if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_RIGHT]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_RIGHT]))
				{
					rightFlg = true;
					TestKey();
				}
				if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_LEFT]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_LEFT]))
				{
					leftFlg = true;
					TestKey();
				}
			}
		}

		if (_plDirect == PL_DIRECTION::LEFT)
		{
			if (plNowPoint == 4 && (_doorExpand == 1.0f || _doorExpand >= 1.5f))
			{
				rightFlg = false;
				leftFlg = false;
				if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_RIGHT]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_RIGHT]))
				{
					leftFlg = true;
					TestKey();
				}
				if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_LEFT]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_LEFT]))
				{
					rightFlg = true;
					TestKey();
				}
			}
		}

		if (_plDirect == PL_DIRECTION::UP)
		{
			if (plNowPoint == 4 && (_doorExpand == 1.0f || _doorExpand >= 1.5f))
			{
				rightFlg = false;
				leftFlg = false;
				if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_RIGHT]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_RIGHT]))
				{
					rightFlg = true;
					TestKey();
				}
				if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_LEFT]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_LEFT]))
				{
					leftFlg = true;
					TestKey();
				}
			}
		}

		if (_plDirect == PL_DIRECTION::DOWN)
		{
			if (plNowPoint == 4 && (_doorExpand == 1.0f || _doorExpand >= 1.5f))
			{
				rightFlg = false;
				leftFlg = false;
				if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_RIGHT]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_RIGHT]))
				{
					rightFlg = true;
					TestKey();
				}
				if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_LEFT]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_LEFT]))
				{
					leftFlg = true;
					TestKey();
				}
			}
		}

		// トの字型(直進と右への道)
		if (plNowPoint == 5 && (_doorExpand == 1.0f || _doorExpand >= 1.5f))
		{
			rightFlg = false;
			leftFlg = false;
			if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_UP]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_UP]))
			{
				TestKey();
			}
			if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_RIGHT]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_RIGHT]))
			{
				rightFlg = true;
				TestKey();
			}
		}

		// トの字型(直進と左への道)
		if (plNowPoint == 6 && (_doorExpand == 1.0f || _doorExpand >= 1.5f))
		{
			rightFlg = false;
			leftFlg = false;
			if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_UP]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_UP]))
			{
				TestKey();
			}
			if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_LEFT]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_LEFT]))
			{
				leftFlg = true;
				TestKey();
			}
		}

		//if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_F2]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_F2]))
		//{
			//
			//testCnt = 0;
			//
			//// 敵が出ないときは扉を出す
			//if (walkCnt != _eventNum[_event->GetNowEvent()] && !moveFlg)
			//{
			//	// クリック音
			//	PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
			//
			//	// 歩行音
			//	PlaySoundMem(_soundSE[1], DX_PLAYTYPE_BACK, true);
			//	_soundWalk = true;
			//
			//	moveFlg = true;
			//	walkCnt++;
			//	// プレイヤーが状態異常のとき
			//	if (_player->GetCondition() == CONDITION::POISON)
			//	{
			//		// ダメージ音
			//		PlaySoundMem(_soundSE[6], DX_PLAYTYPE_BACK, true);
			//
			//		// 体力の1/5ぐらい削ろうかな
			//		_player->SetHP(_player->GetHP() - (float)_player->GetMaxHP() * (1.0f / 5.0f));
			//		// 1ターンマイナス
			//		_player->SetConditionTurn(_player->GetConditionTurn() - 1);
			//
			//		shakeFlg = true;
			//	}
			//}
			//
			//// 扉が開いていたら再設定
			//if (_openDoor)
			//{
			//	//if (plNowPoint == 0)
			//	//{
			//	//	testy++;
			//	//}
			//	//else if (plNowPoint == 1)
			//	//{
			//	//	_plDirect = PL_DIRECTION::RIGHT;
			//	//	rightFlg = true;
			//	//	testx++;
			//	//}				
			//
			//	TestDirect();
			//	//if (!numkai[testy][testx].first)
			//	//{
			//	//	movePos[aaab] = VECTOR2(testx * 50, 550 - (testy * 50));
			//	//	aaab++;
			//	//}
			//	plNowPoint = numkai[testy][testx].second;
			//	//numkai[testy][testx].first = true;
			//
			//	if (!numkai[testy][testx].first)
			//	{
			//		vec.emplace_back(VECTOR2(testx * 50, 550 - (testy * 50)));
			//		numkai[testy][testx].first = true;
			//	}
			//
			//	// 通ったことのある道はフラグがtrueになる仕組み
			//	// フラグがfalse(通ったことがない)なら追加する
			//	//if (!numkai[testy][testx].first)
			//	//{
			//		//mapTest.insert(std::make_pair(testx, testy));
			//		//aaab++;
			//		//numkai[testy][testx].first = true;
			//	//}
			//
			//	// クリック音
			//	PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
			//
			//	_monster[0]->SetEnemyState(ENEMY_STATE::NON);
			//	//doorFlg = true;
			//	moveFlg = false;
			//	_openDoor = false;
			//	_degree = 0.0f;
			//	_doorExpand = 1.0f;
			//}
			//
			//// イベントマスでランダムに数値を出し、対応した数値でイベントを発生させる
			//// ランダムで数字を出して、出た数字で出現させるものを決める
			//// 敵との遭遇回数をふやしたかったら数字の0~3までをenemyとかにすればいいかもしれない
			//if (walkCnt == _eventNum[_event->GetNowEvent()] && eventState == EVENT_STATE::NON && !moveFlg)
			//{
			//	int randNum = GetRand(5);
			//
			//	if (walkCnt == _bossEventNum)
			//	{
			//		randNum = 0;
			//	}
			//
			//	// 前回のイベント内容と被っているならとりま敵にする
			//	if (randNum == _eventChoiceNumOld[0])
			//	{
			//		randNum = 0;
			//	}
			//
			//	// 敵以外のイベント同士で被っているときは敵にする
			//	if (_eventChoiceNumOld[1] != 0 && _eventChoiceNumOld[2] != 0 && _eventChoiceNumOld[1] == _eventChoiceNumOld[2])
			//	{
			//		randNum = 0;
			//	}
			//
			//	// 直近3回のイベントに敵が出現していないときは、敵のイベントをする
			//	if (_eventChoiceNumOld[0] != 0 && _eventChoiceNumOld[1] != 0 && _eventChoiceNumOld[2] != 0)
			//	{
			//		randNum = 0;
			//	}
			//
			//	switch (randNum)	// 0~5
			//	{
			//	case 0:
			//		eventState = EVENT_STATE::YADO;
			//		break;
			//	case 1:
			//		eventState = EVENT_STATE::YADO;
			//		break;
			//	case 2:
			//		eventState = EVENT_STATE::SYOUNIN;
			//		break;
			//	case 3:
			//		eventState = EVENT_STATE::BUTTON;
			//		break;
			//	case 4:
			//		eventState = EVENT_STATE::CHEST;
			//		break;
			//	case 5:
			//		eventState = EVENT_STATE::DRINK;
			//		break;
			//	default:
			//		eventState = EVENT_STATE::ENEMY;
			//		break;
			//	}
			//
			//	// 数字情報が消える前に保存しておく
			//	_eventChoiceNumOld[_eventChoiceNum] = randNum;
			//	if (_eventChoiceNum <= 1)
			//	{
			//		_eventChoiceNum++;
			//	}
			//	else
			//	{
			//		_eventChoiceNum = 0;
			//	}
			//}
			//
			//// 敵の出現
			//if (eventState == EVENT_STATE::ENEMY)
			//{
			//	if (_monster[0]->GetEnemyState() == ENEMY_STATE::NON)
			//	{
			//		//doorFlg = false;
			//		moveFlg = false;
			//
			//		// ボスならこっち
			//		if (walkCnt == _bossEventNum)
			//		{
			//			auto ene = 5;
			//			_monster[0]->SetEnemyNum(ene, _player->GetNowLevel());		// これで敵の情報をセットしている(ボス用)
			//			_cards->SetTurn(_monster[0]->GetMaxTurn());
			//		}
			//		else
			//		{
			//			// 敵は0~4まで
			//			auto ene = GetRand(4);
			//			_monster[0]->SetEnemyNum(ene, _player->GetNowLevel());		// これで敵の情報をセットしている(ランダムにする)
			//			_cards->SetTurn(_monster[0]->GetMaxTurn());
			//		}
			//	}
			//}
			//
			//// 宿屋・商人・ボタン・宝箱の出現
			//if (eventState != EVENT_STATE::NON && eventState != EVENT_STATE::ENEMY)
			//{
			//	//doorFlg = false;
			//	moveFlg = false;
			//}
		//}
		//}
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
		if (walkCnt == _eventNum[_event->GetNowEvent()] && _monster[0]->GetEnemyState() == ENEMY_STATE::DEATH)
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
			//eventState = EVENT_STATE::NON;
			return;
		}
		eve++;
	}

	/*for文で回す前*/
	//if (eventState == EVENT_STATE::YADO)
	//{
	//	_event->SetEvent(eventState);
	//	//Yado();
	//}
	//
	//if (eventState == EVENT_STATE::SYOUNIN)
	//{
	//	_event->SetEvent(eventState);
	//	//Syounin();
	//}
	//
	//if (eventState == EVENT_STATE::BUTTON)
	//{
	//	_event->SetEvent(eventState);
	//	//Button();
	//}
	//
	//if (eventState == EVENT_STATE::CHEST)
	//{
	//	_event->SetEvent(eventState);
	//	//Chest();
	//}
	//
	//if (eventState == EVENT_STATE::DRINK)
	//{
	//	_event->SetEvent(eventState);
	//
	//	//if (mouse & MOUSE_INPUT_LEFT) {			 //マウスの左ボタンが押されていたら
	//	//	if (cursorPos.x >= 600 && cursorPos.x <= 600 + 100 && cursorPos.y >= 345 && cursorPos.y <= 345 + 50)
	//	//	{
	//	//		// 先に進む
	//	//		moveFlg = true;
	//	//		eventState = EVENT_STATE::NON;
	//	//		//_nowEvent++;
	//	//		_pushFlg = false;
	//	//		_fateNum = -1;
	//	//	}
	//	//
	//	//	// 飲む
	//	//	if (_fateNum == -1)
	//	//	{
	//	//		if (cursorPos.x >= 600 && cursorPos.x <= 600 + 100 && cursorPos.y >= 200 && cursorPos.y <= 200 + 50)
	//	//		{
	//	//			_pushFlg = true;
	//	//			_fateNum = GetRand(1);	// 0 or 1
	//	//		}
	//	//	}
	//	//}
	//}
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
		if (!shakeFlg)
		{
			// 攻撃を受けた後はターンを復活させる(敵によって増減)
			_cards->SetTurn(_monster[0]->GetMaxTurn());
			_cards->SetGuard(0);
			_onceFlg = false;
		}
	}
}

void GameScene::pl_Attack(void)
{
	// 敵にダメージを与えたら、ダメージ量を0に戻す
	// ダメージ計算(カード威力+基本攻撃力+装備による攻撃力プラス値+パワーアップ効果(ないときは0))
	_monster[0]->Damage(_cards->GetDamage() + _player->GetAttackDamage() + _menu->GetEquipDamage() + _menu->GetPowUp());
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

	//DrawRotaGraph(450 + _shackPos.x, 300 + _shackPos.y, 1.0f, 0, _room[0], false);
	//DrawRotaGraph(450 + _shackPos.x, 300 + _shackPos.y, 1.0f, 0, straightPNG, false);
	//DrawRotaGraph(450 + _shackPos.x, 300 + _shackPos.y, 1.0f, 0, rightPNG, false);
	//DrawRotaGraph(450 + _shackPos.x, 300 + _shackPos.y, 1.0f, 0, roadPNG[1], false);
	DrawRotaGraph(450 + _shackPos.x, 300 + _shackPos.y, 1.0f, 0, roadPNG[plNowPoint], false);

	if (_shakeTime >= 15.0f)
	{
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

		//if (!_onceFlg)
		//{
		//	//_menu->Setitem(item_pair[4].second,item_pair[4].first);
		//	if (_menu->GetCanHaveItem() != 0)
		//	{
		//		_menu->Setitem(_item->GetPair(4).second, _item->GetPair(4).first);
		//		_onceFlg = true;
		//	}
		//	else
		//	{
		//		_anounceFlg = true;
		//	}
		//}
		//if (moveFlg)
		//{
		//	_monster[0]->SetDropFlg(false);
		//	_onceFlg = false;
		//}
	}

	if (moveFlg)
	{
		_monster[0]->SetDropFlg(false);
		_onceFlg = false;
		_anounceFlg = false;
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
			//// 毒音
			//PlaySoundMem(_soundSE[7], DX_PLAYTYPE_BACK, true);
			//
			//// 体力の1/5ぐらい削ろうかな
			//_player->SetHP(_player->GetHP() - (float)_player->GetMaxHP() * (1.0f / 5.0f));
			//// 1ターンマイナス
			//_player->SetConditionTurn(_player->GetConditionTurn() - 1);
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
			//// 毒音
			//PlaySoundMem(_soundSE[7], DX_PLAYTYPE_BACK, true);
			//
			//// 体力の1/5ぐらい削ろうかな
			//_player->SetHP(_player->GetHP() - (float)_player->GetMaxHP() * (1.0f / 5.0f));
			//// 1ターンマイナス
			//_player->SetConditionTurn(_player->GetConditionTurn() - 1);
		}
		pl_Heal();
		_cards->SetCardsSyurui(CARDS_SYURUI::NON);
	}

	if (_cards->GetCardsSyurui() == CARDS_SYURUI::GUARD)
	{
		// 防御カード音
		PlaySoundMem(_soundSE[4], DX_PLAYTYPE_BACK, true);

		//// スキルチャージが0より大きいときは減らしていく
		if (_player->GetSkillCharge() > 0)
		{
			_player->SetSkillCharge(_player->GetSkillCharge() - 1);
		}

		if (_player->GetCondition() == CONDITION::POISON)
		{
			lambdaPoison();
			//// 毒音
			//PlaySoundMem(_soundSE[7], DX_PLAYTYPE_BACK, true);
			//
			//// 体力の1/5ぐらい削ろうかな
			//_player->SetHP(_player->GetHP() - (float)_player->GetMaxHP() * (1.0f / 5.0f));
			//// 1ターンマイナス
			//_player->SetConditionTurn(_player->GetConditionTurn() - 1);
		}
		_cards->SetCardsSyurui(CARDS_SYURUI::NON);
	}
}

void GameScene::TestDirect(void)
{
	// 行き止まりだったら進行方向にたいしてバック処理
	if (plNowPoint == 3 || _backFlg)
	{
		if (_plDirectOld == PL_DIRECTION::DOWN && (plNowPoint == 4 || plNowPoint == 7 || plNowPoint == 8 || plNowPoint == 0))
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
		rightFlg = false;
		leftFlg = false;
		if (_plDirect == PL_DIRECTION::UP)
		{
			testy--;
		}
		else if (_plDirect == PL_DIRECTION::DOWN)
		{
			testy++;
		}
		else if (_plDirect == PL_DIRECTION::RIGHT)
		{
			testx--;
		}
		else if (_plDirect == PL_DIRECTION::LEFT)
		{
			testx++;
		}

		plNowPoint = numkai[testy][testx].second;

		// 方向だけ転換させる--------

		// T字路テスト
		if (plNowPoint == 4)
		{
			_backFlg = false;
			_plDirect = _plDirectOld;
			//plNowPoint = plOldPoint;
			if (plOldPoint == 5)
			{
				_plDirect = PL_DIRECTION::RIGHT;
				directRota = PI / 2;
			}
			else if (_plDirect == PL_DIRECTION::RIGHT)
			{
				directRota = PI / 2;
			}
			else if (_plDirect == PL_DIRECTION::LEFT)
			{
				directRota = PI + PI / 2;
			}
			else if (_plDirect == PL_DIRECTION::DOWN)
			{
				directRota = PI;
			}
			else if (_plDirect == PL_DIRECTION::UP)
			{
				directRota = 0;
			}
			return;
		}

		// トの字型(直進と右への道)
		if (plNowPoint == 5)
		{
			_backFlg = false;
			//_plDirect = _plDirectOld;
			if (_plDirectOld == PL_DIRECTION::UP)
			{
				directRota = 0;
				_plDirect = _plDirectOld;
			}
			else if (_plDirectOld == PL_DIRECTION::RIGHT)
			{
				directRota = 0;
				_plDirect = PL_DIRECTION::UP;
			}
			return;
		}
		// トの字型(直進と左への道)
		if (plNowPoint == 6)
		{
			_backFlg = false;
			_plDirect = _plDirectOld;
			if (_plDirect == PL_DIRECTION::UP)
			{
				directRota = 0;
			}
			return;
		}

		if (_plDirect == PL_DIRECTION::UP)
		{
			if ((plNowPoint == 0 || plNowPoint == 5) && !rightFlg)	// 直進
			{
				_backFlg = false;
				return;
			}
			else if (plNowPoint == 1)
			{
				_plDirectOld = _plDirect;
				_backFlg = false;
				_plDirect = PL_DIRECTION::LEFT;	// 左曲がり
				leftFlg = true;
				directRota = PI + PI / 2;
				return;
			}
			else if (plNowPoint == 2)
			{
				_plDirectOld = _plDirect;
				_backFlg = false;
				_plDirect = PL_DIRECTION::RIGHT;	// 右曲がり
				rightFlg = true;
				directRota = PI / 2;
				return;
			}
		}

		if (_plDirect == PL_DIRECTION::DOWN)
		{
			if (plNowPoint == 0)	// 直進
			{
				_backFlg = false;
				return;
			}
			else if (plNowPoint == 1)
			{
				//_plDirectOld = _plDirect;
				_backFlg = false;
				_plDirect = PL_DIRECTION::RIGHT;	// 右曲がり
				rightFlg = true;
				directRota = PI / 2;
				return;
			}
			else if (plNowPoint == 2)
			{
				//_plDirectOld = _plDirect;
				_backFlg = false;
				_plDirect = PL_DIRECTION::LEFT;	// 左曲がり
				leftFlg = true;
				directRota = PI + PI / 2;
				return;
			}
		}

		if (_plDirect == PL_DIRECTION::RIGHT)
		{
			if (plNowPoint == 0)	// 直進
			{
				_backFlg = false;
				return;
			}
			else if (plNowPoint == 1)	
			{
				_plDirectOld = _plDirect;
				_backFlg = false;
				_plDirect = PL_DIRECTION::UP;
				directRota = 0.0f;
				return;
			}
			else if (plNowPoint == 2)	
			{
				_plDirectOld = _plDirect;
				_backFlg = false;
				_plDirect = PL_DIRECTION::DOWN;
				directRota = PI;
				return;
			}
		}

		if (_plDirect == PL_DIRECTION::LEFT)
		{
			if (plNowPoint == 0)	// 直進
			{
				_backFlg = false;
				return;
			}
			else if (plNowPoint == 1)	
			{
				_plDirectOld = _plDirect;
				_backFlg = false;
				_plDirect = PL_DIRECTION::DOWN;
				leftFlg = false;
				directRota = PI;
				return;
			}
			else if (plNowPoint == 2)	
			{
				_plDirectOld = _plDirect;
				_backFlg = false;
				_plDirect = PL_DIRECTION::UP;
				leftFlg = false;
				directRota = 0.0f;
				return;
			}
		}
	}
	//------------------------------------------

	// T字路テスト
	if (plNowPoint == 4)
	{
		//rightFlg = false;
		//leftFlg = false;
		if (rightFlg && _plDirect != PL_DIRECTION::UP && _plDirect != PL_DIRECTION::DOWN)
		{
			_plDirectOld = _plDirect;
			plOldPoint = plNowPoint;
			_plDirect = PL_DIRECTION::DOWN;
			testy--;
			directRota = PI;
			return;
		}

		if (leftFlg && _plDirect != PL_DIRECTION::UP && _plDirect != PL_DIRECTION::DOWN)
		{
			_plDirectOld = _plDirect;
			plOldPoint = plNowPoint;
			_plDirect = PL_DIRECTION::UP;
			testy++;
			directRota = 0;
			return;
		}

		if (rightFlg && _plDirect == PL_DIRECTION::UP)
		{
			_plDirectOld = _plDirect;
			plOldPoint = plNowPoint;
			_plDirect = PL_DIRECTION::RIGHT;
			testx++;
			directRota = PI / 2;
			return;
		}

		if (leftFlg && _plDirect == PL_DIRECTION::UP)
		{
			_plDirectOld = _plDirect;
			plOldPoint = plNowPoint;
			_plDirect = PL_DIRECTION::LEFT;
			testx--;
			directRota = PI + PI / 2;
			return;
		}

		if (rightFlg && _plDirect == PL_DIRECTION::DOWN)
		{
			_plDirectOld = _plDirect;
			plOldPoint = plNowPoint;
			_plDirect = PL_DIRECTION::LEFT;
			testx--;
			directRota = PI + PI / 2;
			return;
		}

		if (leftFlg && _plDirect == PL_DIRECTION::DOWN)
		{
			_plDirectOld = _plDirect;
			plOldPoint = plNowPoint;
			_plDirect = PL_DIRECTION::RIGHT;
			testx++;
			directRota = PI / 2;
			return;
		}
	}

	if (_plDirect == PL_DIRECTION::UP)
	{
		if ((plNowPoint == 0 || plNowPoint == 5 || plNowPoint == 6) && !rightFlg && !leftFlg)	// 直進
		{
			plOldPoint = plNowPoint;
			_plDirectOld = _plDirect;
			testy++;
			return;
		}
		else if (plNowPoint == 1 || plNowPoint == 5)
		{
			_plDirectOld = _plDirect;
			plOldPoint = plNowPoint;
			_plDirect = PL_DIRECTION::RIGHT;	// 右曲がり
			rightFlg = true;
			testx++;
			directRota =  PI / 2;
			return;
		}
		else if (plNowPoint == 2 || plNowPoint == 6)
		{
			_plDirectOld = _plDirect;
			plOldPoint = plNowPoint;
			_plDirect = PL_DIRECTION::LEFT;	// 左曲がり
			leftFlg = true;
			testx--;
			directRota = PI + PI / 2;
			return;
		}
	}

	if (_plDirect == PL_DIRECTION::DOWN)
	{
		if (plNowPoint == 0)	// 直進
		{
			plOldPoint = plNowPoint;
			testy--;
			return;
		}
		else if (plNowPoint == 1)
		{
			_plDirect = PL_DIRECTION::LEFT;	// 左曲がり
			leftFlg = true;
			_plDirectOld = _plDirect;
			plOldPoint = plNowPoint;
			testx--;
			directRota = PI + PI / 2;
			return;
		}
		else if (plNowPoint == 2)
		{
			_plDirectOld = _plDirect;
			plOldPoint = plNowPoint;
			_plDirect = PL_DIRECTION::RIGHT;	// 右曲がり
			rightFlg = true;
			testx++;
			directRota = PI / 2;
			return;
		}
	}

	if (_plDirect == PL_DIRECTION::RIGHT)
	{
		if (plNowPoint == 0)	// 直進
		{
			plOldPoint = plNowPoint;
			testx++;
			return;
		}
		else if (plNowPoint == 1 && rightFlg)	// 右曲がり(下)
		{
			_plDirectOld = _plDirect;
			plOldPoint = plNowPoint;
			_plDirect = PL_DIRECTION::DOWN;
			rightFlg = false;
			testy--;
			directRota = PI;
			return;
		}
		else if (plNowPoint == 2 && rightFlg)	// 左曲がり(上)
		{
			_plDirectOld = _plDirect;
			plOldPoint = plNowPoint;
			_plDirect = PL_DIRECTION::UP;
			rightFlg = false;
			testy++;
			directRota = 0.0f;
			return;
		}
	}

	if (_plDirect == PL_DIRECTION::LEFT)
	{
		if (plNowPoint == 0)	// 直進
		{
			plOldPoint = plNowPoint;
			testx--;
			return;
		}
		else if (_plDirectOld == PL_DIRECTION::DOWN)
		{
			_plDirectOld = _plDirect;
			plOldPoint = plNowPoint;
			_plDirect = PL_DIRECTION::DOWN;
			leftFlg = false;
			testy--;
			directRota = PI;
			return;
		}
		else if (plNowPoint == 1 && leftFlg)	// 右曲がり(上)
		{
			_plDirectOld = _plDirect;
			plOldPoint = plNowPoint;
			_plDirect = PL_DIRECTION::UP;
			leftFlg = false;
			testy++;
			directRota = 0.0f;
			return;
		}
		else if (plNowPoint == 2 && leftFlg)	// 左曲がり(下)
		{
			_plDirectOld = _plDirect;
			plOldPoint = plNowPoint;
			_plDirect = PL_DIRECTION::DOWN;
			leftFlg = false;
			testy--;
			directRota = PI;
			return;
		}
	}
}

void GameScene::TestKey(void)
{
	testCnt = 0;

	// 敵が出ないときは扉を出す
	//if (walkCnt != _eventNum[_event->GetNowEvent()] && !moveFlg)
	//{
		// クリック音
		//PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);

		// 歩行音
		//PlaySoundMem(_soundSE[1], DX_PLAYTYPE_BACK, true);
		_soundWalk = true;

		moveFlg = true;
		walkCnt++;
		// プレイヤーが状態異常のとき
		if (_player->GetCondition() == CONDITION::POISON)
		{
			// ダメージ音
			//PlaySoundMem(_soundSE[6], DX_PLAYTYPE_BACK, true);

			// 体力の1/5ぐらい削ろうかな
			_player->SetHP(_player->GetHP() - (float)_player->GetMaxHP() * (1.0f / 5.0f));
			// 1ターンマイナス
			_player->SetConditionTurn(_player->GetConditionTurn() - 1);

			shakeFlg = true;
		}
	//}

	// 扉が開いていたら再設定
	if (_openDoor)
	{
		//if (plNowPoint == 0)
		//{
		//	testy++;
		//}
		//else if (plNowPoint == 1)
		//{
		//	_plDirect = PL_DIRECTION::RIGHT;
		//	rightFlg = true;
		//	testx++;
		//}			

		TestDirect();
		plNowPoint = numkai[testy][testx].second;
		//if (!numkai[testy][testx].first)
		//{
		//	movePos[aaab] = VECTOR2(testx * 50, 550 - (testy * 50));
		//	aaab++;
		//}
		//numkai[testy][testx].first = true;

		if (!numkai[testy][testx].first)
		{
			vec.emplace_back(VECTOR2(testx * 50, 550 - (testy * 50)),plNowPoint, directRota);
			numkai[testy][testx].first = true;
		}

		auto aaa = numkai[testy][testx].second;
		auto bbb = _plDirect;

		// 通ったことのある道はフラグがtrueになる仕組み
		// フラグがfalse(通ったことがない)なら追加する
		//if (!numkai[testy][testx].first)
		//{
			//mapTest.insert(std::make_pair(testx, testy));
			//aaab++;
			//numkai[testy][testx].first = true;
		//}

		// クリック音
		PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);

		_monster[0]->SetEnemyState(ENEMY_STATE::NON);
		//doorFlg = true;
		moveFlg = false;
		_openDoor = false;
		_degree = 0.0f;
		_doorExpand = 1.0f;
	}

	// イベントマスでランダムに数値を出し、対応した数値でイベントを発生させる
	// ランダムで数字を出して、出た数字で出現させるものを決める
	// 敵との遭遇回数をふやしたかったら数字の0~3までをenemyとかにすればいいかもしれない
	//if (walkCnt == _eventNum[_event->GetNowEvent()] && eventState == EVENT_STATE::NON && !moveFlg)
	//{
		//int randNum = GetRand(1);

	if (testx == bossPos.x && testy == bossPos.y)
	{
		eventState = EVENT_STATE::ENEMY;
	}
	else
	{
		int num = plNowPoint;

		// 特定敵イベント中はカウンターを止める
		if (num < 7 && !_event->GetEventMonsEncountFlg())
		{
			if (monsTimeCnt > 0)
			{
				monsTimeCnt--;
			}
			else
			{
				eventState = EVENT_STATE::ENEMY;
				monsTimeCnt = 3;
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
					plNowPoint = 0;
				}
				break;
			case 14:
				eventState = EVENT_STATE::GOAL;
				//_openDoor = true;
				break;
			default:
				eventState = EVENT_STATE::NON;
				break;
			}
		}
	}



	//	if (walkCnt == _bossEventNum)
	//	{
	//		randNum = 0;
	//	}

	//	// 前回のイベント内容と被っているならとりま敵にする
	//	if (randNum == _eventChoiceNumOld[0])
	//	{
	//		randNum = 0;
	//	}

	//	// 敵以外のイベント同士で被っているときは敵にする
	//	if (_eventChoiceNumOld[1] != 0 && _eventChoiceNumOld[2] != 0 && _eventChoiceNumOld[1] == _eventChoiceNumOld[2])
	//	{
	//		randNum = 0;
	//	}

	//	// 直近3回のイベントに敵が出現していないときは、敵のイベントをする
	//	if (_eventChoiceNumOld[0] != 0 && _eventChoiceNumOld[1] != 0 && _eventChoiceNumOld[2] != 0)
	//	{
	//		randNum = 0;
	//	}

		//switch (randNum)	// 0~5
		//{
		//case 0:
		//	eventState = EVENT_STATE::ENEMY;
		//	break;
		//case 1:
		//	eventState = EVENT_STATE::CHEST;
		//	chestFate = GetRand(2);	// 0 ~ 2
		//	break;
		//case 2:
		//	eventState = EVENT_STATE::SYOUNIN;
		//	break;
		//case 3:
		//	eventState = EVENT_STATE::BUTTON;
		//	break;
		//case 4:
		//	eventState = EVENT_STATE::CHEST;
		//	break;
		//case 5:
		//	eventState = EVENT_STATE::DRINK;
		//	break;
		//default:
		//	eventState = EVENT_STATE::ENEMY;
		//	break;
		//}

	//	// 数字情報が消える前に保存しておく
	//	_eventChoiceNumOld[_eventChoiceNum] = randNum;
	//	if (_eventChoiceNum <= 1)
	//	{
	//		_eventChoiceNum++;
	//	}
	//	else
	//	{
	//		_eventChoiceNum = 0;
	//	}
	//}

	// 敵の出現(特定敵イベントでもここに入るようにする)
	if (eventState == EVENT_STATE::ENEMY)
	{
		monsterFlg = true;
		if (_monster[0]->GetEnemyState() == ENEMY_STATE::NON)
		{
			//doorFlg = false;
			moveFlg = false;

			// ボスならこっち
			if (/*walkCnt == _bossEventNum*/testx == bossPos.x && testy == bossPos.y)
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
