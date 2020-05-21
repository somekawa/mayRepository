#include "Dxlib.h"
#include "Event.h"
#include "GameScene.h"
#include "Player.h"
#include "Cards.h"

Event::Event()
{
	Init();
}

Event::~Event()
{
	// 音関係
	for (int i = 0; i < 6; i++)
	{
		DeleteSoundMem(_soundSE[i]);
	}
}

void Event::Init(void)
{
	_event = EVENT_STATE::NON;
	_nowEvent = 0;
	_chooseNum = -1;
	_buyFlg = false;
	_chooseFlg = false;
	_healYadoFlg = false;	
	_nonMoneyFlg = false;
	_pushFlg = false;
	_fateNum = -1;
	_getFlg = false;
	_anounceFlg = false;
	_soundWalk = false;

	pngInit();

	// SE
	_soundSE[0] = LoadSoundMem("sound/se/click.mp3");
	_soundSE[1] = LoadSoundMem("sound/se/biribiri.mp3");
	_soundSE[2] = LoadSoundMem("sound/se/walk_short.mp3");
	_soundSE[3] = LoadSoundMem("sound/se/damage.mp3");
	_soundSE[4] = LoadSoundMem("sound/se/poison.mp3");
	_soundSE[5] = LoadSoundMem("sound/se/door.mp3");

	// 宝箱設定
	auto testFileHandle = FileRead_open("csv/chest.csv");
	if (testFileHandle == NULL)
	{
		return; //エラー時の処理
	}

	for (int i = 0; i < 2; i++)
	{
		FileRead_scanf(testFileHandle, "%d,%d,%d,%d", &chestOpen[i], &chestBingo[i], &chestPos[i].x, &chestPos[i].y);
	}

	//ファイルを閉じる
	FileRead_close(testFileHandle);
}

void Event::pngInit(void)
{
	// 宿屋
	std::string heal_human = "image/heal_human.png";
	_healHumanPNG = LoadGraph(heal_human.c_str());
	// 商人
	std::string syounin = "image/syounin.png";
	_syouninPNG = LoadGraph(syounin.c_str());
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
	std::string soldout = "image/soldout.png";
	_soldOutPNG = LoadGraph(soldout.c_str());
	// 宝箱
	std::string trasure_0 = "image/trasure_0.png";
	LoadDivGraph(trasure_0.c_str(), 2, 2, 1, 390 / 2, 431, _chestPNG);
	// 宝箱(アイテム時)
	std::string tresure_1 = "image/tresure_1.png";
	_chsetItemPNG = LoadGraph(tresure_1.c_str());
	// 謎の瓶
	std::string nazo_bin = "image/nazo_bin.png";
	_drinkPNG = LoadGraph(nazo_bin.c_str());
	// 文字画像の分割読み込み
	std::string sentakusi = "image/sentakusi/sentakusi.png";
	LoadDivGraph(sentakusi.c_str(),12, 12, 1, 150, 75, _sentakusiPNG);
	// 矢印
	std::string yajirusi = "image/yajirusi.png";
	yajirusiPNG = LoadGraph(yajirusi.c_str());
	// 空の宝箱
	std::string chest_kara = "image/chest_kara.png";
	karaPNG = LoadGraph(chest_kara.c_str());
	// 即死トラップの像
	std::string zou = "image/daiza.png";
	zouPNG = LoadGraph(zou.c_str());
	// イベント敵
	std::string eveMons = "image/monster/event_monster.png";
	eventMonsPNG = LoadGraph(eveMons.c_str());
}

void Event::UpDate(GameScene* game, Player* player, Menu* menu, Item* item, Monster* monster,Cards* cards)
{
	if (_event == EVENT_STATE::YADO)
	{
		Yado(game,player);
	}

	if (_event == EVENT_STATE::SYOUNIN)
	{
		Syounin(game, player,menu,item);
	}

	if (_event == EVENT_STATE::BUTTON)
	{
		Button(game, player);
	}

	if (_event == EVENT_STATE::CHEST)
	{
		Chest(game, player, menu, item);
	}

	if (_event == EVENT_STATE::DRINK)
	{
		Drink(game, player);
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
	// 歩行音からドア音までのつなぎ用
	// 1:再生中 0:再生していない
	if (CheckSoundMem(_soundSE[2]) == 1 && _soundWalk)
	{
		_soundWalk = true;
		_seCnt = 20;
	}
	else if (CheckSoundMem(_soundSE[2]) == 0 && _soundWalk)
	{
		if (_seCnt >= 0)
		{
			_seCnt--;
		}
		else
		{
			// ドア音
			PlaySoundMem(_soundSE[5], DX_PLAYTYPE_BACK, true);
			_soundWalk = false;
		}
	}
}

void Event::Draw(GameScene* game, Player* player, Menu* menu, Item* item)
{
	// 宿屋
	if (_event == EVENT_STATE::YADO)
	{
		// 人画像
		DrawGraph(0, 0, _healHumanPNG, true);
		// 進む
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
	if (_event == EVENT_STATE::SYOUNIN)
	{
		// 人画像
		DrawGraph(100, 0, _syouninPNG, true);
		// メッセージボックス
		DrawGraph(420, 50, _messagePNG, true);
		DrawFormatString(450, 70, 0x000000, "商人:\n何か買うか?");

		if (_buyFlg)
		{
			DrawGraph(200, 0, _syouninWakuPNG, true);

			// 枠の描画
			//for (int y = 100; y <= 400; y += 100)
			//{
			//	for (int x = 290; x <= 500; x += 100)
			//	{
			//		DrawGraph(x, y, _itemBoxPNG, true);
			//	}
			//}

			// 1ページ目
			if (!_itemNextPage)
			{
				// アイテム表示
				for (int i = 0; i <= 7; i++)
				{
					DrawGraph(item->GetPos(i).x, item->GetPos(i).y, _itemBoxPNG, true);
					DrawGraph(item->GetPos(i).x, item->GetPos(i).y, item->GetPair(i).first, true);

					//DrawGraph(_itemPos[i].x, _itemPos[i].y, _itemBoxPNG, true);
					//DrawGraph(_itemPos[i].x, _itemPos[i].y, item_pair[i].first, true);
				}
			}
			else
			{
				// 2ページ目
				// アイテム表示
				for (int i = 0; i <= 3; i++)
				{
					DrawGraph(item->GetPos(i).x, item->GetPos(i).y, _itemBoxPNG, true);
					DrawGraph(item->GetPos(i).x, item->GetPos(i).y, item->GetPair(i+8).first, true);
				}
			}
			//DrawGraph(_itemPos[0].x, _itemPos[0].y, item_pair[0].first, true);

			// 選択中のアイテムの枠
			if (_chooseNum >= 0)
			{
				DrawGraph(item->GetPos(_chooseNum).x, item->GetPos(_chooseNum).y, _itemChoicePNG, true);
				//DrawGraph(_itemPos[_chooseNum].x, _itemPos[_chooseNum].y, _itemChoicePNG, true);
			}

			// 現在の所持金の表示
			DrawFormatString(470, 485, 0x000000, "所持金:%d円", player->GetMoney());

			// 商品の次のページへの矢印
			DrawGraph(490,300, yajirusiPNG, true);
		}

		// 進む
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
			//DrawFormatString(300, 450, 0xFFFFFF, "%s\n", item->GetSetumei(_chooseNum));
			//DrawFormatString(300, 470, 0xFFFFFF, "%d円\n", item->GetCostMoney(_chooseNum));
			DrawFormatString(300, 450, 0x000000, "%s\n", item->GetSetumei(static_cast<int>(_itemInfo)-1));
			DrawFormatString(300, 470, 0x000000, "%d円\n", item->GetCostMoney(static_cast<int>(_itemInfo) - 1));

			//DrawFormatString(300, 450, 0xFFFFFF, "%s\n", item[_chooseNum].setumei);
			//DrawFormatString(300, 470, 0xFFFFFF, "%d円\n", item[_chooseNum].itemCostMoney);
			//DrawFormatString(150, 130, 0xFFFFFF, "%s\n", item[1].setumei);
			//DrawFormatString(150, 160, 0xFFFFFF, "%s\n", item[2].setumei);
		}
	}

	// ボタン出現中
	if (_event == EVENT_STATE::BUTTON)
	{
		// メッセージボックス
		DrawGraph(420, 50, _messagePNG, true);

		// 進む(ボタン無視)
		DrawGraph(600, 345, _sentakusiPNG[10], true);

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

	// 宝箱出現中
	if (_event == EVENT_STATE::CHEST)
	{
		// メッセージボックス
		DrawGraph(420, 50, _messagePNG, true);

		// 宝箱チェック
		int a = 0;
		for (int i = 0; i < 2; i++)
		{
			if (chestPos[i].x == GameScene::testx && chestPos[i].y == GameScene::testy)
			{
				a = i;
			}
		}
		//if (chestPos[0].x == GameScene::testx && chestPos[0].y == GameScene::testy)
		//{
			if (chestOpen[a] == 0)
			{
				//if (_getFlg)
				//{
				//	// 取る
				//	DrawGraph(600, 200, _sentakusiPNG[8], true);
				//}
				//
				//if (_anounceFlg)
				//{
				//	// 持ち物満タンだからもてない
				//	DrawFormatString(600, 180, 0xffffff, "所持品がいっぱいだ");
				//}
		
				// 進む(宝箱無視)
				DrawGraph(600, 345, _sentakusiPNG[10], true);
		
				if (_fateNum == -1)
				{
					// 開ける
					DrawGraph(600, 200, _sentakusiPNG[3], true);
					DrawGraph(350, 150, _chestPNG[0], true);
					DrawFormatString(450, 70, 0x000000, "宝箱が置いてある");
				}
		
				//if (_fateNum == 1)
				//{
				//	DrawGraph(350, 150, _chsetItemPNG, true);
				//	DrawFormatString(450, 70, 0x000000, "アイテムが入っていた!");
				//}
				//
				//if (_fateNum == 0)
				//{
				//	DrawGraph(350, 150, _chestPNG[1], true);
				//	DrawFormatString(450, 70, 0x000000, "中からゴーストが現れ\nあなたに攻撃してきた!");
				//}
		
				// 鑑定アイテムを使ったときの描画
				if (menu->GetMeganeFlg())
				{
					//if (game->chestFate == 0)
					//{
					//	DrawFormatString(450, 70, 0xff0000, "\n\n特におかしいところはない");
					//}
					//
					//if (game->chestFate > 0)
					//{
					//	DrawFormatString(450, 70, 0xff0000, "\n\nゴーストが見える");
					//}
		
					if (chestBingo[a] == 1)
					{
						DrawFormatString(450, 70, 0xff0000, "\n\n特におかしいところはない");
					}
		
					if (chestBingo[a] == 0)
					{
						DrawFormatString(450, 70, 0xff0000, "\n\nゴーストが見える");
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
					DrawGraph(350, 150, _chsetItemPNG, true);
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
					DrawGraph(350, 150, karaPNG, true);
				}
		
				// 進む(宝箱無視)
				DrawGraph(600, 345, _sentakusiPNG[10], true);
			}
		//}
	}

	// 瓶出現中
	if (_event == EVENT_STATE::DRINK)
	{
		// メッセージボックス
		DrawGraph(420, 50, _messagePNG, true);
		// 瓶画像
		DrawGraph(350, 250, _drinkPNG, true);
		// 進む(瓶無視)
		DrawGraph(600, 345, _sentakusiPNG[10], true);

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

	// 即死トラップ出現中
	if (_event == EVENT_STATE::TRAP)
	{
		// メッセージボックス
		DrawGraph(420, 50, _messagePNG, true);
		// 進む
		DrawGraph(600, 345, _sentakusiPNG[10], true);
		// 調べる
		DrawGraph(600, 200, _sentakusiPNG[9], true);
		DrawGraph(200, 75,zouPNG, true);

		if (nowTrapFlg)
		{
			if (eventmonsencountFlg && !eventmonsEndFlg)
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
			if (eventmonsencountFlg && !eventmonsEndFlg)
			{
				DrawFormatString(450, 90, 0xff0000, "背後に敵が迫っている…!");
			}
		}
	}

	// イベント敵と遭遇中
	if (_event == EVENT_STATE::EVE_MONS)
	{
		DrawGraph(100, 75, eventMonsPNG, true);

		// 戦闘に入る前
		if (!eventmonsFlg)
		{
			// メッセージボックス
			DrawGraph(420, 50, _messagePNG, true);
			// 去る
			DrawGraph(600, 345, _sentakusiPNG[10], true);
			// 調べる
			DrawGraph(600, 200, _sentakusiPNG[11], true);
			DrawFormatString(450, 70, 0x000000, "強そうな敵がいる");
		}
	}

	// イベントがNONでもeventmonFlg立ってたら描画してみる(画面に張り付き)
	if (_event == EVENT_STATE::NON && eventmonsencountFlg)
	{
		DrawGraph(100, 75, eventMonsPNG, true);
		// メッセージボックス
		DrawGraph(420, 50, _messagePNG, true);
		DrawFormatString(450, 70, 0x000000, "なんと敵が追いかけてきた!");
	}

}

void Event::SetEvent(EVENT_STATE state)
{
	_event = state;
}

int Event::GetNowEvent(void)
{
	return _nowEvent;
}

void Event::SetNowEvent(int num)
{
	_nowEvent = num;
}

void Event::SetFateNum(int num)
{
	_fateNum = num;
}

void Event::SetReset(void)
{
	// 宝箱の状態をリセットする
	for (int i = 0; i < 2; i++)
	{
		chestOpen[i] = 0;
	}

	// 即死トラップ発動中で死亡していたら新たな展開へ
	if (trapFlg)
	{
		nowTrapFlg = true;
	}
	trapFlg = false;
}

bool Event::GetEventMonsFlg(void)
{
	return eventmonsFlg;
}

void Event::SetEventMonsFlg(bool flag)
{
	eventmonsFlg = flag;
}

bool Event::GetEventMonsEndFlg(void)
{
	return eventmonsEndFlg;
}

void Event::SetEventMonsEncountFlg(bool flag)
{
	eventmonsencountFlg = flag;
}

bool Event::GetEventMonsEncountFlg(void)
{
	return eventmonsencountFlg;
}

void Event::Enemy(GameScene* game, Player* player, Monster* monster)
{
	_nowEvent++;

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
		// 回復いらない
		if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 345 && game->cursorPos.y <= 345 + 75)
		{
			// クリック音
			PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
			// 歩行音
			PlaySoundMem(_soundSE[2], DX_PLAYTYPE_BACK, true);

			game->_backFlg = true;

			// 先に進む
			game->walkCnt++;
			//game->moveFlg = true;
			game->eventState = EVENT_STATE::NON;
			_event = EVENT_STATE::NON;
			_nowEvent++;
			_healYadoFlg = false;
			_nonMoneyFlg = false;
			_soundWalk = true;
		}

		// 回復してほしい
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

				// お金が減るがHP満タン!
				// 減るお金はその時の自分のレベルによって異なる
				player->SetMoney(player->GetMoney() - player->GetNowLevel() * 100);
				player->SetHP(player->GetMaxHP());
				_healYadoFlg = true;
			}
		}
	}
}

void Event::Syounin(GameScene* game, Player* player, Menu* menu, Item* item)
{
	if (game->mouse & MOUSE_INPUT_LEFT) {			 //マウスの左ボタンが押されていたら
		// 何も買わない
		if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 345 && game->cursorPos.y <= 345 + 75)
		{
			// クリック音
			PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
			// 歩行音
			PlaySoundMem(_soundSE[2], DX_PLAYTYPE_BACK, true);

			game->_backFlg = true;

			// 先に進む
			game->walkCnt++;
			_chooseNum = -1;		// -1にしないと、選択した状態で進むを押したら次の商人でsoldoutになっちゃう
			game->eventState = EVENT_STATE::NON;
			_event = EVENT_STATE::NON;
			_nowEvent++;
			_buyFlg = false;
			_chooseFlg = false;
			_nonMoneyFlg = false;
			//game->moveFlg = true;
			_soundWalk = true;

			// アイテムの補充処理
			for (int i = 0; i <= 10; i++)
			{
				//if (item_pair[i].first == _soldOutPNG)
				//{
				//	item_pair[i].first = item[i].png;
				//	item_pair[i].second = static_cast<ITEM>(static_cast<int>(ITEM::POTION) + i);
				//	_itemPos[i] = { (((i % 3) + 2) * 100) + 90,((i / 3) + 1) * 100 };
				//}

				// アイテム補充で3以上進んでたら販売装備を一部変更させている
				// soldoutの画像をそれぞれで用意しちゃってるのでアドレスが違うから=にならない
				//if (item->GetPair(i).second == ITEM::NON)
				//{
					item->SetHojuPair(i,_nowEvent);
					item->SetPos(i);
				//}
			}

		}

		if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 200 && game->cursorPos.y <= 200 + 75)
		{
			if (!_buyFlg)
			{
				// クリック音
				PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);

				// 購入する
				// 背景枠を出してその中に商品をメニューと同じ風に描画
				// 購入したらプレイヤーの持ち物の中に追加する
				_buyFlg = true;
			}
		}
	}

	// 商人の持ち物との当たり判定
	if (_buyFlg)
	{
		if (game->mouse & MOUSE_INPUT_LEFT && !(game->mouseOld & MOUSE_INPUT_LEFT)) {			 //マウスの左ボタンが押されていたら

			//if (cursorPos.x >= _itemPos[0].x && cursorPos.x <= _itemPos[0].x + 100 && cursorPos.y >= _itemPos[0].y && cursorPos.y <= _itemPos[0].y + 100)
			//{
			//	if (item_pair[0].second != ITEM::NON)
			//	{
			//		// 本当はここで必要なお金と効果を表示して、購入するかやめるかのボタンを表示する
			//		// ボタンは、購入と進むを変更する位置に描画する
			//
			//		// プレイヤーのメニューに表示されるようにする
			//		//_menu->SetPNG(item_pair[0].first);
			//		//_menu->Setitem(item_pair[0].second);
			//
			//		// 買ったところにはsoldoutでも書いといて、itemをnonにする
			//		//item_pair[0].first = _soldOutPNG;
			//		//item_pair[0].second = ITEM::NON;
			//	}
			//}

			// 次のページへが押された時
			if (game->cursorPos.x >= 490 && game->cursorPos.x <= 490 + 100 && game->cursorPos.y >= 300 && game->cursorPos.y <= 300 + 100)
			{
				_itemNextPage = !_itemNextPage;
			}

			for (int i = 0; i <= 7; i++)
			{
				//if (cursorPos.x >= _itemPos[i].x && cursorPos.x <= _itemPos[i].x + 100 && cursorPos.y >= _itemPos[i].y && cursorPos.y <= _itemPos[i].y + 100)
				//{
				//	if (item_pair[i].second != ITEM::NON)
				//	{
				//		// 購入するかのボタンを表示する
				//		// ボタンは、購入と進むを変更する位置に描画する
				//		_chooseFlg = true;
				//		_chooseNum = i;
				//	}
				//}
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

				//if (item_pair[_chooseNum].second != ITEM::NON)
				//{
				//	// お金が足りない→買えない
				//	//if (_player->GetMoney() < item[_chooseNum].itemCostMoney)
				//	//{
				//		//_chooseFlg = false;
				//	//}
				//	//else
				//	{
				//		// プレイヤーのお金が減る
				//		//_player->SetMoney(_player->GetMoney() - item[_chooseNum].itemCostMoney);
				//
				//		// プレイヤーのメニューに表示されるようにする
				//		_menu->Setitem(item_pair[_chooseNum].second,item_pair[_chooseNum].first);
				//
				//		// 買ったところにはsoldoutでも書いといて、itemをnonにする
				//		item_pair[_chooseNum].first = _soldOutPNG;
				//		item_pair[_chooseNum].second = ITEM::NON;
				//		_chooseFlg = false;
				//	}
				//}

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
							//player->SetMoney(player->GetMoney() - item->GetCostMoney(_chooseNum));
							player->SetMoney(player->GetMoney() - item->GetCostMoney(static_cast<int>(_itemInfo) - 1));

							// プレイヤーのメニューに表示されるようにする
							//menu->Setitem(item->GetPair(_chooseNum).second, item->GetPair(_chooseNum).first);

							// 買ったところにはsoldoutにしてitemをnonにする
							if (!_itemNextPage)
							{
								menu->Setitem(item->GetPair(_chooseNum).second, item->GetPair(_chooseNum).first);
								item->SetSoldOutPair(_chooseNum);
							}
							else
							{
								menu->Setitem(item->GetPair(_chooseNum+8).second, item->GetPair(_chooseNum+8).first);
								item->SetSoldOutPair(_chooseNum+8);
							}
							//item->SetSoldOutPair(_chooseNum);
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
			// 歩行音
			PlaySoundMem(_soundSE[2], DX_PLAYTYPE_BACK, true);

			game->_backFlg = true;

			// 先に進む
			game->walkCnt++;
			//game->moveFlg = true;
			game->eventState = EVENT_STATE::NON;
			_event = EVENT_STATE::NON;
			_nowEvent++;
			_pushFlg = false;
			_fateNum = -1;
			_soundWalk = true;
		}

		// 押す
		if (_fateNum == -1)
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
	if (_pushFlg)
	{
		if (_fateNum == 0)
		{
			// ラッキー!お金もらえる
			player->SetMoney(player->GetMoney() + 1000);
			_pushFlg = false;
		}
		else
		{
			// 電流音
			PlaySoundMem(_soundSE[1], DX_PLAYTYPE_BACK, true);

			// アンラッキー!びりびりの刑で体力マイナス
			// 体力の1/4ぐらい削ろうかな
			player->SetHP(player->GetHP() - (float)player->GetMaxHP() * (1.0f / 4.0f));
			_pushFlg = false;
			game->shakeFlg = true;
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
			// 歩行音
			PlaySoundMem(_soundSE[2], DX_PLAYTYPE_BACK, true);

			game->_backFlg = true;

			// 先に進む
			game->walkCnt++;
			//game->moveFlg = true;
			game->eventState = EVENT_STATE::NON;
			_event = EVENT_STATE::NON;
			_nowEvent++;
			_pushFlg = false;
			_fateNum = -1;
			_anounceFlg = false;
			_getFlg = false;
			_soundWalk = true;
			if (menu->GetMeganeFlg())
			{
				menu->SetMeganeFlg(false);
			}
		}

		// 宝箱チェック
		int a = 0;
		for (int i = 0; i < 2; i++)
		{
			if (chestPos[i].x == GameScene::testx && chestPos[i].y == GameScene::testy)
			{
				a = i;
			}
		}
		// 開ける
		if (_fateNum == -1 && chestOpen[a] == 0)
		{
			if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 200 && game->cursorPos.y <= 200 + 75)
			{
				_pushFlg = true;
				//_fateNum = GetRand(2);	// 0 ~ 2
				//_fateNum = 0;
				//_fateNum = game->chestFate;
				//if (chestPos[i].x == GameScene::testx && chestPos[i].y == GameScene::testy)
				//{
					// あたりかはずれかをいれる
					// 開けたことにする
					chestOpen[a] = 1;
					_fateNum = chestBingo[a];
				//}
				if (menu->GetMeganeFlg())
				{
					menu->SetMeganeFlg(false);
				}
			}
		}
		//// 開ける
		//if (_fateNum == -1 && !chestOpen[0])
		//{
		//	if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 200 && game->cursorPos.y <= 200 + 75)
		//	{
		//		_pushFlg = true;
		//		//_fateNum = GetRand(2);	// 0 ~ 2
		//		//_fateNum = 0;
		//		//_fateNum = game->chestFate;
		//		if (chestPos[0].x == GameScene::testx && chestPos[0].y == GameScene::testy)
		//		{
		//			// あたりかはずれかをいれる
		//			// 開けたことにする
		//			chestOpen[0] = true;
		//			_fateNum = chestBingo[0];
		//		}
		//		if (menu->GetMeganeFlg())
		//		{
		//			menu->SetMeganeFlg(false);
		//		}
		//	}
		//}
	}

	// 宝箱を開けることにしたとき
	if (_pushFlg)
	{
		if (_fateNum == 1)
		{
			// ラッキー!アイテムがもらえる
			// プレイヤーのメニューに表示されるようにする
			//_menu->Setitem(item_pair[0].second, item_pair[0].first);
			//
			//menu->Setitem(item->GetPair(0).second, item->GetPair(0).first);
			//if (menu->GetCanHaveItem() != 0)
			//{
			//	// 持ち物が満タンじゃなければ持てる
			//	menu->Setitem(item->GetPair(0).second, item->GetPair(0).first);
			//}
			_pushFlg = false;
			_getFlg = true;
		}
		else if(_fateNum == 0)
		{
			// ダメージ音
			PlaySoundMem(_soundSE[3], DX_PLAYTYPE_BACK, true);

			// アンラッキー!びっくり箱だった
			// 体力の1/4ぐらい削ろうかな
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
					// 持ち物が満タンじゃなければ持てる
					//menu->Setitem(item->GetPair(static_cast<int>(ITEM::POTION_BIG) - 1).second, item->GetPair(static_cast<int>(ITEM::POTION_BIG)-1).first);
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
			// 歩行音
			PlaySoundMem(_soundSE[2], DX_PLAYTYPE_BACK, true);

			game->_backFlg = true;

			// 先に進む
			game->walkCnt++;
			//game->moveFlg = true;
			game->eventState = EVENT_STATE::NON;
			_event = EVENT_STATE::NON;
			_nowEvent++;
			_pushFlg = false;
			_fateNum = -1;
			_soundWalk = true;
		}

		// 飲む
		if (_fateNum == -1)
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

	// 飲むことにしたとき
	if (_pushFlg)
	{
		if (_fateNum == 0)
		{
			// ラッキー!基礎防御力が上がる
			player->SetDifense(player->GetDifense() + 2);
			_pushFlg = false;
		}
		else
		{
			// 毒音
			PlaySoundMem(_soundSE[4], DX_PLAYTYPE_BACK, true);

			// アンラッキー!毒にかかる
			player->SetCondition(CONDITION::POISON);
			_pushFlg = false;
			game->shakeFlg = true;
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
			// 歩行音
			PlaySoundMem(_soundSE[2], DX_PLAYTYPE_BACK, true);

			game->_backFlg = true;

			// 先に進む
			game->walkCnt++;
			//game->moveFlg = true;
			game->eventState = EVENT_STATE::NON;
			_event = EVENT_STATE::NON;
			_nowEvent++;
			_fateNum = -1;
			_soundWalk = true;
		}

		// 調べる
		if (!trapFlg)
		{
			if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 200 && game->cursorPos.y <= 200 + 75)
			{
				// クリック音
				PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
				trapFlg = true;
			}
		}
	}

	// 調べることにしたとき
	if (trapFlg)
	{
		// 即死トラップの発動
		player->SetHP(player->GetHP() - player->GetMaxHP());

		// とりあえずここで特定敵イベントを終了させる
		if (eventmonsencountFlg && !eventmonsEndFlg)
		{
			eventmonsencountFlg = false;
			eventmonsEndFlg = true;
		}
	}
}

void Event::eventMons(GameScene* game, Monster* monster, Cards* cards)
{
	eventmonsencountFlg = true;
	if (game->mouse & MOUSE_INPUT_LEFT) {			 //マウスの左ボタンが押されていたら
		if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 345 && game->cursorPos.y <= 345 + 75)
		{
			// クリック音
			PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
			// 歩行音
			PlaySoundMem(_soundSE[2], DX_PLAYTYPE_BACK, true);

			game->_backFlg = true;

			// 先に進む
			game->walkCnt++;
			//game->moveFlg = true;
			game->eventState = EVENT_STATE::NON;
			_event = EVENT_STATE::NON;
			_nowEvent++;
			_fateNum = -1;
			_soundWalk = true;
		}

		// 戦う
		if (!eventmonsFlg)
		{
			if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 200 && game->cursorPos.y <= 200 + 75)
			{
				// クリック音
				PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
				eventmonsFlg = true;
				auto ene = 6;
				monster->SetEnemyNum(ene, 0);
				cards->SetTurn(3);
			}
		}
		//if(eventmonsFlg)
		//{
		//	auto ene = 6;
		//	monster->SetEnemyNum(ene, 0);
		//	cards->SetTurn(3);
		//}
	}
}