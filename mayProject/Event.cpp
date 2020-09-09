#include "Dxlib.h"
#include "Event.h"
#include "GameScene.h"
#include "Player.h"
#include "Cards.h"
#include "SelectScene.h"
#include "MouseCtl.h"

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
	_healYadoFlg = false;
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
	_kyouseiButtleFlg = false;

	_chestItemMap.try_emplace(0, ITEM::POTION_BIG);
	_chestItemMap.try_emplace(1, ITEM::DETOX);
	_chestItemMap.try_emplace(2, ITEM::KYOUKA_POW);
	_chestItemMap.try_emplace(3, ITEM::HEART);

	// 宝箱設定
	int chestHandle;
	// ボタンと飲み物の座標設定
	int posHandle;

	if (SelectScene::modeTest == MODE::NORMAL)
	{
		chestHandle = FileRead_open("csv/chest1.csv");
		posHandle = FileRead_open("csv/buttonDrink1.csv");
		if (chestHandle == NULL || posHandle == NULL)
		{
			return; //エラー時の処理
		}
	}
	else if (SelectScene::modeTest == MODE::HARD)
	{
		chestHandle = FileRead_open("csv/chest2.csv");
		posHandle = FileRead_open("csv/buttonDrink2.csv");
		if (chestHandle == NULL || posHandle == NULL)
		{
			return; //エラー時の処理
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

	yadoSt = std::make_unique<YadoSt>();
	syouninSt = std::make_unique<SyouninSt>();
	buttonSt = std::make_unique<ButtonSt>();
	chestSt = std::make_unique<ChestSt>();
	drinkSt = std::unique_ptr<DrinkSt>();
}

void Event::pngInit(void)
{
	eventImages.try_emplace("heal_human", LoadGraph("image/heal_human.png"));		// 宿屋
	eventImages.try_emplace("syounin", LoadGraph("image/syounin.png"));				// 商人
	eventImages.try_emplace("bin", LoadGraph("image/nazo_bin.png"));				// 飲み物
	eventImages.try_emplace("chestInItem", LoadGraph("image/tresure_1.png"));		// 宝箱(アイテムが入っている)
	eventImages.try_emplace("chestKara", LoadGraph("image/chest_kara.png"));		// 空宝箱
	eventImages.try_emplace("zou", LoadGraph("image/daiza.png"));					// 即死トラップ
	eventImages.try_emplace("mons", LoadGraph("image/monster/event_monster.png"));	// 特定敵

	_drawHandle.try_emplace("message", LoadGraph("image/message.png"));
	_drawHandle.try_emplace("frame", LoadGraph("image/frame.png"));
	_drawHandle.try_emplace("itembox", LoadGraph("image/itembox.png"));
	_drawHandle.try_emplace("itemChoice", LoadGraph("image/itemChoice.png"));
	_drawHandle.try_emplace("yajirusi", LoadGraph("image/yajirusi.png"));

	//// メッセージ
	//std::string message = "image/message.png";
	//_messagePNG = LoadGraph(message.c_str());
	//// 商人の持ち物の後ろ枠
	//std::string frame = "image/frame.png";
	//_syouninWakuPNG = LoadGraph(frame.c_str());
	//// アイテムボックス背景
	//std::string itembox = "image/itembox.png";
	//_itemBoxPNG = LoadGraph(itembox.c_str());
	//// 選択中のアイテム
	//std::string itemChoice = "image/itemChoice.png";
	//_itemChoicePNG = LoadGraph(itemChoice.c_str());
	// 宝箱
	std::string trasure_0 = "image/trasure_0.png";
	LoadDivGraph(trasure_0.c_str(), 2, 2, 1, 390 / 2, 431, _chestPNG);
	// 文字画像の分割読み込み
	std::string sentakusi = "image/sentakusi/sentakusi.png";
	LoadDivGraph(sentakusi.c_str(),12, 12, 1, 150, 75, _sentakusiPNG);
	//// 商品ページを移動するための矢印の画像
	//std::string yajirusi = "image/yajirusi.png";
	//_yajirusiPNG = LoadGraph(yajirusi.c_str());
}

void Event::UpDate(GameScene* game, Player* player, Menu* menu, Item* item, Monster* monster,Cards* cards, MouseCtl* mouse)
{
	auto lambda = [&]() {
		monster->SetEnemyNum(6, 0);
		cards->SetTurn(3);
		_onceFlg = true;
		_kyouseiButtleFlg = true;
	};

	if (_event == EVENT_STATE::YADO)
	{
		if (_eventMonsFlg && !_onceFlg)
		{
			lambda();
		}
		else
		{
			yadoSt->Update(*this, *game, *player, *mouse);

			//Yado(game, player,mouse);
		}
	}

	if (_event == EVENT_STATE::SYOUNIN)
	{
		if (_eventMonsFlg && !_onceFlg)
		{
			lambda();
		}
		else
		{
			syouninSt->Update(*this, *game, *player, *mouse, *item, *menu);
			//Syounin(game, player, menu, item,mouse);
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
			//Button(game, player,mouse);
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
			//Chest(game, player, menu, item,mouse);
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
			//Drink(game, player,mouse);
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

void Event::Draw(GameScene* game, Player* player, Menu* menu, Item* item)
{
	// 宿屋
	if (_event == EVENT_STATE::YADO && !_eventMonsFlg)
	{
		//// 人画像
		//DrawGraph(0, 0, eventImages["heal_human"], true);
		//// 去る
		//DrawGraph(600, 345, _sentakusiPNG[10], true);
		//// メッセージボックス
		//DrawGraph(420, 50, _messagePNG, true);
		//if (_nonMoneyFlg)
		//{
		//	DrawFormatString(450, 70, 0x000000, "怪しい老婆:\n%金が足らんようじゃの。\n去るがよい");
		//}
		//if (!_healYadoFlg)
		//{
		//	// 頼む
		//	DrawGraph(600, 200, _sentakusiPNG[2], true);
		//	DrawFormatString(450, 70, 0x000000, "怪しい老婆:\n%d円で回復してやろうか? \n(所持金:%d円)", player->GetNowLevel() * 100,player->GetMoney());
		//}
		//else
		//{
		//	if (!_nonMoneyFlg)
		//	{
		//		DrawFormatString(450, 70, 0x000000, "怪しい老婆:\nﾋｯﾋｯﾋｯﾋｯ...");
		//	}
		//}

		yadoSt->Draw(*this, *player);
	}

	// 商人
	if (_event == EVENT_STATE::SYOUNIN && !_eventMonsFlg)
	{
		//// 人画像
		//DrawGraph(100, 0, eventImages["syounin"], true);
		//// メッセージボックス
		//DrawGraph(420, 50, _messagePNG, true);
		//DrawFormatString(450, 70, 0x000000, "商人:\n何か買うか?");
		//if (_buyFlg)
		//{
		//	DrawGraph(200, 0, _syouninWakuPNG, true);
		//	// 1ページ目
		//	if (!_itemNextPage)
		//	{
		//		// アイテム表示
		//		for (int i = 0; i <= 7; i++)
		//		{
		//			DrawGraph(item->GetPos(i).x, item->GetPos(i).y, _itemBoxPNG, true);
		//			DrawGraph(item->GetPos(i).x, item->GetPos(i).y, item->GetPair(i).first, true);
		//		}
		//	}
		//	else
		//	{
		//		// 2ページ目
		//		// アイテム表示
		//		for (int i = 0; i <= 7; i++)
		//		{
		//			DrawGraph(item->GetPos(i).x, item->GetPos(i).y, _itemBoxPNG, true);
		//			DrawGraph(item->GetPos(i).x, item->GetPos(i).y, item->GetPair(i+8).first, true);
		//		}
		//	}
		//	// 選択中のアイテムの枠
		//	if (_chooseNum >= 0)
		//	{
		//		DrawGraph(item->GetPos(_chooseNum).x, item->GetPos(_chooseNum).y, _itemChoicePNG, true);
		//	}
		//	// 現在の所持金の表示
		//	DrawFormatString(470, 485, 0x000000, "所持金:%d円", player->GetMoney());
		//	// 商品の次のページへの矢印
		//	DrawGraph(490,300, _yajirusiPNG, true);
		//}
		//// 去る
		//DrawGraph(600, 345, _sentakusiPNG[10], true);
		//if (!_buyFlg || _chooseFlg)
		//{
		//	// 購入
		//	DrawGraph(600, 200, _sentakusiPNG[0], true);
		//	if (menu->GetCanHaveItem() == 0)
		//	{
		//		// 持ち物満タンだからもてないよ
		//		DrawFormatString(600, 160, 0xffffff, "所持品がいっぱいだ");
		//	}
		//}
		//if (_buyFlg && _chooseFlg)
		//{
		//	if (_nonMoneyFlg)
		//	{
		//		// 所持金が足りないよ
		//		DrawFormatString(600, 180, 0xffffff, "所持金が足りない");
		//	}
		//}
		//// 文字表示(商品選択中のみ)
		//if (_chooseNum != -1)
		//{
		//	DrawFormatString(300, 450, 0x000000, "%s\n", item->GetSetumei(static_cast<int>(_itemInfo)-1));
		//	DrawFormatString(300, 470, 0x000000, "%d円\n", item->GetCostMoney(static_cast<int>(_itemInfo) - 1));
		//}

		syouninSt->Draw(*this, *player, *item, *menu);
	}

	// ボタン出現中
	if (_event == EVENT_STATE::BUTTON && !_eventMonsFlg)
	{
		//for (int i = 0; i < 4; i++)
		//{
		//	if (_buttonDrink[i].x == game->plPosX && _buttonDrink[i].y == game->plPosY)
		//	{
		//		_buttonNum = i;
		//	}
		//}
		//if (!_buttonPush[_buttonNum])
		//{
		//	// メッセージボックス
		//	DrawGraph(420, 50, _messagePNG, true);
		//	if (_fateNum == -1)
		//	{
		//		// 押す
		//		DrawGraph(600, 200, _sentakusiPNG[4], true);
		//		DrawFormatString(450, 70, 0x000000, "壁にボタンがついている...\n");
		//	}
		//	if (_fateNum == 0)
		//	{
		//		DrawFormatString(450, 70, 0x000000, "なんと1000円がでてきた!\n");
		//	}
		//	if (_fateNum > 0)
		//	{
		//		DrawFormatString(450, 70, 0x000000, "体中に電流が流れた!!");
		//	}
		//}
		//// 去る
		//DrawGraph(600, 345, _sentakusiPNG[10], true);

		buttonSt->Draw(*this, *game);
	}

	// 宝箱出現中
	if (_event == EVENT_STATE::CHEST && !_eventMonsFlg)
	{
		//// メッセージボックス
		//DrawGraph(420, 50, _messagePNG, true);
		//// 宝箱チェック
		//int a = 0;
		//for (int i = 0; i < 4; i++)
		//{
		//	if (_chestPos[i].x == GameScene::plPosX && _chestPos[i].y == GameScene::plPosY)
		//	{
		//		a = i;
		//	}
		//}
		//if (_chestOpen[a] == 0)
		//{		
		//	// 進む(宝箱無視)
		//	DrawGraph(600, 345, _sentakusiPNG[10], true);
		//
		//	if (_fateNum == -1)
		//	{
		//		// 開ける
		//		DrawGraph(600, 200, _sentakusiPNG[3], true);
		//		DrawGraph(350, 150, _chestPNG[0], true);
		//		DrawFormatString(450, 70, 0x000000, "宝箱が置いてある");
		//	}
		//		
		//	// 鑑定アイテムを使ったときの描画
		//	if (menu->GetMeganeFlg())
		//	{
		//		if (_chestBingo[a] == 1)
		//		{
		//			DrawFormatString(450, 70, 0xff0000, "\n\n特におかしいところはない");
		//		}
		//		if (_chestBingo[a] == 0)
		//		{
		//			DrawFormatString(450, 70, 0xff0000, "\n\nゴーストが取り憑いている");
		//		}
		//	}
		//}
		//else
		//{
		//	if (_getFlg)
		//	{
		//		// 取る
		//		DrawGraph(600, 200, _sentakusiPNG[8], true);
		//	}
		//
		//	if (_anounceFlg)
		//	{
		//		// 持ち物満タンだからもてない
		//		DrawFormatString(600, 180, 0xffffff, "所持品がいっぱいだ");
		//	}
		//
		//	if (_fateNum == 1)
		//	{
		//		DrawGraph(350, 150, eventImages["chestInItem"], true);
		//		DrawFormatString(450, 70, 0x000000, "アイテムが入っていた!");
		//	}
		//	else if (_fateNum == 0)
		//	{
		//		DrawGraph(350, 150, _chestPNG[1], true);
		//		DrawFormatString(450, 70, 0x000000, "ゴーストが現れ攻撃してきた!");
		//	}
		//	else
		//	{
		//		// 宝箱をすでに開けている
		//		DrawFormatString(450, 70, 0x000000, "宝箱は開いている");
		//		DrawGraph(350, 150, eventImages["chestKara"], true);
		//	}
		//
		//	// 去る(宝箱無視)
		//	DrawGraph(600, 345, _sentakusiPNG[10], true);
		//}

		chestSt->Draw(*this, *menu);
	}

	// 瓶出現中
	if (_event == EVENT_STATE::DRINK && !_eventMonsFlg)
	{
		//for (int i = 0; i < 4; i++)
		//{
		//	if (_buttonDrink[i].x == game->plPosX && _buttonDrink[i].y == game->plPosY)
		//	{
		//		_drinkNum = i;
		//	}
		//}
		//if (!_drinking[_drinkNum])
		//{
		//	// メッセージボックス
		//	DrawGraph(420, 50, _messagePNG, true);
		//	// 瓶画像
		//	DrawGraph(350, 250, eventImages["bin"], true);
		//	if (_fateNum == -1)
		//	{
		//		// 飲む
		//		DrawGraph(600, 200, _sentakusiPNG[1], true);
		//		DrawFormatString(450, 70, 0x000000, "[Drink Me]\nとかかれた瓶がある...");
		//	}
		//	if (_fateNum == 0)
		//	{
		//		DrawFormatString(450, 70, 0x000000, "体が頑丈になった!\n防御力が上がった");
		//	}
		//	if (_fateNum > 0)
		//	{
		//		DrawFormatString(450, 70, 0x000000, "毒にかかってしまった...");
		//	}
		//}
		//// 去る
		//DrawGraph(600, 345, _sentakusiPNG[10], true);

		drinkSt->Draw(*this, *game);
	}

	// 即死トラップ出現中
	if (_event == EVENT_STATE::TRAP)
	{
		// メッセージボックス
		DrawGraph(420, 50, _drawHandle["message"], true);
		// 去る
		DrawGraph(600, 345, _sentakusiPNG[10], true);
		// 調べる
		DrawGraph(600, 200, _sentakusiPNG[9], true);
		DrawGraph(200, 75,eventImages["zou"], true);

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
		DrawGraph(100, 75, eventImages["mons"], true);

		// 戦闘に入る前
		if (!_eventMonsFlg)
		{
			// メッセージボックス
			DrawGraph(420, 50, _drawHandle["message"], true);
			// 去る
			DrawGraph(600, 345, _sentakusiPNG[10], true);
			// 調べる
			DrawGraph(600, 200, _sentakusiPNG[11], true);
			DrawFormatString(450, 70, 0x000000, "敵が道をふさいでいる\n何か良い方法はないだろうか..");
		}
	}

	// イベントがNONでもeventmonFlg立ってたら描画してみる(画面に張り付き)
	if (_event == EVENT_STATE::NON && _eventMonsEncountFlg)
	{
		DrawGraph(100, 75, eventImages["mons"], true);
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
	_kyouseiButtleFlg = flag;
}

bool Event::GetCautionFlg(void)const
{
	return _kyouseiButtleFlg;
}

void Event::Enemy(GameScene* game, Player* player, Monster* monster)
{
	game->eventState = EVENT_STATE::NON;
	_event = EVENT_STATE::NON;

	// プレイヤーnext_level - 敵経験値
	// next_level <= 0ならnowNumを+1して、ステータス更新
	player->SetNextLevel(player->GetNextLevel() - monster->GetKeikenti());

	// 所持金を増やす
	player->SetMoney(player->GetMoney() + monster->GetMoney());
}

void Event::Trap(GameScene* game, Player* player, MouseCtl* mouse)
{
	if (mouse->GetClickTrg())
	{			
		if (mouse->GetPos().x >= 600 && mouse->GetPos().x <= 600 + 150 && mouse->GetPos().y >= 345 && mouse->GetPos().y <= 345 + 75)
		{
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

void Event::eventMons(GameScene* game, Monster* monster, Cards* cards, MouseCtl* mouse)
{
	_eventMonsEncountFlg = true;
	if (mouse->GetClickTrg()) 
	{			 
		if (mouse->GetPos().x >= 600 && mouse->GetPos().x <= 600 + 150 && mouse->GetPos().y >= 345 && mouse->GetPos().y <= 345 + 75)
		{
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
				// クリック音
				PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
				_eventMonsFlg = true;
				monster->SetEnemyNum(6, 0);
				cards->SetTurn(3);
			}
		}
	}
}