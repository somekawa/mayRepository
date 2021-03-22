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
	_chestItemMap.try_emplace(2, ITEM::KYOUKA_POW);
	_chestItemMap.try_emplace(3, ITEM::HEART);

	// 宝箱設定
	int chestHandle;
	// ボタンと飲み物の座標設定
	int posHandle;

	if (SelectScene::modeSelect == MODE::NORMAL)
	{
		chestHandle = FileRead_open("csv/chest1.csv");
		posHandle = FileRead_open("csv/buttonDrink1.csv");
		if (chestHandle == NULL || posHandle == NULL)
		{
			return; // エラー
		}
	}
	else if (SelectScene::modeSelect == MODE::HARD)
	{
		chestHandle = FileRead_open("csv/chest2.csv");
		posHandle = FileRead_open("csv/buttonDrink2.csv");
		if (chestHandle == NULL || posHandle == NULL)
		{
			return; // エラー
		}
	}
	else
	{
		// ここにきたらエラー
		return;
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
	_soundSE[1] = LoadSoundMem("sound/se/biribiri.mp3");
	_soundSE[2] = LoadSoundMem("sound/se/damage.mp3");
	_soundSE[3] = LoadSoundMem("sound/se/poison.mp3");

	innSt = std::make_unique<INNSt>();
	merchantSt = std::make_unique<MerchantSt>();
	buttonSt = std::make_unique<ButtonSt>();
	chestSt = std::make_unique<ChestSt>();
	drinkSt = std::make_unique<DrinkSt>();

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
	_drawHandle.try_emplace("yajirusi", LoadGraph("image/yajirusi.png"));

	// 宝箱
	std::string trasure_0 = "image/event/trasure_0.png";
	LoadDivGraph(trasure_0.c_str(), 2, 2, 1, 390 / 2, 431, _chestPNG);
	// 文字画像の分割読み込み
	std::string sentakusi = "image/sentakusi/sentakusi.png";
	LoadDivGraph(sentakusi.c_str(),12, 12, 1, 150, 75, _choicesPNG);
}

void Event::UpDate(GameScene* game, const std::shared_ptr<Player>& player, const std::shared_ptr<Menu>& menu, const std::shared_ptr<Item>& item, const std::shared_ptr<Monster>& monster, const std::shared_ptr<Cards>& cards, const std::shared_ptr<MouseCtl>& mouse)
{
	auto lambda = [&]() {
		monster->SetEnemyNum(6, 0);
		cards->SetTurn(3);
		_onceFlg = true;
		_forcedButtleFlg = true;
	};

	if (_event == EVENT_STATE::INN)
	{
		if (_eventMonsFlg && !_onceFlg)
		{
			lambda();
		}
		else
		{
			innSt->Update(*this, *game, *player, *mouse);
		}
	}

	if (_event == EVENT_STATE::MERCHANT)
	{
		if (_eventMonsFlg && !_onceFlg)
		{
			lambda();
		}
		else
		{
			merchantSt->Update(*this, *game, *player, *mouse, *item, *menu);
		}
	}

	if (_event == EVENT_STATE::BUTTON)
	{
		if (_eventMonsFlg && !_onceFlg)
		{
			lambda();
		}
		else
		{
			buttonSt->Update(*this, *game, *player, *mouse);
		}
	}

	if (_event == EVENT_STATE::CHEST)
	{
		if (_eventMonsFlg && !_onceFlg)
		{
			lambda();
		}
		else
		{
			chestSt->Update(*this, *game, *player, *menu, *item, *mouse);
		}
	}

	if (_event == EVENT_STATE::DRINK)
	{
		if (_eventMonsFlg && !_onceFlg)
		{
			lambda();
		}
		else
		{
			drinkSt->Update(*this, *game, *player, *mouse);
		}
	}

	if (_event == EVENT_STATE::TRAP)
	{
		Trap(game,player,mouse);
	}

	if (_event == EVENT_STATE::ENEMY)
	{
		Enemy(game, player, monster);
	}

	if (_event == EVENT_STATE::EVE_MONS)
	{
		eventMons(game, monster,cards,mouse);
	}
}

void Event::Draw(GameScene* game, const std::shared_ptr<Player>& player, const std::shared_ptr<Menu>& menu, const std::shared_ptr<Item>& item)
{
	// 宿屋
	if (_event == EVENT_STATE::INN && !_eventMonsFlg)
	{
		innSt->Draw(*this, *player);
	}

	// 商人
	if (_event == EVENT_STATE::MERCHANT && !_eventMonsFlg)
	{
		merchantSt->Draw(*this, *player, *item, *menu);
	}

	// ボタン出現中
	if (_event == EVENT_STATE::BUTTON && !_eventMonsFlg)
	{
		buttonSt->Draw(*this, *game);
	}

	// 宝箱出現中
	if (_event == EVENT_STATE::CHEST && !_eventMonsFlg)
	{
		chestSt->Draw(*this, *menu);
	}

	// 瓶出現中
	if (_event == EVENT_STATE::DRINK && !_eventMonsFlg)
	{
		drinkSt->Draw(*this, *game);
	}

	// 即死トラップ出現中
	if (_event == EVENT_STATE::TRAP)
	{
		// メッセージボックス
		DrawGraph(420, 50, _drawHandle["message"], true);
		// 去る
		DrawRotaGraph(600 + 150 / 2, 345 + 75 / 2, 0.9f + sinf(PI * 2.0f / 200.0f * exr) * 0.1, 0.0f, _choicesPNG[10], true);
		// 調べる
		DrawRotaGraph(600 + 150 / 2, 200 + 75 / 2, 0.9f + sinf(PI * 2.0f / 200.0f * exr) * 0.1, 0.0f, _choicesPNG[9], true);
		DrawGraph(200, 75,_eventImages["deathTrap"], true);

		if (_nowTrapFlg)
		{
			if (_eventMonsEncountFlg && !_eventMonsEndFlg)
			{
				DrawFormatString(450, 70, 0x000000, "背後に敵が迫っている…!");
			}
			else
			{
				DrawFormatString(450, 70, 0x000000, "即死トラップのようだ");
			}
		}
		else
		{
			DrawFormatString(450, 70, 0x000000, "怪しげな像がある");
			if (_eventMonsEncountFlg && !_eventMonsEndFlg)
			{
				DrawFormatString(450, 90, 0xff0000, "背後に敵が迫っている…!");
			}
		}
	}

	// イベント敵と遭遇中
	if (_event == EVENT_STATE::EVE_MONS || _event != EVENT_STATE::NON && _eventMonsFlg)
	{
		DrawGraph(100, 75, _eventImages["mons"], true);

		// 戦闘に入る前
		if (!_eventMonsFlg)
		{
			// メッセージボックス
			DrawGraph(420, 50, _drawHandle["message"], true);
			// 去る
			DrawRotaGraph(600 + 150 / 2, 345 + 75 / 2, 0.9f + sinf(PI * 2.0f / 200.0f * exr) * 0.1, 0.0f, _choicesPNG[10], true);
			// 調べる
			DrawRotaGraph(600 + 150 / 2, 200 + 75 / 2, 0.9f + sinf(PI * 2.0f / 200.0f * exr) * 0.1, 0.0f, _choicesPNG[11], true);
			DrawFormatString(450, 70, 0x000000, "敵が道をふさいでいる\n何か良い方法はないだろうか..");
		}
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

	// プレイヤーnext_level - 敵経験値
	// next_level <= 0ならnowNumを+1して、ステータス更新
	player->SetNextLevel(player->GetNextLevel() - monster->GetEXP());

	// 所持金を増やす
	player->SetMoney(player->GetMoney() + monster->GetMoney());
}

void Event::Trap(GameScene* game, const std::shared_ptr<Player>& player, const std::shared_ptr<MouseCtl>& mouse)
{
	exr++;
	if (mouse->GetClickTrg())
	{			
		if (mouse->GetPos().x >= 600 && mouse->GetPos().x <= 600 + 150 && mouse->GetPos().y >= 345 && mouse->GetPos().y <= 345 + 75)
		{
			exr = 0.0f;
			// クリック音
			PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
			game->backFlg = true;
			game->eventState = EVENT_STATE::NON;
			_event = EVENT_STATE::NON;
			_fateNum = -1;
		}

		// 調べる
		if (!_trapFlg)
		{
			if (mouse->GetPos().x >= 600 && mouse->GetPos().x <= 600 + 150 && mouse->GetPos().y >= 200 && mouse->GetPos().y <= 200 + 75)
			{
				// クリック音
				PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
				_trapFlg = true;
			}
		}
	}

	// 調べることにしたとき
	if (_trapFlg)
	{
		exr = 0.0f;
		// 即死トラップの発動
		player->SetHP(player->GetHP() - player->GetMaxHP());

		// とりあえずここで特定敵イベントを終了させる
		if (_eventMonsEncountFlg && !_eventMonsEndFlg)
		{
			_eventMonsEncountFlg = false;
			_eventMonsEndFlg = true;
		}
	}
}

void Event::eventMons(GameScene* game, const std::shared_ptr<Monster>& monster, const std::shared_ptr<Cards>& cards, const std::shared_ptr<MouseCtl>& mouse)
{
	exr++;
	_eventMonsEncountFlg = true;
	if (mouse->GetClickTrg()) 
	{			 
		if (mouse->GetPos().x >= 600 && mouse->GetPos().x <= 600 + 150 && mouse->GetPos().y >= 345 && mouse->GetPos().y <= 345 + 75)
		{
			exr = 0.0f;
			// クリック音
			PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
			// 去る
			game->backFlg = true;
			game->eventState = EVENT_STATE::NON;
			_event = EVENT_STATE::NON;
			_fateNum = -1;
		}

		// 戦う
		if (!_eventMonsFlg)
		{
			if (mouse->GetPos().x >= 600 && mouse->GetPos().x <= 600 + 150 && mouse->GetPos().y >= 200 && mouse->GetPos().y <= 200 + 75)
			{
				exr = 0.0f;
				// クリック音
				PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
				_eventMonsFlg = true;
				monster->SetEnemyNum(6, 0);
				cards->SetTurn(3);
			}
		}
	}
}