#include <Dxlib.h>
#include "Event.h"
#include "../Scene/GameScene.h"
#include "../Player.h"
#include "../Card/Cards.h"
#include "../Scene/SelectScene.h"
#include "../Common/MouseCtl.h"

#define PI 3.141592653589793f

Event::Event()
{
	Init();
}

Event::~Event()
{
	// 音関係
	for (int i = 0; i < 4; i++)
	{
		DeleteSoundMem(_soundSE[i]);
	}
}

void Event::Init(void)
{
	_event = EVENT_STATE::NON;
	// 宿屋・商人
	_healinnFlg = false;
	_nonMoneyFlg = false;
	_buyFlg = false;
	_chooseFlg = false;
	_chooseNum = -1;
	_itemNextPage = false;

	// ボタン・宝箱・飲み物
	_pushFlg = false;
	_fateNum = -1;

	// その他
	_getFlg = false;
	_anounceFlg = false;
	_onceFlg = false;
	_forcedButtleFlg = false;

	_chestItemMap.try_emplace(0, ITEM::POTION_BIG);
	_chestItemMap.try_emplace(1, ITEM::DETOX);
	_chestItemMap.try_emplace(2, ITEM::POW_UP);
	_chestItemMap.try_emplace(3, ITEM::HEART);

	// 宝箱設定
	int chestHandle;
	// ボタンと飲み物の座標設定
	int posHandle;

	if (SelectScene::modeSelect == MODE::NORMAL)
	{
		chestHandle = FileRead_open("csv/chest1.csv");
		posHandle   = FileRead_open("csv/buttonDrink1.csv");
		if (chestHandle == NULL || posHandle == NULL)
		{
			return; // エラー
		}
	}
	else if (SelectScene::modeSelect == MODE::HARD)
	{
		chestHandle = FileRead_open("csv/chest2.csv");
		posHandle   = FileRead_open("csv/buttonDrink2.csv");
		if (chestHandle == NULL || posHandle == NULL)
		{
			return; // エラー
		}
	}
	else
	{
		return;		// エラー
	}

	for (int i = 0; i < 4; i++)
	{
		FileRead_scanf(chestHandle, "%d,%d,%d,%d", &_chestOpen[i], &_chestBingo[i], &_chestPos[i].x, &_chestPos[i].y);
	}

	//ファイルを閉じる
	FileRead_close(chestHandle);

	for (int i = 0; i < 4; i++)
	{
		FileRead_scanf(posHandle, "%d,%d", &_buttonDrink[i].x, &_buttonDrink[i].y);
	}

	//ファイルを閉じる
	FileRead_close(posHandle);

	// ボタン
	_buttonNum = -1;
	_buttonPush[0] = false;
	_buttonPush[1] = false;
	_buttonEventFlg = false;

	// 飲み物
	_drinkNum = -1;
	_drinking[0] = false;
	_drinking[1] = false;
	_drinkEventFlg = false;

	// 即死トラップ
	_trapFlg = false;
	_nowTrapFlg = false;

	// 特定敵
	_eventMonsFlg = false;
	_eventMonsEncountFlg = false;
	_eventMonsEndFlg = false;

	pngInit();

	// SE
	_soundSE[0] = LoadSoundMem("sound/se/click.mp3");
	_soundSE[1] = LoadSoundMem("sound/se/blitz.mp3");
	_soundSE[2] = LoadSoundMem("sound/se/damage.mp3");
	_soundSE[3] = LoadSoundMem("sound/se/poison.mp3");

	innSt	    = std::make_unique<INNSt>();
	merchantSt  = std::make_unique<MerchantSt>();
	buttonSt    = std::make_unique<ButtonSt>();
	chestSt     = std::make_unique<ChestSt>();
	drinkSt     = std::make_unique<DrinkSt>();
	deathTrapSt = std::make_unique<DeathTrapSt>();
	eventMonsSt = std::make_unique<EventMonsSt>();

	exr = 0.0f;
}

void Event::pngInit(void)
{
	_eventImages.try_emplace("inn", LoadGraph("image/event/inn.png"));						// 宿屋
	_eventImages.try_emplace("merchant", LoadGraph("image/event/merchant.png"));			// 商人
	_eventImages.try_emplace("bottle", LoadGraph("image/event/bottle.png"));				// 飲み物
	_eventImages.try_emplace("chestInItem", LoadGraph("image/event/tresure_1.png"));		// 宝箱(アイテムが入っている)
	_eventImages.try_emplace("chestEmpty", LoadGraph("image/event/chest_empty.png"));		// 空宝箱
	_eventImages.try_emplace("deathTrap", LoadGraph("image/event/instant_death_trap.png"));	// 即死トラップ
	_eventImages.try_emplace("mons", LoadGraph("image/monster/event_monster.png"));			// 特定敵

	_drawHandle.try_emplace("message", LoadGraph("image/message.png"));
	_drawHandle.try_emplace("frame", LoadGraph("image/frame.png"));
	_drawHandle.try_emplace("itembox", LoadGraph("image/itembox.png"));
	_drawHandle.try_emplace("itemChoice", LoadGraph("image/itemChoice.png"));
	_drawHandle.try_emplace("arrow", LoadGraph("image/arrow.png"));

	// 宝箱
	std::string trasure_0 = "image/event/trasure_0.png";
	LoadDivGraph(trasure_0.c_str(), 2, 2, 1, 390 / 2, 431, _chestPNG);
	// 文字画像の分割読み込み
	std::string sentakusi = "image/choice/choice.png";
	LoadDivGraph(sentakusi.c_str(),12, 12, 1, 150, 75, _choicesPNG);
}

void Event::UpDate(GameScene* game, const std::shared_ptr<Player>& player, const std::shared_ptr<Menu>& menu, const std::shared_ptr<Item>& item, const std::shared_ptr<Monster>& monster, const std::shared_ptr<Cards>& cards, const std::shared_ptr<MouseCtl>& mouse)
{
	// 強制戦闘の設定
	auto forcedButtleLambda = [&]() {
		if (_eventMonsFlg && !_onceFlg)
		{
			monster->SetEnemyNum(6, 0);
			cards->SetTurn(3);
			_onceFlg = true;
			_forcedButtleFlg = true;
		}
	};

	// _eventと一致しているイベントを発生させる
	switch (_event)
	{
	case EVENT_STATE::ENEMY:
		Enemy(game, player, monster);
		break;
	case EVENT_STATE::INN:
		forcedButtleLambda();
		innSt->Update(*this, *game, *player, *mouse);
		break;
	case EVENT_STATE::MERCHANT:
		forcedButtleLambda();
		merchantSt->Update(*this, *game, *player, *mouse, *item, *menu);
		break;
	case EVENT_STATE::BUTTON:
		forcedButtleLambda();
		buttonSt->Update(*this, *game, *player, *mouse);
		break;
	case EVENT_STATE::CHEST:
		forcedButtleLambda();
		chestSt->Update(*this, *game, *player, *menu, *item, *mouse);
		break;
	case EVENT_STATE::DRINK:
		forcedButtleLambda();
		drinkSt->Update(*this, *game, *player, *mouse);
		break;
	case EVENT_STATE::TRAP:
		deathTrapSt->Update(*this, *game, *player, *mouse);
		break;
	case EVENT_STATE::EVE_MONS:
		eventMonsSt->Update(*this, *game, *monster, *cards, *mouse);
		break;
	default:
		break;
	}
}

void Event::Draw(GameScene* game, const std::shared_ptr<Player>& player, const std::shared_ptr<Menu>& menu, const std::shared_ptr<Item>& item)
{
	// _eventと一致しているイベントを描画する
	switch (_event)
	{
	case EVENT_STATE::INN:
		if (!_eventMonsFlg)
		{
			innSt->Draw(*this, *player);
		}
		break;
	case EVENT_STATE::MERCHANT:
		if (!_eventMonsFlg)
		{
			merchantSt->Draw(*this, *player, *item, *menu);
		}
		break;
	case EVENT_STATE::BUTTON:
		if (!_eventMonsFlg)
		{
			buttonSt->Draw(*this, *game);
		}
		break;
	case EVENT_STATE::CHEST:
		if (!_eventMonsFlg)
		{
			chestSt->Draw(*this, *menu);
		}
		break;
	case EVENT_STATE::DRINK:
		if (!_eventMonsFlg)
		{
			drinkSt->Draw(*this, *game);
		}
		break;
	case EVENT_STATE::TRAP:
		deathTrapSt->Draw(*this);
		break;
	default:
		break;
	}

	// イベント敵と遭遇中
	if (_event == EVENT_STATE::EVE_MONS || _event != EVENT_STATE::NON && _eventMonsFlg)
	{
		eventMonsSt->Draw(*this);
	}

	// イベントがNONでもeventmonFlgが立っていたら描画(画面に張り付き)
	if (_event == EVENT_STATE::NON && _eventMonsEncountFlg)
	{
		DrawGraph(100, 75, _eventImages["mons"], true);
		// メッセージボックス
		DrawGraph(420, 50, _drawHandle["message"], true);
		DrawFormatString(450, 70, 0x000000, "なんと敵が追いかけてきた!");
	}
}

void Event::SetEvent(const EVENT_STATE& state)
{
	_event = state;
}

void Event::SetFateNum(const int& num)
{
	_fateNum = num;
}

void Event::SetReset(void)
{
	_onceFlg = false;

	// ボタンの状態をリセットする
	_buttonPush[0] = false;
	_buttonPush[1] = false;

	// 飲み物の状態をリセットする
	_drinking[0] = false;
	_drinking[1] = false;

	// 宝箱の状態をリセットする
	for (int i = 0; i < 4; i++)
	{
		_chestOpen[i] = 0;
	}

	// 即死トラップ発動中で死亡していたら新たな展開へ
	if (_trapFlg)
	{
		_nowTrapFlg = true;
	}
	_trapFlg = false;
}

bool Event::GetEventMonsFlg(void)const
{
	return _eventMonsFlg;
}

void Event::SetEventMonsFlg(const bool& flag)
{
	_eventMonsFlg = flag;
}

bool Event::GetEventMonsEndFlg(void)const
{
	return _eventMonsEndFlg;
}

void Event::SetEventMonsEncountFlg(const bool& flag)
{
	_eventMonsEncountFlg = flag;
}

bool Event::GetEventMonsEncountFlg(void)const
{
	return _eventMonsEncountFlg;
}

void Event::SetCautionFlg(const bool& flag)
{
	_forcedButtleFlg = flag;
}

bool Event::GetCautionFlg(void)const
{
	return _forcedButtleFlg;
}

void Event::Enemy(GameScene* game, const std::shared_ptr<Player>& player, const std::shared_ptr<Monster>& monster)
{
	game->eventState = EVENT_STATE::NON;
	_event = EVENT_STATE::NON;

	player->SetNextLevel(player->GetNextLevel() - monster->GetEXP());

	// 所持金を増やす
	player->SetMoney(player->GetMoney() + monster->GetMoney());
}