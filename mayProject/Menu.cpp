#include <DxLib.h>
#include <string>
#include "SelectScene.h"
#include "GameScene.h"
#include "Menu.h"
#include "Player.h"
#include "Item.h"
#include "Monster.h"
#include "Cards.h"

// static変数の実体<型>クラス名::変数名 = 初期化;
bool Menu::loadFlg = false;

struct item
{
	VECTOR2 pos;		// ボックスの位置
	ITEM _item;
	int png;			// 画像
}itemBox[12];

Menu::Menu()
{
	Init();
}

Menu::~Menu()
{
	// 音関係
	for (int i = 0; i < 5; i++)
	{
		DeleteSoundMem(_soundSE[i]);
	}
}

void Menu::Init(void)
{
	_menu = MENU::NON;
	_itemAction = ITEM::NON;
	_itemSetumei = ITEM::NON;

	// 見えないところに出しとく
	_equipShieldPos = { -100,-1 };
	_equipSwordPos = { -100,-1 };
	_choicePos = { -100,-100 };

	/*メニュー関係*/
	_chooseNum = -1;
	_menuBackPngFlg = false;
	_menuSelPngFlg = false;

	// ステータス・アイテム・つづけるのボタン位置
	for (int i = 0; i <= 2; i++)
	{
		menu_pair[i].first = { 350,100 + (i * 100) };
		menu_pair[i].second = static_cast<MENU>(static_cast<int>(MENU::STATUS) + i);
		buttonSize[i] = { 200, 100 };
	}

	// タイトルへのボタン位置
	menu_pair[3].first = { 300,425 };
	menu_pair[3].second = static_cast<MENU>(static_cast<int>(MENU::TO_TITLE));
	buttonSize[3] = { 140, 70 };

	// セーブのボタン位置
	menu_pair[4].first = { 475,425 };
	menu_pair[4].second = static_cast<MENU>(static_cast<int>(MENU::SAVE));
	buttonSize[4] = { 140, 70 };

	/*アイテム関係*/
	_powUpNum = 0;
	_equipDamage = 0;
	_equipGuard = 0;
	_useOrThrowAway = false;
	_nonNeedFlg = false;
	_nonDamageFlg = false;

	// テスト(ロード時にアイテム画像を描画する)
	//std::string potion = "image/potion.png";
	//std::pair<const char*, ITEM> pair;
	//pair.first = potion.c_str();
	//pair.second = ITEM::POTION;
	std::string itemName[static_cast<int>(ITEM::MAX)-1];
	std::pair<std::string, ITEM> pair[static_cast<int>(ITEM::MAX) - 1];

	// アイテム関係
	//ファイルを読み込む
	auto FileHandle_item = FileRead_open("csv/itemName.csv");
	if (FileHandle_item == NULL)
	{
		return; //エラー時の処理
	}

	std::string a = "image/";
	std::string b = ".png";
	for (int i = 0; i <= static_cast<int>(ITEM::MAX) - 2; i++)
	{
		char name[256];
		// 画像名 説明 費用
		//FileRead_scanf(FileHandle_item, "%[^,],%[^,],%d", _itemStruct[i].name, _itemStruct[i].setumei, &_itemStruct[i].itemCostMoney);
		//auto d = a + _itemStruct[i].name + b;
		//_itemStruct[i].png = LoadGraph(d.c_str());
		FileRead_scanf(FileHandle_item, "%[^,]",name);
		pair[i].first  = a + name + b;
		pair[i].second = static_cast<ITEM>(i+1);
	}
	//ファイルを閉じる
	FileRead_close(FileHandle_item);

	// アイテムボックスの位置と所持アイテムの状態
	for (int i = 0; i <= 11; i++)
	{
		itemBox[i].pos = { (((i % 3) + 3) * 100),((i / 3) + 1) * 100 - 30 };
		if (!loadFlg)
		{
			itemBox[i]._item = ITEM::NON;
		}
		else
		{
			// ロード時のアイテム処理
			if (itemBox[i]._item == pair[i].second)
			{
				itemBox[i]._item = pair[i].second;
				itemBox[i].png = LoadGraph(pair[i].first.c_str());
			}
			else
			{
				itemBox[i]._item = ITEM::NON;
			}
		}
	}

	pngInit();

	// SE
	_soundSE[0] = LoadSoundMem("sound/se/click.mp3");
	_soundSE[1] = LoadSoundMem("sound/se/healCard.mp3");
	_soundSE[2] = LoadSoundMem("sound/se/poison_care.mp3");
	_soundSE[3] = LoadSoundMem("sound/se/charge.mp3");	
	_soundSE[4] = LoadSoundMem("sound/se/dropItem.mp3");
}

void Menu::pngInit(void)
{
	// メニューのボタン3つ
	std::string menuButton = "image/menu/menuSel.png";
	LoadDivGraph(menuButton.c_str(), 3, 3, 1, 200, 100, _menuSelPNG);

	// タイトルへ戻るボタン
	std::string titleBackButton = "image/menuTitleBackButton.png";
	_menuTitleBackPNG = LoadGraph(titleBackButton.c_str());

	// セーブボタン
	std::string menuSave = "image/menuSave.png";
	_menuSavePNG = LoadGraph(menuSave.c_str());

	std::string menuback = "image/menu_window.png";
	_menuBackPNG = LoadGraph(menuback.c_str());

	// アイテムボックス背景
	std::string itembox = "image/itembox.png";
	_itemBoxPNG = LoadGraph(itembox.c_str());

	// 選択中のアイテム
	std::string itemChoice = "image/itemChoice.png";
	_itemChoicePNG = LoadGraph(itemChoice.c_str());

	// 使用の文字
	std::string use = "image/use.png";
	_usePNG = LoadGraph(use.c_str());

	// 捨てるの文字
	std::string suteru = "image/suteru.png";
	_suteruPNG = LoadGraph(suteru.c_str());

	// 戻るの文字
	std::string back = "image/back.png";
	_backPNG = LoadGraph(back.c_str());

	// 説明の後ろ画像
	std::string setumei = "image/setumei.png";
	_setumeiPNG = LoadGraph(setumei.c_str());

	// メニューボタン
	std::string menu = "image/menuButton.png";
	_menuPNG = LoadGraph(menu.c_str());
}

void Menu::Update(GameScene* game,Player* player, Monster* monster, Cards* cards)
{
	if (_menu == MENU::SAVE)
	{
		// セーブ処理を書いてみる
		//player->SaveTest();
		SaveTest(player);
	}

	// ゲーム画面戻し
	if (_menu == MENU::TO_GAME || _menu == MENU::SAVE)
	{
		_menuBackPngFlg = false;
		_menuSelPngFlg = false;
		_menu = MENU::NON;
	}

	// アイテム画面処理
	if (_menu == MENU::ITEM)
	{
		if (_mouse & MOUSE_INPUT_LEFT) {		 // マウスの左ボタンが押されていたら
			for (int i = 0; i <= 11; i++)
			{
				if (_cursorPos.x >= itemBox[i].pos.x && _cursorPos.x <= itemBox[i].pos.x + 100 && _cursorPos.y >= itemBox[i].pos.y && _cursorPos.y <= itemBox[i].pos.y + 100)
				{
					if (itemBox[i]._item != ITEM::NON)
					{
						// アイテムを選択
						_chooseNum = i;
						_choicePos = itemBox[i].pos;
						_itemSetumei = itemBox[i]._item;
						_useOrThrowAway = true;
						_nonNeedFlg = false;
					}

					//if (itemBox[i]._item == ITEM::POTION)
					//{
					//	// プレイヤーのHPを回復する
					//	_itemAction = ITEM::POTION;
					//	// 画像を消す(回復薬を消す)
					//	itemBox[i]._item = ITEM::NON;
					//	itemBox[i].png = -1;
					//}
					//
					//if (itemBox[i]._item == ITEM::SWORD)
					//{
					//	_itemAction = ITEM::SWORD;
					//}
					//
					//if (itemBox[i]._item == ITEM::SHIELD)
					//{
					//	_itemAction = ITEM::SHIELD;
					//}
				}
			}

			// 選択中のアイテムをどうするか
			if (_useOrThrowAway)
			{
				// 画像を消すところの共通処理
				auto lambdaPNGSakujyo = [&]() {
					_itemSetumei = ITEM::NON;
					itemBox[_chooseNum]._item = ITEM::NON;
					itemBox[_chooseNum].png = -1;
					_chooseNum = -1;
					_useOrThrowAway = false;
					_choicePos = { -100,-100 };
				};

				// 効果がないよフラグ
				if (itemBox[_chooseNum]._item == ITEM::POTION || itemBox[_chooseNum]._item == ITEM::POTION_BIG)
				{
					if (player->GetHP() == player->GetMaxHP())
					{
						_nonNeedFlg = true;
					}
				}

				if (itemBox[_chooseNum]._item == ITEM::DETOX)
				{
					if (player->GetCondition() == CONDITION::FINE)
					{
						_nonNeedFlg = true;
					}
				}

				if (itemBox[_chooseNum]._item == ITEM::MEGANE)
				{
					if (_meganeFlg || game->eventState != EVENT_STATE::CHEST)
					{
						_nonNeedFlg = true;
					}
				}

				if (itemBox[_chooseNum]._item == ITEM::ENEMY_1)
				{
					if (monster->GetEnemyState() != ENEMY_STATE::EXIST)
					{
						_nonNeedFlg = true;
					}
				}

				if (itemBox[_chooseNum]._item == ITEM::ENEMY_2)
				{
					if (monster->GetEnemyState() != ENEMY_STATE::EXIST || _nonDamageFlg)
					{
						_nonNeedFlg = true;
					}
				}

				if (itemBox[_chooseNum]._item == ITEM::ENEMY_3)
				{
					if (monster->GetEnemyState() != ENEMY_STATE::EXIST || cards->GetTurn() == monster->GetMaxTurn())
					{
						_nonNeedFlg = true;
					}
				}

				if (itemBox[_chooseNum]._item == ITEM::ENEMY_4)
				{
					if (monster->GetEnemyState() != ENEMY_STATE::EXIST || monster->GetEnemyNum() == 5 || monster->GetEnemyNum() == 6)
					{
						_nonNeedFlg = true;
					}
				}

				if (itemBox[_chooseNum]._item == ITEM::KYOUKA_POW)
				{
					if (_powUpNum == 5)
					{
						_nonNeedFlg = true;
					}
				}

				if (itemBox[_chooseNum]._item == ITEM::SKILL_FAST)
				{
					if (player->GetSkillCharge() == 0)
					{
						_nonNeedFlg = true;
					}
				}

				// 使うor装備
				if (_cursorPos.x >= 50 && _cursorPos.x <= 50 + 150 && _cursorPos.y >= 400 && _cursorPos.y <= 400 + 75)
				{
					// クリック音
					PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);

					// 装備(剣)ラムダ式
					auto lambdaSword = [&](ITEM item) {
						_itemAction = item;
						_equipSwordPos = itemBox[_chooseNum].pos;
						_chooseNum = -1;
						_useOrThrowAway = false;
						_choicePos = { -100,-100 };
					};

					// 装備(盾)ラムダ式
					auto lambdaShield = [&](ITEM item) {
						_itemAction = item;
						_equipShieldPos = itemBox[_chooseNum].pos;
						_chooseNum = -1;
						_useOrThrowAway = false;
						_choicePos = { -100,-100 };
					};

					// 商品アイテムに関して
					for (auto item : ITEM())
					{
						if (item >= ITEM::POTION && item <= ITEM::MEGANE)
						{
							if (itemBox[_chooseNum]._item == item)
							{
								if (!_nonNeedFlg)
								{
									_itemAction = item;
									lambdaPNGSakujyo();
								}
								else
								{
									_useOrThrowAway = false;
									_nonNeedFlg = false;
								}
							}
						}
					}

					//if (itemBox[_chooseNum]._item == ITEM::POTION)
					//{	
					//	if(!_nonNeedFlg)
					//	{
					//		// プレイヤーのHPを回復する
					//		_itemAction = ITEM::POTION;
					//		lambdaPNGSakujyo();
					//	}
					//	else
					//	{
					//		_useOrThrowAway = false;
					//		_nonNeedFlg = false;
					//	}
					//	
					//	// 画像を消す(回復薬を消す)
					//	//_itemSetumei = ITEM::NON;
					//	//itemBox[_chooseNum]._item = ITEM::NON;
					//	//itemBox[_chooseNum].png = -1;
					//	//_chooseNum = -1;
					//	//_useOrThrowAway = false;
					//	//_choicePos = { -100,-100 };
					//}
					//if (itemBox[_chooseNum]._item == ITEM::DETOX)
					//{
					//	if (!_nonNeedFlg)
					//	{
					//		// プレイヤーの毒を回復する
					//		_itemAction = ITEM::DETOX;
					//		lambdaPNGSakujyo();
					//	}
					//	else
					//	{
					//		_useOrThrowAway = false;
					//		_nonNeedFlg = false;
					//	}
					//	// 画像を消す(毒を治す)
					//	//_itemSetumei = ITEM::NON;
					//	//itemBox[_chooseNum]._item = ITEM::NON;
					//	//itemBox[_chooseNum].png = -1;
					//	//_chooseNum = -1;
					//	//_useOrThrowAway = false;
					//	//_choicePos = { -100,-100 };
					//}
					//if (itemBox[_chooseNum]._item == ITEM::KYOUKA_POW)
					//{
					//	if (!_nonNeedFlg)
					//	{
					//		// 一時的に攻撃力アップ(5ターンぐらい)
					//		_itemAction = ITEM::KYOUKA_POW;
					//		lambdaPNGSakujyo();
					//	}
					//	else
					//	{
					//		_useOrThrowAway = false;
					//		_nonNeedFlg = false;
					//	}
					//	// 画像を消す
					//	//_itemSetumei = ITEM::NON;
					//	//itemBox[_chooseNum]._item = ITEM::NON;
					//	//itemBox[_chooseNum].png = -1;
					//	//_chooseNum = -1;
					//	//_useOrThrowAway = false;
					//	//_choicePos = { -100,-100 };
					//}

					// 剣に関して
					for (auto item : ITEM())
					{
						if (item >= ITEM::SWORD && item <= ITEM::SWORD_LV3)
						{
							if (itemBox[_chooseNum]._item == item)
							{
								lambdaSword(item);
							}
						}
					}

					// 盾に関して
					for (auto item : ITEM())
					{
						if (item >= ITEM::SHIELD && item <= ITEM::SHIELD_LV3)
						{
							if (itemBox[_chooseNum]._item == item)
							{
								lambdaShield(item);
							}
						}
					}

					//if (itemBox[_chooseNum]._item == ITEM::SWORD)
					//{
					//	lambdaSword(ITEM::SWORD);
					//  _itemAction = ITEM::SWORD;
					//  _equipSwordPos = itemBox[_chooseNum].pos;
					//  _chooseNum = -1;
					//  _useOrThrowAway = false;
					//  _choicePos = { -100,-100 };
					//}
					//if (itemBox[_chooseNum]._item == ITEM::SWORD_LV2)
					//{
					//	lambdaSword(ITEM::SWORD_LV2);
					//	//_itemAction = ITEM::SWORD_LV2;
					//	//_equipSwordPos = itemBox[_chooseNum].pos;
					//	//_chooseNum = -1;
					//	//_useOrThrowAway = false;
					//	//_choicePos = { -100,-100 };
					//}

					//if (itemBox[_chooseNum]._item == ITEM::SHIELD)
					//{
					//	lambdaShield(ITEM::SHIELD);
					//	//_itemAction = ITEM::SHIELD;
					//	//_equipShieldPos = itemBox[_chooseNum].pos;
					//	//_chooseNum = -1;
					//	//_useOrThrowAway = false;
					//	//_choicePos = { -100,-100 };
					//}
					//if (itemBox[_chooseNum]._item == ITEM::SHIELD_LV2)
					//{
					//	lambdaShield(ITEM::SHIELD_LV2);
					//}

					// ドロップアイテムに関して
					for (auto item : ITEM())
					{
						if (item >= ITEM::ENEMY_1 && item <= ITEM::POTION_BIG)
						{
							if (itemBox[_chooseNum]._item == item)
							{
								if (!_nonNeedFlg)
								{
									_itemAction = item;
									lambdaPNGSakujyo();
								}
								else
								{
									_useOrThrowAway = false;
									_nonNeedFlg = false;
								}
							}
						}
					}

					//if (itemBox[_chooseNum]._item == ITEM::ENEMY_1)
					//{
					//	if (!_nonNeedFlg)
					//	{
					//		// 敵に固定ダメージ20
					//		_itemAction = ITEM::ENEMY_1;
					//		lambdaPNGSakujyo();
					//	}
					//	else
					//	{
					//		_useOrThrowAway = false;
					//		_nonNeedFlg = false;
					//	}
					//	// 画像を消す
					//	//_itemSetumei = ITEM::NON;
					//	//itemBox[_chooseNum]._item = ITEM::NON;
					//	//itemBox[_chooseNum].png = -1;
					//	//_chooseNum = -1;
					//	//_useOrThrowAway = false;
					//	//_choicePos = { -100,-100 };
					//}
					//if (itemBox[_chooseNum]._item == ITEM::ENEMY_2)
					//{
					//	if (!_nonNeedFlg)
					//	{
					//		// 次のターンは敵の攻撃無効化
					//		_itemAction = ITEM::ENEMY_2;
					//		lambdaPNGSakujyo();
					//	}
					//	else
					//	{
					//		_useOrThrowAway = false;
					//		_nonNeedFlg = false;
					//	}
					//}
					//if (itemBox[_chooseNum]._item == ITEM::POTION_BIG)
					//{
					//	if (!_nonNeedFlg)
					//	{
					//		// プレイヤーのHPを大回復する
					//		_itemAction = ITEM::POTION_BIG;
					//		lambdaPNGSakujyo();
					//	}
					//	else
					//	{
					//		_useOrThrowAway = false;
					//		_nonNeedFlg = false;
					//	}
					//	// 画像を消す
					//	//_itemSetumei = ITEM::NON;
					//	//itemBox[_chooseNum]._item = ITEM::NON;
					//	//itemBox[_chooseNum].png = -1;
					//	//_chooseNum = -1;
					//	//_useOrThrowAway = false;
					//	//_choicePos = { -100,-100 };
					//}
				}

				// 捨てる
				if (_cursorPos.x >= 50 && _cursorPos.x <= 50 + 150 && _cursorPos.y >= 500 && _cursorPos.y <= 500 + 75)
				{
					// クリック音
					PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);

					// 剣に関して
					for (auto item : ITEM())
					{
						if (item >= ITEM::SWORD && item <= ITEM::SWORD_LV3)
						{
							if (itemBox[_chooseNum]._item == item)
							{
								_equipSwordPos = { -100,-1 };
								_equipDamage = 0;
							}
						}
					}

					// 盾に関して
					for (auto item : ITEM())
					{
						if (item >= ITEM::SHIELD && item <= ITEM::SHIELD_LV3)
						{
							if (itemBox[_chooseNum]._item == item)
							{
								_equipShieldPos = { -100,-1 };
								_equipGuard = 0;
							}
						}
					}

					//if (itemBox[_chooseNum]._item == ITEM::SHIELD)
					//{
					//	_equipShieldPos = { -100,-1 };
					//	_equipGuard = 0;
					//}
					//if (itemBox[_chooseNum]._item == ITEM::SWORD)
					//{
					//	_equipSwordPos = { -100,-1 };
					//	_equipDamage = 0;
					//}
					//if (itemBox[_chooseNum]._item == ITEM::SWORD_LV2)
					//{
					//	_equipSwordPos = { -100,-1 };
					//	_equipDamage = 0;
					//}
					//if (itemBox[_chooseNum]._item == ITEM::SHIELD_LV2)
					//{
					//	_equipShieldPos = { -100,-1 };
					//	_equipGuard = 0;
					//}

					lambdaPNGSakujyo();
					// 画像を消す
					//_itemSetumei = ITEM::NON;
					//itemBox[_chooseNum]._item = ITEM::NON;
					//itemBox[_chooseNum].png = -1;
					//_chooseNum = -1;
					//_useOrThrowAway = false;
					//_choicePos = { -100,-100 };
				}
			}
		}
	}

	// 戦闘中はアイテムを使うたびに画面を戻す
	// 戦闘中は、アイテムを使うごとに敵の待機ターン-1にする
	auto lambdaBattle = [&]() {
		// メニュー画面を消す
		_menuBackPngFlg = false;
		_menuSelPngFlg = false;			// 文字消す
		_menu = MENU::NON;			// 状態を戻す
		_useOrThrowAway = false;	// 使うと捨てるの文字描画消す
		cards->SetTurn(cards->GetTurn() - 1);
	};

	// プレイヤーの回復薬による回復
	if (_itemAction == ITEM::POTION)
	{
		PlaySoundMem(_soundSE[1], DX_PLAYTYPE_BACK, true);
		player->SetHP(player->GetHP() + 30);
		_itemAction = ITEM::NON;
		if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
		{
			lambdaBattle();
		}
	}

	// プレイヤーの体力増加剤による増加
	if (_itemAction == ITEM::HEART)
	{
		PlaySoundMem(_soundSE[1], DX_PLAYTYPE_BACK, true);
		player->SetMaxHP(player->GetMaxHP() + 10);
		_itemAction = ITEM::NON;
		if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
		{
			lambdaBattle();
		}
	}

	// プレイヤーの回復薬による回復
	if (_itemAction == ITEM::POTION_BIG)
	{
		PlaySoundMem(_soundSE[1], DX_PLAYTYPE_BACK, true);
		player->SetHP(player->GetHP() + 50);
		_itemAction = ITEM::NON;
		if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
		{
			lambdaBattle();
		}
	}

	// 解毒薬による状態異常回復
	if (_itemAction == ITEM::DETOX)
	{
		PlaySoundMem(_soundSE[2], DX_PLAYTYPE_BACK, true);
		player->SetCondition(CONDITION::FINE);
		player->SetConditionTurn(-1);
		_itemAction = ITEM::NON;
		if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
		{
			lambdaBattle();
		}
	}

	// プレイヤースキル発動5ターン短縮
	if (_itemAction == ITEM::SKILL_FAST)
	{
		PlaySoundMem(_soundSE[3], DX_PLAYTYPE_BACK, true);
		player->SetSkillCharge(player->GetSkillCharge() - 5);
		if (player->GetSkillCharge() <= 0)
		{
			// 0を下回らないようにする
			player->SetSkillCharge(0);
		}
		_itemAction = ITEM::NON;
		if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
		{
			lambdaBattle();
		}
	}

	// 宝箱の中身を鑑定する
	if (_itemAction == ITEM::MEGANE)
	{
		PlaySoundMem(_soundSE[3], DX_PLAYTYPE_BACK, true);
		_meganeFlg = true;
		_itemAction = ITEM::NON;
		// メニュー画面を消す
		_menuBackPngFlg = false;
		_menuSelPngFlg = false;			// 文字消す
		_menu = MENU::NON;			// 状態を戻す
		_useOrThrowAway = false;	// 使うと捨てるの文字描画消す
	}

	// プレイヤーの装備
	// 剣に関して
	for (auto item : ITEM())
	{
		if (item >= ITEM::SWORD && item <= ITEM::SWORD_LV3)
		{
			if (itemBox[_chooseNum]._item == item)
			{
				_equipDamage = (static_cast<int>(item) - 6) * 5;
				_itemAction = ITEM::NON;
				if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
				{
					lambdaBattle();
				}
			}
		}
	}

	// 盾に関して
	for (auto item : ITEM())
	{
		if (item >= ITEM::SHIELD && item <= ITEM::SHIELD_LV3)
		{
			if (itemBox[_chooseNum]._item == item)
			{
				_equipGuard = (static_cast<int>(item) - 9) * 4;
				_itemAction = ITEM::NON;
				if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
				{
					lambdaBattle();
				}
			}
		}
	}

	// 剣
	//if (_itemAction == ITEM::SWORD)
	//{
	//	_equipDamage = 2;
	//	_itemAction = ITEM::NON;
	//	if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
	//	{
	//		lambdaBattle();
	//	}
	//}
	//// 剣2
	//if (_itemAction == ITEM::SWORD_LV2)
	//{
	//	_equipDamage = 10;
	//	_itemAction = ITEM::NON;
	//	if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
	//	{
	//		lambdaBattle();
	//	}
	//}
	//// 盾
	//if (_itemAction == ITEM::SHIELD)
	//{
	//	_equipGuard = 3;
	//	_itemAction = ITEM::NON;
	//	if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
	//	{
	//		lambdaBattle();
	//	}
	//}
	//// 盾2
	//if (_itemAction == ITEM::SHIELD_LV2)
	//{
	//	_equipGuard = 10;
	//	_itemAction = ITEM::NON;
	//	if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
	//	{
	//		lambdaBattle();
	//	}
	//}

	// 敵魂攻撃アイテム
	if (_itemAction == ITEM::ENEMY_1)
	{
		PlaySoundMem(_soundSE[4], DX_PLAYTYPE_BACK, true);
		// 固定ダメージ20
		monster->Damage(20,cards);
		_itemAction = ITEM::NON;

		//// メニュー画面を消す
		//_menuBackPngFlg = false;
		//_menuSelPngFlg = false;			// 文字消す
		//_menu = MENU::NON;			// 状態を戻す
		//_useOrThrowAway = false;	// 使うと捨てるの文字描画消す
		if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
		{
			lambdaBattle();
		}
	}

	if (_itemAction == ITEM::ENEMY_2)
	{
		PlaySoundMem(_soundSE[4], DX_PLAYTYPE_BACK, true);
		// 次のターンは敵の攻撃を無効化
		_nonDamageFlg = true;
		_itemAction = ITEM::NON;

		if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
		{
			lambdaBattle();
		}
	}

	if (_itemAction == ITEM::ENEMY_3)
	{
		PlaySoundMem(_soundSE[4], DX_PLAYTYPE_BACK, true);
		// 敵のターンを巻き戻す
		cards->SetTurn(monster->GetMaxTurn()+1);
		_itemAction = ITEM::NON;

		if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
		{
			lambdaBattle();
		}
	}

	if (_itemAction == ITEM::ENEMY_4)
	{
		PlaySoundMem(_soundSE[4], DX_PLAYTYPE_BACK, true);
		// 戦闘から逃走する
		_escapeFlg = true;
		_itemAction = ITEM::NON;

		if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
		{
			lambdaBattle();
		}
	}

	// 一時的に攻撃力up
	if (_itemAction == ITEM::KYOUKA_POW)
	{
		PlaySoundMem(_soundSE[3], DX_PLAYTYPE_BACK, true);
		_powUpNum = 5;
		_itemAction = ITEM::NON;

		//// メニュー画面を消す
		//_menuBackPngFlg = false;
		//_menuSelPngFlg = false;			// 文字消す
		//_menu = MENU::NON;			// 状態を戻す
		//_useOrThrowAway = false;	// 使うと捨てるの文字描画消す
		if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
		{
			lambdaBattle();
		}
	}
}

void Menu::Draw(Player* player, Item* item, Monster* monster)
{
	//DrawGraph(0, 0, _menuPNG, true);

	if (monster->GetEnemyState() != ENEMY_STATE::EXIST)
	{
		DrawGraph(0, 0, _menuPNG, true);
	}
	else
	{
		// アイテムのみ使用可能
		DrawGraph(0, 0, _menuSelPNG[1], true);
	}

	// メニュー画面の表示
	if (_menuBackPngFlg)
	{
		DrawGraph(200, 0, _menuBackPNG, true);
	}

	// メニュー項目
	if (_menuSelPngFlg)
	{
		for (int i = 0; i <= 2; i++)
		{
			DrawGraph(menu_pair[i].first.x, menu_pair[i].first.y, _menuSelPNG[i], true);
		}

		DrawGraph(menu_pair[3].first.x, menu_pair[3].first.y, _menuTitleBackPNG, true);	
		DrawGraph(menu_pair[4].first.x, menu_pair[4].first.y, _menuSavePNG, true);
	}

	// アイテム
	if (_menu == MENU::ITEM)
	{
		// 枠と入手アイテムの描画
		for (int i = 0; i <= 11; i++)
		{
			DrawGraph(itemBox[i].pos.x, itemBox[i].pos.y, _itemBoxPNG, true);
			// 入手しているアイテムの描画(していなかったら何も描画されないようにする)
			DrawGraph(itemBox[i].pos.x, itemBox[i].pos.y, itemBox[i].png, true);
		}

		DrawGraph(_choicePos.x, _choicePos.y, _itemChoicePNG, true);

		// 装備されたらそこにEの文字を出す
		DrawFormatString(_equipSwordPos.x + 10, _equipSwordPos.y + 10, GetColor(255, 0, 0), "E");
		DrawFormatString(_equipShieldPos.x + 10, _equipShieldPos.y + 10, GetColor(255, 0, 0), "E");
	}

	// ステータス画面
	if (_menu == MENU::STATUS)
	{
		DrawFormatString(350, 100, 0x000000, "レベル:%d", player->GetNowLevel());
		DrawFormatString(350, 130, 0x000000, "体力:%d / %d", player->GetHP(), player->GetMaxHP());
		DrawFormatString(350, 160, 0x000000, "攻撃力:%d(+ %d) = %d", player->GetAttackDamage(), _equipDamage, player->GetAttackDamage() + _equipDamage);
		DrawFormatString(350, 190, 0x000000, "防御力:%d(+ %d) = %d", player->GetDifense(), _equipGuard, player->GetDifense() + _equipGuard);
		DrawFormatString(350, 220, 0x000000, "次のレベルまで:残り%d", player->GetNextLevel());
		DrawFormatString(350, 250, 0x000000, "所持金:%d円", player->GetMoney());
		DrawFormatString(350, 280, 0x000000, "スキルチャージ完了まで:%d回", player->GetSkillCharge());
	}

	// メニューのステータス画面とアイテム画面の描画
	if (_menu == MENU::ITEM || _menu == MENU::STATUS)
	{
		// 戻る
		DrawGraph(375, 470, _backPNG, true);

		if (_useOrThrowAway)
		{
			// 説明の後ろ画像
			DrawGraph(0, 320, _setumeiPNG, true);

			// アイテムを使うことに効果があるとき
			if (!_nonNeedFlg)
			{
				// 使う
				DrawGraph(50, 400, _usePNG, true);
			}
			else
			{
				// アイテムを使うことに効果がないとき

				// 描画ブレンドモード変更
				SetDrawBlendMode(DX_BLENDMODE_ADD, 100);
				// 使う
				DrawGraph(50, 400, _usePNG, true);

				// 描画ブレンドモードをノーブレンドにする
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				DrawFormatString(20, 300, 0xffffff, "今使っても効果がない");
			}
			// 捨てる
			DrawGraph(50, 500, _suteruPNG, true);
			// メニューのアイテムで選択したアイテムの説明を出す
			if (_itemSetumei != ITEM::NON)
			{
				//_chooseNum = _menu->GetNumNum();
				DrawFormatString(25, 340, 0x000000, "%s\n", item->GetSetumei(static_cast<int>(_itemSetumei) - 1));
				//DrawFormatString(700, 450, 0xFFFFFF, "%s\n", item[static_cast<int>(_menu->Getitemsetumei()) -1].setumei);
			}
		}
	}
}

void Menu::MenuButton_NonEnemy(void)
{
	_mouse = GetMouseInput();					 //マウスの入力状態取得
	GetMousePoint(&_cursorPos.x, &_cursorPos.y);	 //マウスの座標取得

	if (_mouse & MOUSE_INPUT_LEFT) {				 //マウスの左ボタンが押されていたら
		//X:0,Y:0
		// 文字表示中に上から項目が表示されないようにする
		if (!_menuBackPngFlg)
		{
			if (_cursorPos.x >= 0 && _cursorPos.x <= 0 + 150 && _cursorPos.y >= 0 && _cursorPos.y <= 0 + 75)
			{
				// クリック音
				PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);

				_menuBackPngFlg = true;
				_menuSelPngFlg = true;
			}
		}

		if (_menuSelPngFlg)
		{
			// メニュー項目ボタンとの当たり判定
			for (int i = 0; i <= 4; i++)
			{
				if (_cursorPos.x >= menu_pair[i].first.x && _cursorPos.x <= menu_pair[i].first.x + buttonSize[i].x && _cursorPos.y >= menu_pair[i].first.y && _cursorPos.y <= menu_pair[i].first.y + buttonSize[i].y)
				{
					// クリック音
					PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);

					_menu = menu_pair[i].second;
					_menuSelPngFlg = false;	// 文字消す
				}
			}
		}

		// 戻るボタンを押したらゲーム再開
		if (_menu == MENU::ITEM || _menu == MENU::STATUS)
		{
			if (_cursorPos.x >= 375 && _cursorPos.x <= 375 + 150 && _cursorPos.y >= 470 && _cursorPos.y <= 470 + 60)
			{
				// クリック音
				PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);

				_menuBackPngFlg = false;
				_menuSelPngFlg = false;			// 文字消す
				_menu = MENU::NON;			// 状態を戻す
				_useOrThrowAway = false;	// 使うと捨てるの文字描画消す
			}
		}
	}
}

void Menu::MenuButton_Enemy(void)
{
	// アイテムボタンのみ表示
	_mouse = GetMouseInput();					 //マウスの入力状態取得
	GetMousePoint(&_cursorPos.x, &_cursorPos.y); //マウスの座標取得

	if (_mouse & MOUSE_INPUT_LEFT) {				 //マウスの左ボタンが押されていたら
		if (_cursorPos.x >= 0 && _cursorPos.x <= 0 + 200 && _cursorPos.y >= 0 && _cursorPos.y <= 0 + 100)
		{
			// クリック音
			PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);

			_menu = MENU::ITEM;
			_menuBackPngFlg = true;
		}

		// 戻るボタンを押したらゲーム再開
		if (_menu == MENU::ITEM || _menu == MENU::STATUS)
		{
			if (_cursorPos.x >= 375 && _cursorPos.x <= 375 + 150 && _cursorPos.y >= 470 && _cursorPos.y <= 470 + 60)
			{
				// クリック音
				PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);

				_menuBackPngFlg = false;
				_menuSelPngFlg = false;			// 文字消す
				_menu = MENU::NON;			// 状態を戻す
				_useOrThrowAway = false;	// 使うと捨てるの文字描画消す
			}
		}
	}
}

void Menu::Setitem(ITEM item, int png)
{
	//元は12
	for (int i = 0; i < 12; i++)
	{
		// 入れられる場所を探して入れる
		if (itemBox[i]._item == ITEM::NON)
		{
			itemBox[i]._item = item;
			itemBox[i].png = png;
			return;						// 入れたらfor文を抜けるようにする
		}
	}
	//itemBox[0]._item = item;
}

bool Menu::GetMenuFlg(void)
{
	return _menuSelPngFlg;
}

MENU Menu::GetMenu(void)
{
	return _menu;
}

int Menu::GetCanHaveItem(void)
{
	int canHave = 0;
	//元は12
	for (int i = 0; i < 12; i++)
	{
		// 入れられる場所を探して入れる
		if (itemBox[i]._item == ITEM::NON)
		{
			canHave++;
		}
	}

	// 最後まで0なら、入れられるところがない
	return canHave;
}

int Menu::GetEquipDamage(void)
{
	return _equipDamage;
}

int Menu::GetEquipGuard(void)
{
	return _equipGuard;
}

int Menu::GetPowUp(void)
{
	return _powUpNum;
}

void Menu::SetPowUp(int num)
{
	// 攻撃するごとに-1されていく
	_powUpNum = num;
}

bool Menu::GetNonDamageFlg(void)
{
	return _nonDamageFlg;
}

void Menu::SetNonDamageFlg(bool flag)
{
	_nonDamageFlg = flag;
}

bool Menu::GetMenuBackPngFlg(void)
{
	return _menuBackPngFlg;
}

bool Menu::GetEscapeFlg(void)
{
	return _escapeFlg;
}

void Menu::SetEscapeFlg(bool flag)
{
	_escapeFlg = flag;
}

bool Menu::GetMeganeFlg(void)
{
	return _meganeFlg;
}

void Menu::SetMeganeFlg(bool flag)
{
	_meganeFlg = flag;
}

void Menu::SaveTest(Player* player)
{
	if (MessageBox(			// メッセージ
		NULL,
		"現在の状態をセーブしますか?",
		"確認ダイアログ",
		MB_OKCANCEL
	) == IDOK)
	{
		// セーブをする
		FILE* file;
		fopen_s(&file, "data/save1.csv", "wb");
		fprintf(file, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n", player->GetNowLevel(), player->GetMaxHP(), player->GetHP(), player->GetAttackDamage(), player->GetDifense(), player->GetNextLevel(), player->GetMoney(), player->GetConditionTurn(), player->GetCondition());
		fprintf(file, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", itemBox[0]._item, itemBox[1]._item, itemBox[2]._item, itemBox[3]._item, itemBox[4]._item, itemBox[5]._item, itemBox[6]._item, itemBox[7]._item, itemBox[8]._item, itemBox[9]._item, itemBox[10]._item, itemBox[11]._item);
		fclose(file);
	}
}

void Menu::LoadTest()
{
	if (MessageBox(			// メッセージ
		NULL,
		"ロードしますか?",
		"確認ダイアログ",
		MB_OKCANCEL
	) == IDOK)
	{
		// ロードをする
		//ファイルを読み込む
		int FileHandle;
		FileHandle = FileRead_open("data/save1.csv");
		if (FileHandle == NULL)
		{
			return;
		}

		FileRead_scanf(FileHandle, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n", &Player::saveData[0], &Player::saveData[1], &Player::saveData[2], &Player::saveData[3], &Player::saveData[4], &Player::saveData[5], &Player::saveData[6], &Player::saveData[7], &Player::saveData[8]);
		FileRead_scanf(FileHandle, "%d,%d,%d,%d,%d,%d,%d,%d,%d", &itemBox[0]._item, &itemBox[1]._item, &itemBox[2]._item, &itemBox[3]._item, &itemBox[4]._item, &itemBox[5]._item, &itemBox[6]._item, &itemBox[7]._item, &itemBox[8]._item, &itemBox[9]._item, &itemBox[10]._item, &itemBox[11]._item);

		//ファイルを閉じる
		FileRead_close(FileHandle);

		Player::loadFlg = true;
		loadFlg = true;
		SelectScene::pushFlg = true;
	}
	else if (MessageBox(			// メッセージ
		NULL,
		"はじめからしますか?",
		"確認ダイアログ",
		MB_OKCANCEL
	) == IDOK)
	{
		SelectScene::pushFlg = true;
	}
	else
	{
		SelectScene::pushFlg = false;
	}
}
