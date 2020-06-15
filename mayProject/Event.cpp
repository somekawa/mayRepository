#include "Dxlib.h"
#include "Event.h"
#include "GameScene.h"
#include "Player.h"
#include "Cards.h"
#include "SelectScene.h"

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

	// メッセージ
	std::string message = "image/message.png";
	_messagePNG = LoadGraph(message.c_str());
	// 商人の持ち物の後ろ枠
	std::string frame = "image/frame.png";
	_syouninWakuPNG = LoadGraph(frame.c_str());
	// アイテムボックス背景
	std::string itembox = "image/itembox.png";
	_itemBoxPNG = LoadGraph(itembox.c_str());
	// 選択中のアイテム
	std::string itemChoice = "image/itemChoice.png";
	_itemChoicePNG = LoadGraph(itemChoice.c_str());
	// 売り切れの文字
	//std::string soldout = "image/soldout.png";
	//_soldOutPNG = LoadGraph(soldout.c_str());
	// 宝箱
	std::string trasure_0 = "image/trasure_0.png";
	LoadDivGraph(trasure_0.c_str(), 2, 2, 1, 390 / 2, 431, _chestPNG);
	// 文字画像の分割読み込み
	std::string sentakusi = "image/sentakusi/sentakusi.png";
	LoadDivGraph(sentakusi.c_str(),12, 12, 1, 150, 75, _sentakusiPNG);
	// 商品ページを移動するための矢印の画像
	std::string yajirusi = "image/yajirusi.png";
	_yajirusiPNG = LoadGraph(yajirusi.c_str());
}

void Event::UpDate(GameScene* game, Player* player, Menu* menu, Item* item, Monster* monster,Cards* cards)
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
			Yado(game, player);
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
			Syounin(game, player, menu, item);
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
			Button(game, player);
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
			Chest(game, player, menu, item);
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
			Drink(game, player);
		}
	}

	if (_event == EVENT_STATE::TRAP)
	{
		Trap(game,player);
	}

	if (_event == EVENT_STATE::ENEMY)
	{
		Enemy(game, player, monster);
	}

	if (_event == EVENT_STATE::EVE_MONS)
	{
		eventMons(game, monster,cards);
	}
}

void Event::Draw(GameScene* game, Player* player, Menu* menu, Item* item)
{
	// 宿屋
	if (_event == EVENT_STATE::YADO && !_eventMonsFlg)
	{
		// 人画像
		DrawGraph(0, 0, eventImages["heal_human"], true);
		// 去る
		DrawGraph(600, 345, _sentakusiPNG[10], true);
		// メッセージボックス
		DrawGraph(420, 50, _messagePNG, true);

		if (_nonMoneyFlg)
		{
			DrawFormatString(450, 70, 0x000000, "怪しい老婆:\n%金が足らんようじゃの。\n去るがよい");
		}

		if (!_healYadoFlg)
		{
			// 頼む
			DrawGraph(600, 200, _sentakusiPNG[2], true);
			DrawFormatString(450, 70, 0x000000, "怪しい老婆:\n%d円で回復してやろうか? \n(所持金:%d円)", player->GetNowLevel() * 100,player->GetMoney());
		}
		else
		{
			if (!_nonMoneyFlg)
			{
				DrawFormatString(450, 70, 0x000000, "怪しい老婆:\nﾋｯﾋｯﾋｯﾋｯ...");
			}
		}
	}

	// 商人
	if (_event == EVENT_STATE::SYOUNIN && !_eventMonsFlg)
	{
		// 人画像
		DrawGraph(100, 0, eventImages["syounin"], true);
		// メッセージボックス
		DrawGraph(420, 50, _messagePNG, true);
		DrawFormatString(450, 70, 0x000000, "商人:\n何か買うか?");

		if (_buyFlg)
		{
			DrawGraph(200, 0, _syouninWakuPNG, true);

			// 1ページ目
			if (!_itemNextPage)
			{
				// アイテム表示
				for (int i = 0; i <= 7; i++)
				{
					DrawGraph(item->GetPos(i).x, item->GetPos(i).y, _itemBoxPNG, true);
					DrawGraph(item->GetPos(i).x, item->GetPos(i).y, item->GetPair(i).first, true);
				}
			}
			else
			{
				// 2ページ目
				// アイテム表示
				for (int i = 0; i <= 7; i++)
				{
					DrawGraph(item->GetPos(i).x, item->GetPos(i).y, _itemBoxPNG, true);
					DrawGraph(item->GetPos(i).x, item->GetPos(i).y, item->GetPair(i+8).first, true);
				}
			}

			// 選択中のアイテムの枠
			if (_chooseNum >= 0)
			{
				DrawGraph(item->GetPos(_chooseNum).x, item->GetPos(_chooseNum).y, _itemChoicePNG, true);
			}

			// 現在の所持金の表示
			DrawFormatString(470, 485, 0x000000, "所持金:%d円", player->GetMoney());

			// 商品の次のページへの矢印
			DrawGraph(490,300, _yajirusiPNG, true);
		}

		// 去る
		DrawGraph(600, 345, _sentakusiPNG[10], true);

		if (!_buyFlg || _chooseFlg)
		{
			// 購入
			DrawGraph(600, 200, _sentakusiPNG[0], true);
			if (menu->GetCanHaveItem() == 0)
			{
				// 持ち物満タンだからもてないよ
				DrawFormatString(600, 160, 0xffffff, "所持品がいっぱいだ");
			}
		}

		if (_buyFlg && _chooseFlg)
		{
			if (_nonMoneyFlg)
			{
				// 所持金が足りないよ
				DrawFormatString(600, 180, 0xffffff, "所持金が足りない");
			}
		}

		// 文字表示(商品選択中のみ)
		if (_chooseNum != -1)
		{
			DrawFormatString(300, 450, 0x000000, "%s\n", item->GetSetumei(static_cast<int>(_itemInfo)-1));
			DrawFormatString(300, 470, 0x000000, "%d円\n", item->GetCostMoney(static_cast<int>(_itemInfo) - 1));
		}
	}

	// ボタン出現中
	if (_event == EVENT_STATE::BUTTON && !_eventMonsFlg)
	{
		for (int i = 0; i < 4; i++)
		{
			if (_buttonDrink[i].x == game->plPosX && _buttonDrink[i].y == game->plPosY)
			{
				_buttonNum = i;
			}
		}

		if (!_buttonPush[_buttonNum])
		{
			// メッセージボックス
			DrawGraph(420, 50, _messagePNG, true);

			if (_fateNum == -1)
			{
				// 押す
				DrawGraph(600, 200, _sentakusiPNG[4], true);
				DrawFormatString(450, 70, 0x000000, "壁にボタンがついている...\n");
			}

			if (_fateNum == 0)
			{
				DrawFormatString(450, 70, 0x000000, "なんと1000円がでてきた!\n");
			}

			if (_fateNum > 0)
			{
				DrawFormatString(450, 70, 0x000000, "体中に電流が流れた!!");
			}
		}

		// 去る
		DrawGraph(600, 345, _sentakusiPNG[10], true);
	}

	// 宝箱出現中
	if (_event == EVENT_STATE::CHEST && !_eventMonsFlg)
	{
		// メッセージボックス
		DrawGraph(420, 50, _messagePNG, true);

		// 宝箱チェック
		int a = 0;
		for (int i = 0; i < 4; i++)
		{
			if (_chestPos[i].x == GameScene::plPosX && _chestPos[i].y == GameScene::plPosY)
			{
				a = i;
			}
		}
		if (_chestOpen[a] == 0)
		{		
			// 進む(宝箱無視)
			DrawGraph(600, 345, _sentakusiPNG[10], true);
		
			if (_fateNum == -1)
			{
				// 開ける
				DrawGraph(600, 200, _sentakusiPNG[3], true);
				DrawGraph(350, 150, _chestPNG[0], true);
				DrawFormatString(450, 70, 0x000000, "宝箱が置いてある");
			}
				
			// 鑑定アイテムを使ったときの描画
			if (menu->GetMeganeFlg())
			{
				if (_chestBingo[a] == 1)
				{
					DrawFormatString(450, 70, 0xff0000, "\n\n特におかしいところはない");
				}
				if (_chestBingo[a] == 0)
				{
					DrawFormatString(450, 70, 0xff0000, "\n\nゴーストが取り憑いている");
				}
			}
		}
		else
		{
			if (_getFlg)
			{
				// 取る
				DrawGraph(600, 200, _sentakusiPNG[8], true);
			}
		
			if (_anounceFlg)
			{
				// 持ち物満タンだからもてない
				DrawFormatString(600, 180, 0xffffff, "所持品がいっぱいだ");
			}
		
			if (_fateNum == 1)
			{
				DrawGraph(350, 150, eventImages["chestInItem"], true);
				DrawFormatString(450, 70, 0x000000, "アイテムが入っていた!");
			}
			else if (_fateNum == 0)
			{
				DrawGraph(350, 150, _chestPNG[1], true);
				DrawFormatString(450, 70, 0x000000, "ゴーストが現れ攻撃してきた!");
			}
			else
			{
				// 宝箱をすでに開けている
				DrawFormatString(450, 70, 0x000000, "宝箱は開いている");
				DrawGraph(350, 150, eventImages["chestKara"], true);
			}
		
			// 去る(宝箱無視)
			DrawGraph(600, 345, _sentakusiPNG[10], true);
		}
	}

	// 瓶出現中
	if (_event == EVENT_STATE::DRINK && !_eventMonsFlg)
	{
		for (int i = 0; i < 4; i++)
		{
			if (_buttonDrink[i].x == game->plPosX && _buttonDrink[i].y == game->plPosY)
			{
				_drinkNum = i;
			}
		}

		if (!_drinking[_drinkNum])
		{
			// メッセージボックス
			DrawGraph(420, 50, _messagePNG, true);
			// 瓶画像
			DrawGraph(350, 250, eventImages["bin"], true);
			if (_fateNum == -1)
			{
				// 飲む
				DrawGraph(600, 200, _sentakusiPNG[1], true);
				DrawFormatString(450, 70, 0x000000, "[Drink Me]\nとかかれた瓶がある...");
			}

			if (_fateNum == 0)
			{
				DrawFormatString(450, 70, 0x000000, "体が頑丈になった!\n防御力が上がった");
			}

			if (_fateNum > 0)
			{
				DrawFormatString(450, 70, 0x000000, "毒にかかってしまった...");
			}
		}
		// 去る
		DrawGraph(600, 345, _sentakusiPNG[10], true);
	}

	// 即死トラップ出現中
	if (_event == EVENT_STATE::TRAP)
	{
		// メッセージボックス
		DrawGraph(420, 50, _messagePNG, true);
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
			DrawGraph(420, 50, _messagePNG, true);
			// 去る
			DrawGraph(600, 345, _sentakusiPNG[10], true);
			// 調べる
			DrawGraph(600, 200, _sentakusiPNG[11], true);
			DrawFormatString(450, 70, 0x000000, "敵が道をふさいでいる");
		}
	}

	// イベントがNONでもeventmonFlg立ってたら描画してみる(画面に張り付き)
	if (_event == EVENT_STATE::NON && _eventMonsEncountFlg)
	{
		DrawGraph(100, 75, eventImages["mons"], true);
		// メッセージボックス
		DrawGraph(420, 50, _messagePNG, true);
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

void Event::Yado(GameScene* game, Player* player)
{
	if (game->mouse & MOUSE_INPUT_LEFT) {			 //マウスの左ボタンが押されていたら
		if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 345 && game->cursorPos.y <= 345 + 75)
		{
			// クリック音
			PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
			// 去る
			game->backFlg = true;
			game->eventState = EVENT_STATE::NON;
			_event = EVENT_STATE::NON;
			_healYadoFlg = false;
			_nonMoneyFlg = false;
		}

		// 回復を頼む
		if (!_healYadoFlg)
		{
			if (player->GetMoney() < player->GetNowLevel() * 100)
			{
				// 所持金が足りなくて回復できない
				_healYadoFlg = true;
				_nonMoneyFlg = true;
			}

			if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 200 && game->cursorPos.y <= 200 + 75)
			{
				// クリック音
				PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);

				// お金が減るがHP全回復
				// 減るお金はその時の自分のレベルによって異なる
				player->SetMoney(player->GetMoney() - player->GetNowLevel() * 50);
				player->SetHP(player->GetMaxHP());
				_healYadoFlg = true;
			}
		}
	}
}

void Event::Syounin(GameScene* game, Player* player, Menu* menu, Item* item)
{
	if (game->mouse & MOUSE_INPUT_LEFT) {			 //マウスの左ボタンが押されていたら
		if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 345 && game->cursorPos.y <= 345 + 75)
		{
			// クリック音
			PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
			// 去る
			game->backFlg = true;
			_chooseNum = -1;
			game->eventState = EVENT_STATE::NON;
			_event = EVENT_STATE::NON;
			_buyFlg = false;
			_chooseFlg = false;
			_nonMoneyFlg = false;
			// アイテムの補充処理
			//for (int i = 0; i <= 10; i++)
			//{
				//if (item_pair[i].first == _soldOutPNG)
				//{
				//	item_pair[i].first = item[i].png;
				//	item_pair[i].second = static_cast<ITEM>(static_cast<int>(ITEM::POTION) + i);
				//	_itemPos[i] = { (((i % 3) + 2) * 100) + 90,((i / 3) + 1) * 100 };
				//}
				//
				// アイテム補充で3以上進んでたら販売装備を一部変更させている
				// soldoutの画像をそれぞれで用意しちゃってるのでアドレスが違うから=にならない
				//if (item->GetPair(i).second == ITEM::NON)
				//{
					//item->SetHojuPair(i,_nowEvent);
					//item->SetPos(i);
				//}
			//}
		}

		if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 200 && game->cursorPos.y <= 200 + 75)
		{
			if (!_buyFlg)
			{
				// クリック音
				PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
				// 購入する
				// 購入したらプレイヤーの持ち物の中に追加する
				_buyFlg = true;
			}
		}
	}

	// 商人の持ち物との当たり判定
	if (_buyFlg)
	{
		if (game->mouse & MOUSE_INPUT_LEFT && !(game->mouseOld & MOUSE_INPUT_LEFT)) {			 //マウスの左ボタンが押されていたら
			// 次のページへが押された時
			if (game->cursorPos.x >= 490 && game->cursorPos.x <= 490 + 100 && game->cursorPos.y >= 300 && game->cursorPos.y <= 300 + 100)
			{
				_itemNextPage = !_itemNextPage;
			}

			for (int i = 0; i <= 7; i++)
			{
				if (game->cursorPos.x >= item->GetPos(i).x && game->cursorPos.x <= item->GetPos(i).x + 100 && game->cursorPos.y >= item->GetPos(i).y && game->cursorPos.y <= item->GetPos(i).y + 100)
				{
					if (item->GetPair(i).second != ITEM::NON)
					{
						// 購入するかのボタンを表示する
						_chooseFlg = true;
						_chooseNum = i;
						if (!_itemNextPage)
						{
							_itemInfo = item->GetPair(i).second;
						}
						else
						{
							_itemInfo = static_cast<ITEM>(static_cast<int>(item->GetPair(i).second) + 8);
						}
					}
				}
			}
		}
	}

	if (_chooseFlg)
	{
		// 所持金が足りない
		if (player->GetMoney() < item->GetCostMoney(static_cast<int>(_itemInfo) - 1))
		{
			_nonMoneyFlg = true;
		}
		else
		{
			_nonMoneyFlg = false;
		}

		if (game->mouse & MOUSE_INPUT_LEFT) {			 //マウスの左ボタンが押されていたら
			if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 200 && game->cursorPos.y <= 200 + 75)
			{
				// クリック音
				PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);

				if (item->GetPair(_chooseNum).second != ITEM::NON)
				{
					// choosenum→static_cast<int>(_itemInfo) - 1に変更
					if (player->GetMoney() < item->GetCostMoney(static_cast<int>(_itemInfo) - 1))
					{
						_chooseFlg = false;
					}
					else
					{
						// 所持品がいっぱいで入れられない
						if (menu->GetCanHaveItem() == 0)
						{
							_chooseFlg = false;
						}
						else
						{
							// プレイヤーのお金が減る
							// _chooseNumだと、装備に対応できない
							player->SetMoney(player->GetMoney() - item->GetCostMoney(static_cast<int>(_itemInfo) - 1));
							if (!_itemNextPage)
							{
								menu->Setitem(item->GetPair(_chooseNum).second, item->GetPair(_chooseNum).first);
								//item->SetSoldOutPair(_chooseNum);
							}
							else
							{
								menu->Setitem(item->GetPair(_chooseNum+8).second, item->GetPair(_chooseNum+8).first);
								//item->SetSoldOutPair(_chooseNum+8);
							}
							_chooseFlg = false;
						}
					}
				}
			}
		}
	}
}

void Event::Button(GameScene* game, Player* player)
{
	if (game->mouse & MOUSE_INPUT_LEFT) {			 //マウスの左ボタンが押されていたら
		if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 345 && game->cursorPos.y <= 345 + 75)
		{
			// クリック音
			PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
			// 去る
			game->backFlg = true;
			game->eventState = EVENT_STATE::NON;
			_event = EVENT_STATE::NON;
			_fateNum = -1;
			_buttonEventFlg = false;
			if (_pushFlg)
			{
				_buttonPush[_buttonNum] = true;
				_pushFlg = false;
			}
		}

		// 押す
		for (int i = 0; i < 4; i++)
		{
			if (_buttonDrink[i].x == game->plPosX && _buttonDrink[i].y == game->plPosY)
			{
				_buttonNum = i;
			}
		}

		if (_fateNum == -1 && !_buttonPush[_buttonNum])
		{
			if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 200 && game->cursorPos.y <= 200 + 75)
			{
				// クリック音
				PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
				_pushFlg = true;
				_fateNum = GetRand(2);	// 0 ~ 2
			}
		}
	}

	// ボタンを押すことにしたとき
	if (_pushFlg && !_buttonEventFlg)
	{
		if (_fateNum == 0)
		{
			// お金もらえる
			player->SetMoney(player->GetMoney() + 1000);
			//_pushFlg = false;
			_buttonEventFlg = true;
		}
		else
		{
			// 電流音
			PlaySoundMem(_soundSE[1], DX_PLAYTYPE_BACK, true);
			// 体力の1/4削る
			player->SetHP(player->GetHP() - (float)player->GetMaxHP() * (1.0f / 4.0f));
			//_pushFlg = false;
			game->shakeFlg = true;
			_buttonEventFlg = true;
		}
	}
}

void Event::Chest(GameScene* game, Player* player, Menu* menu, Item* item)
{
	if (game->mouse & MOUSE_INPUT_LEFT) {			 //マウスの左ボタンが押されていたら
		if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 345 && game->cursorPos.y <= 345 + 75)
		{
			// クリック音
			PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
			// 去る
			game->backFlg = true;
			game->eventState = EVENT_STATE::NON;
			_event = EVENT_STATE::NON;
			_pushFlg = false;
			_fateNum = -1;
			_anounceFlg = false;
			_getFlg = false;
			if (menu->GetMeganeFlg())
			{
				menu->SetMeganeFlg(false);
			}
		}

		// 宝箱チェック
		int a = 0;
		for (int i = 0; i < 4; i++)
		{
			if (_chestPos[i].x == GameScene::plPosX && _chestPos[i].y == GameScene::plPosY)
			{
				a = i;
			}
		}
		// 開ける
		if (_fateNum == -1 && _chestOpen[a] == 0)
		{
			if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 200 && game->cursorPos.y <= 200 + 75)
			{
				_pushFlg = true;
				_chestOpen[a] = 1;
				_fateNum = _chestBingo[a];
				if (menu->GetMeganeFlg())
				{
					menu->SetMeganeFlg(false);
				}
			}
		}
	}

	// 宝箱を開けることにしたとき
	if (_pushFlg)
	{
		if (_fateNum == 1)
		{
			_pushFlg = false;
			_getFlg = true;
		}
		else if(_fateNum == 0)
		{
			// ダメージ音
			PlaySoundMem(_soundSE[2], DX_PLAYTYPE_BACK, true);
			// 体力の1/4削る
			player->SetHP(player->GetHP() - (float)player->GetMaxHP() * (1.0f / 4.0f));
			_pushFlg = false;
			game->shakeFlg = true;
		}
	}

	if (_getFlg)
	{
		// 持ち物が満タンかどうか調べる
		if (menu->GetCanHaveItem() != 0)
		{
			_anounceFlg = false;
		}
		else
		{
			_anounceFlg = true;
		}

		if (game->mouse & MOUSE_INPUT_LEFT) {			 //マウスの左ボタンが押されていたら
			if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 200 && game->cursorPos.y <= 200 + 75)
			{
				if (menu->GetCanHaveItem() != 0)
				{
					// クリック音
					PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
					int randNum = GetRand(4);
					switch (randNum)	// 0~3
					{
					case 0:
						// 持ち物が満タンじゃなければ持てる
						menu->Setitem(item->GetPair(static_cast<int>(ITEM::POTION_BIG) - 1).second, item->GetPair(static_cast<int>(ITEM::POTION_BIG) - 1).first);
						break;
					case 1:
						// 持ち物が満タンじゃなければ持てる
						menu->Setitem(item->GetPair(static_cast<int>(ITEM::DETOX) - 1).second, item->GetPair(static_cast<int>(ITEM::DETOX) - 1).first);
						break;
					case 2:
						// 持ち物が満タンじゃなければ持てる
						menu->Setitem(item->GetPair(static_cast<int>(ITEM::KYOUKA_POW) - 1).second, item->GetPair(static_cast<int>(ITEM::KYOUKA_POW) - 1).first);
						break;
					case 3:
						// 持ち物が満タンじゃなければ持てる
						menu->Setitem(item->GetPair(static_cast<int>(ITEM::HEART) - 1).second, item->GetPair(static_cast<int>(ITEM::HEART) - 1).first);
						break;
					default:
						// 持ち物が満タンじゃなければ持てる
						menu->Setitem(item->GetPair(static_cast<int>(ITEM::POTION_BIG) - 1).second, item->GetPair(static_cast<int>(ITEM::POTION_BIG) - 1).first);
					}
					_getFlg = false;
					_anounceFlg = false;
				}
			}
		}
	}
}

void Event::Drink(GameScene* game, Player* player)
{
	if (game->mouse & MOUSE_INPUT_LEFT) {			 //マウスの左ボタンが押されていたら
		if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 345 && game->cursorPos.y <= 345 + 75)
		{
			// クリック音
			PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
			// 去る
			game->backFlg = true;
			game->eventState = EVENT_STATE::NON;
			_event = EVENT_STATE::NON;
			_fateNum = -1;

			_drinkEventFlg = false;
			if (_pushFlg)
			{
				_drinking[_drinkNum] = true;
				_pushFlg = false;
			}
		}

		for (int i = 0; i < 4; i++)
		{
			if (_buttonDrink[i].x == game->plPosX && _buttonDrink[i].y == game->plPosY)
			{
				_drinkNum = i;
			}
		}

		// 飲む
		if (_fateNum == -1 && !_drinking[_drinkNum])
		{
			if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 200 && game->cursorPos.y <= 200 + 75)
			{
				// クリック音
				PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
				_pushFlg = true;
				//_fateNum = GetRand(2);	// 0 ~ 2
				_fateNum = 1;
			}
		}
	}

	// 飲むことにしたとき
	if (_pushFlg && !_drinkEventFlg)
	{
		if (_fateNum == 0)
		{
			// ラッキー!基礎防御力が上がる
			player->SetDifense(player->GetDifense() + 2);
			//_pushFlg = false;
			_drinkEventFlg = true;
		}
		else
		{
			// 毒音
			PlaySoundMem(_soundSE[3], DX_PLAYTYPE_BACK, true);
			// 毒にかかる
			player->SetCondition(CONDITION::POISON);
			//_pushFlg = false;
			game->shakeFlg = true;
			_drinkEventFlg = true;
		}
	}
}

void Event::Trap(GameScene* game, Player* player)
{
	if (game->mouse & MOUSE_INPUT_LEFT) {			 //マウスの左ボタンが押されていたら
		if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 345 && game->cursorPos.y <= 345 + 75)
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
			if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 200 && game->cursorPos.y <= 200 + 75)
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

void Event::eventMons(GameScene* game, Monster* monster, Cards* cards)
{
	_eventMonsEncountFlg = true;
	if (game->mouse & MOUSE_INPUT_LEFT) {			 //マウスの左ボタンが押されていたら
		if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 345 && game->cursorPos.y <= 345 + 75)
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
			if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 200 && game->cursorPos.y <= 200 + 75)
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