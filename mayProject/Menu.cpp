#include <DxLib.h>
#include <string>
#include "Scene/SelectScene.h"
#include "Scene/GameScene.h"
#include "Menu.h"
#include "Player.h"
#include "Item/Item.h"
#include "Enemy/Monster.h"
#include "Card/Cards.h"
#include "Common/MouseCtl.h"

// static変数の実体<型>クラス名::変数名 = 初期化;
bool Menu::_loadFlg = false;

struct item
{
	VECTOR2 pos;		// ボックスの位置
	ITEM _item;			// アイテム種類
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

bool Menu::Init(void)
{
	_menu = MENU::NON;
	_itemAction = ITEM::NON;
	_itemExplanation = ITEM::NON;

	// 見えないところに出しておく
	_equipShieldPos = { -100,-1 };
	_equipSwordPos  = { -100,-1 };
	_choicePos      = { -100,-100 };

	/*メニュー関係*/
	_chooseNum = -1;
	_menuBackPngFlg = false;
	_menuSelPngFlg  = false;

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
	_equipGuard  = 0;
	_useOrThrowAway = false;
	_nonNeedFlg   = false;
	_nonDamageFlg = false;
	_escapeFlg    = false;
	_AppraisalFlg = false;

	std::string itemName[static_cast<int>(ITEM::MAX)-1];
	std::pair<std::string, ITEM> pair[static_cast<int>(ITEM::MAX) - 1];

	// アイテム関係
	// ファイルを読み込む
	const auto FileHandle_item = FileRead_open("csv/itemName.csv");
	if (FileHandle_item == NULL)
	{
		return false; //エラー
	}

	const std::string image = "image/";
	const std::string png   = ".png";
	for (int i = 0; i <= static_cast<int>(ITEM::MAX) - 2; i++)
	{
		char name[256];
		FileRead_scanf(FileHandle_item, "%s",name);
		name[255] = '\0';
		pair[i].first  = image + name + png;
		pair[i].second = static_cast<ITEM>(i+1);
	}
	//ファイルを閉じる
	FileRead_close(FileHandle_item);

	// アイテムボックスの位置と所持アイテムの状態
	for (int i = 0; i <= 11; i++)
	{
		itemBox[i].pos = { ((i % 3) + 3) * 100,((i / 3) + 1) * 100 - 30 };
		if (!_loadFlg)
		{
			itemBox[i]._item = ITEM::NON;
		}
		else
		{
			// ロード時のアイテム処理
			if (static_cast<int>(itemBox[i]._item) > 0)
			{
				itemBox[i].png = LoadGraph(pair[static_cast<int>(itemBox[i]._item)-1].first.c_str());
			}
			else
			{
				itemBox[i]._item = ITEM::NON;
			}
		}
	}

	PngInit();

	// SE
	_soundSE[0] = LoadSoundMem("sound/se/click.mp3");
	_soundSE[1] = LoadSoundMem("sound/se/healCard.mp3");
	_soundSE[2] = LoadSoundMem("sound/se/poison_care.mp3");
	_soundSE[3] = LoadSoundMem("sound/se/charge.mp3");	
	_soundSE[4] = LoadSoundMem("sound/se/dropItem.mp3");
	return true;
}

void Menu::PngInit(void)
{
	// メニューのボタン3つ
	std::string menuButton = "image/menu/menuSel.png";
	LoadDivGraph(menuButton.c_str(), 3, 3, 1, 200, 100, _menuSelPNG);

	_menuImages.try_emplace("titleBack", LoadGraph("image/menuTitleBackButton.png"));	// タイトルへ戻るボタン
	_menuImages.try_emplace("save", LoadGraph("image/menuSave.png"));					// セーブボタン
	_menuImages.try_emplace("menuButton", LoadGraph("image/menuButton.png"));			// メニュー画面背景
	_menuImages.try_emplace("menuWindow", LoadGraph("image/menu_window.png"));			// アイテムボックス背景
	_menuImages.try_emplace("use", LoadGraph("image/choice/use.png"));					// 使用の文字
	_menuImages.try_emplace("throw_away", LoadGraph("image/choice/throw_away.png"));	// 捨てるの文字
	_menuImages.try_emplace("back", LoadGraph("image/choice/back.png"));				// 戻るの文字
	_menuImages.try_emplace("description", LoadGraph("image/description.png"));			// 説明の後ろ画像
	_menuImages.try_emplace("itembox", LoadGraph("image/itembox.png"));					// アイテムボックス画像
	_menuImages.try_emplace("itemChoice", LoadGraph("image/itemChoice.png"));			// 選択中アイテム
}

void Menu::UseItem(void)
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
		if (item >= ITEM::POTION && item <= ITEM::GLASSES)
		{
			if (itemBox[_chooseNum]._item == item)
			{
				if (!_nonNeedFlg)
				{
					_itemAction = item;
					DeleteItem();
				}
				else
				{
					_useOrThrowAway = false;
					_nonNeedFlg = false;
				}
			}
		}
	}

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
					DeleteItem();
				}
				else
				{
					_useOrThrowAway = false;
					_nonNeedFlg = false;
				}
			}
		}
	}
}

void Menu::ThrowAwayItem(void)
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

	DeleteItem();
}

void Menu::DeleteItem(void)
{
	_itemExplanation = ITEM::NON;
	itemBox[_chooseNum]._item = ITEM::NON;
	itemBox[_chooseNum].png   = -1;
	_chooseNum = -1;
	_useOrThrowAway = false;
	_choicePos = { -100,-100 };
}

void Menu::GameContinue(const std::shared_ptr<MouseCtl>& mouse)
{
	// 戻るボタンを押したらゲーム再開
	if (_menu == MENU::ITEM || _menu == MENU::STATUS)
	{
		if (mouse->GetPos().x >= 375 && mouse->GetPos().x <= 375 + 150 &&
			mouse->GetPos().y >= 470 && mouse->GetPos().y <= 470 + 60)
		{
			// クリック音
			PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);

			_menuBackPngFlg = false;
			_menuSelPngFlg = false;	// 文字を消す
			_menu = MENU::NON;			// 状態を戻す
			_useOrThrowAway = false;	// 使うと捨てるの文字を消す
		}
	}
}

void Menu::Update(GameScene* game,const std::shared_ptr<Player>& player, const std::shared_ptr<Monster>& monster, const std::shared_ptr<Cards>& cards, const std::shared_ptr<MouseCtl>& mouse)
{
	if (_menu == MENU::SAVE)
	{
		// セーブ処理
		Save(player);
	}

	// ゲーム画面戻し
	if (_menu == MENU::TO_GAME || _menu == MENU::SAVE)
	{
		_menuBackPngFlg = false;
		_menuSelPngFlg = false;
		_menu = MENU::NON;
	}

	// アイテム画面処理(クリックされた時にITEM画面であれば処理を行う)
	if (mouse->GetClickTrg() && (_menu == MENU::ITEM))
	{
		for (int i = 0; i <= 11; i++)
		{
			if (mouse->GetPos().x >= itemBox[i].pos.x && mouse->GetPos().x <= itemBox[i].pos.x + 100 &&
				mouse->GetPos().y >= itemBox[i].pos.y && mouse->GetPos().y <= itemBox[i].pos.y + 100)
			{
				if (itemBox[i]._item != ITEM::NON)
				{
					// アイテムを選択
					_chooseNum = i;
					_choicePos = itemBox[i].pos;
					_itemExplanation = itemBox[i]._item;
					_useOrThrowAway  = true;
					_nonNeedFlg = false;
				}
			}
		}

		// 選択中のアイテムをどうするか
		if (_useOrThrowAway)
		{
			// アイテムを使用しても効果がないと知らせるフラグをtrueにする
			switch (itemBox[_chooseNum]._item)
			{
			case ITEM::POTION:
				if (player->GetHP() == player->GetMaxHP())
				{
					_nonNeedFlg = true;
				}
				break;
			case ITEM::POTION_BIG:
				if (player->GetHP() == player->GetMaxHP())
				{
					_nonNeedFlg = true;
				}
				break;
			case ITEM::DETOX:
				if (player->GetCondition() == CONDITION::FINE)
				{
					_nonNeedFlg = true;
				}
				break;
			case ITEM::GLASSES:
				if (_AppraisalFlg || game->eventState != EVENT_STATE::CHEST)
				{
					_nonNeedFlg = true;
				}
				break;
			case ITEM::ENEMY_1:
				if (monster->GetEnemyState() != ENEMY_STATE::EXIST)
				{
					_nonNeedFlg = true;
				}
				break;
			case ITEM::ENEMY_2:
				if (monster->GetEnemyState() != ENEMY_STATE::EXIST || _nonDamageFlg)
				{
					_nonNeedFlg = true;
				}
				break;
			case ITEM::ENEMY_3:
				if (monster->GetEnemyState() != ENEMY_STATE::EXIST || cards->GetTurn() == monster->GetMaxTurn())
				{
					_nonNeedFlg = true;
				}
				break;
			case ITEM::ENEMY_4:
				if (monster->GetEnemyState() != ENEMY_STATE::EXIST || monster->GetEnemyNum() == 5 || monster->GetEnemyNum() == 6)
				{
					_nonNeedFlg = true;
				}
				break;
			case ITEM::POW_UP:
				if (_powUpNum >= 5)
				{
					_nonNeedFlg = true;
				}
				break;
			case ITEM::SKILL_FAST:
				if (player->GetSkillCharge() == 0)
				{
					_nonNeedFlg = true;
				}
				break;
			default:
				break;
			}

			// 使うor装備
			if (mouse->GetPos().x >= 50 &&  mouse->GetPos().x <= 50 + 150 &&
				mouse->GetPos().y >= 400 && mouse->GetPos().y <= 400 + 75)
			{
				UseItem();
			}

			// 捨てる
			if (mouse->GetPos().x >= 50 && mouse->GetPos().x <= 50 + 150 &&
				mouse->GetPos().y >= 500 && mouse->GetPos().y <= 500 + 75)
			{
				ThrowAwayItem();
			}
		}
	}

	// 戦闘中はアイテムを使うたびに画面を戻す
	// 戦闘中は、アイテムを使うごとに敵の待機ターン-1にする
	auto BattleLambda = [&]() {
		// メニュー画面を消す
		_menuBackPngFlg = false;
		_menuSelPngFlg  = false;	// 文字消す
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
			BattleLambda();
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
			BattleLambda();
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
			BattleLambda();
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
			BattleLambda();
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
			BattleLambda();
		}
	}

	// 宝箱の中身を鑑定する
	if (_itemAction == ITEM::GLASSES)
	{
		PlaySoundMem(_soundSE[3], DX_PLAYTYPE_BACK, true);
		_AppraisalFlg = true;
		_itemAction = ITEM::NON;
		// メニュー画面を消す
		_menuBackPngFlg = false;
		_menuSelPngFlg = false;		// 文字消す
		_menu = MENU::NON;			// 状態を戻す
		_useOrThrowAway = false;	// 使うと捨てるの文字描画消す
	}

	// プレイヤーの装備処理
	bool tmpFlg = false;
	for (auto item : ITEM())
	{
		if (item >= ITEM::SWORD && item <= ITEM::SWORD_LV3)			// 剣に関して
		{
			if (itemBox[_chooseNum]._item != item)
			{
				continue;
			}
			_equipDamage = (static_cast<int>(item) - (static_cast<int>(ITEM::SWORD)-1)) * 5;
			tmpFlg = true;
		}
		else if (item >= ITEM::SHIELD && item <= ITEM::SHIELD_LV3)	// 盾に関して
		{
			if (itemBox[_chooseNum]._item != item)
			{
				continue;
			}
			_equipGuard = (static_cast<int>(item) - (static_cast<int>(ITEM::SHIELD) - 1)) * 4;
			tmpFlg = true;
		}
		else
		{
			// 何も処理を行わない
		}

		// 戦闘中のアイテム処理なのかを確かめる
		if (tmpFlg)	
		{
			_itemAction = ITEM::NON;
			if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
			{
				BattleLambda();
			}
			break;
		}
	}

	//// 盾に関して
	//for (auto item : ITEM())
	//{
	//	if (item >= ITEM::SHIELD && item <= ITEM::SHIELD_LV3)
	//	{
	//		if (itemBox[_chooseNum]._item != item)
	//		{
	//			continue;
	//		}
	//		_equipGuard = (static_cast<int>(item) - (static_cast<int>(ITEM::SHIELD) - 1)) * 4;
	//		_itemAction = ITEM::NON;
	//		if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
	//		{
	//			BattleLambda();
	//		}
	//	}
	//}

	bool tmpUseItem = false;
	if (_itemAction == ITEM::ENEMY_1)
	{
		PlaySoundMem(_soundSE[4], DX_PLAYTYPE_BACK, true);
		monster->Damage(20,cards);	// 固定ダメージ20
		tmpUseItem = true;
	}

	if (_itemAction == ITEM::ENEMY_2)
	{
		PlaySoundMem(_soundSE[4], DX_PLAYTYPE_BACK, true);
		_nonDamageFlg = true; // 次のターンは敵の攻撃を無効化
		tmpUseItem = true;
	}

	if (_itemAction == ITEM::ENEMY_3)
	{
		PlaySoundMem(_soundSE[4], DX_PLAYTYPE_BACK, true);
		cards->SetTurn(monster->GetMaxTurn()+1);  // プレイヤーの行動可能ターンを増やす
		tmpUseItem = true;
	}

	if (_itemAction == ITEM::ENEMY_4)
	{
		PlaySoundMem(_soundSE[4], DX_PLAYTYPE_BACK, true);
		_escapeFlg = true;	 // 戦闘から逃走する
		tmpUseItem = true;
	}

	if (_itemAction == ITEM::POW_UP)
	{
		PlaySoundMem(_soundSE[3], DX_PLAYTYPE_BACK, true);
		_powUpNum = 5;		 // 一時的に攻撃力up
		tmpUseItem = true;
	}

	// アイテム使用時の処理
	if (tmpUseItem)
	{
		_itemAction = ITEM::NON;
		if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
		{
			BattleLambda();
		}
	}
}

void Menu::Draw(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item, const std::shared_ptr<Monster>& monster)
{
	if (monster->GetEnemyState() != ENEMY_STATE::EXIST)
	{
		DrawGraph(0, 0, _menuImages["menuButton"], true);
	}
	else
	{
		// アイテムのみ使用可能
		DrawGraph(0, 0, _menuSelPNG[1], true);
	}

	// メニュー画面の表示
	if (_menuBackPngFlg)
	{
		DrawGraph(200, 0, _menuImages["menuWindow"], true);
	}

	// メニュー項目
	if (_menuSelPngFlg)
	{
		for (int i = 0; i <= 2; i++)
		{
			DrawGraph(menu_pair[i].first.x, menu_pair[i].first.y, _menuSelPNG[i], true);
		}

		DrawGraph(menu_pair[3].first.x, menu_pair[3].first.y, _menuImages["titleBack"], true);	
		DrawGraph(menu_pair[4].first.x, menu_pair[4].first.y, _menuImages["save"], true);
	}

	// アイテム
	if (_menu == MENU::ITEM)
	{
		// 枠と入手アイテムの描画
		for (int i = 0; i <= 11; i++)
		{
			DrawGraph(itemBox[i].pos.x, itemBox[i].pos.y, _menuImages["itembox"], true);
			// 入手しているアイテムの描画(していなかったら何も描画されないようにする)
			DrawGraph(itemBox[i].pos.x, itemBox[i].pos.y, itemBox[i].png, true);
		}

		DrawGraph(_choicePos.x, _choicePos.y, _menuImages["itemChoice"], true);

		// 装備されたらそこにEの文字を出す
		DrawFormatString(_equipSwordPos.x + 10, _equipSwordPos.y + 10, GetColor(255, 0, 0), "E");
		DrawFormatString(_equipShieldPos.x + 10, _equipShieldPos.y + 10, GetColor(255, 0, 0), "E");
	}

	// ステータス画面
	if (_menu == MENU::STATUS)
	{
		int x = 350;
		int y = 100;
		int offset = 30;
		DrawFormatString(x, y + offset * 0, 0x000000, "レベル:%d"                  , player->GetNowLevel());
		DrawFormatString(x, y + offset * 1, 0x000000, "体力:%d / %d"               , player->GetHP(), player->GetMaxHP());
		DrawFormatString(x, y + offset * 2, 0x000000, "攻撃力:%d(+ %d) = %d"       , player->GetAttackDamage(), _equipDamage, player->GetAttackDamage() + _equipDamage);
		DrawFormatString(x, y + offset * 3, 0x000000, "防御力:%d(+ %d) = %d"       , player->GetDifense(), _equipGuard, player->GetDifense() + _equipGuard);
		DrawFormatString(x, y + offset * 4, 0x000000, "次のレベルまで:残り%d"      , player->GetNextLevel());
		DrawFormatString(x, y + offset * 5, 0x000000, "所持金:%d円"                , player->GetMoney());
		DrawFormatString(x, y + offset * 6, 0x000000, "スキルチャージ完了まで:%d回", player->GetSkillCharge());

		// デバッグモード用に攻撃力を高く設定できるようにしておく
		if (CheckHitKey(KEY_INPUT_F10) == 1)
		{
			player->SetAttackDamage(999);
		}
	}

	// メニューのステータス画面とアイテム画面の描画
	if (_menu == MENU::ITEM || _menu == MENU::STATUS)
	{
		// 戻る
		DrawGraph(375, 470, _menuImages["back"], true);

		if (!_useOrThrowAway)
		{
			return;
		}

		// 説明の後ろ画像
		DrawGraph(0, 320, _menuImages["description"], true);

		// アイテムを使うことに効果があるとき
		if (!_nonNeedFlg)
		{
			// 使う
			DrawGraph(50, 400, _menuImages["use"], true);
		}
		else
		{
			// アイテムを使うことに効果がないとき
			// 描画ブレンドモード変更
			SetDrawBlendMode(DX_BLENDMODE_ADD, 100);
			// 使う
			DrawGraph(50, 400, _menuImages["use"], true);

			// 描画ブレンドモードをノーブレンドにする
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			DrawFormatString(20, 300, 0xffffff, "今使っても効果がない");
		}

		// 捨てる
		DrawGraph(50, 500, _menuImages["throw_away"], true);
		// メニューのアイテムで選択したアイテムの説明を出す
		if (_itemExplanation != ITEM::NON)
		{
			DrawFormatString(25, 340, 0x000000, "%s\n", item->GetExplanation(static_cast<int>(_itemExplanation) - 1));
		}
	}
}

void Menu::MenuButton_NonEnemy(const std::shared_ptr<MouseCtl>& mouse)
{
	// クリックしていないときはreturnする
	if (!mouse->GetClickTrg())
	{
		return;
	}

	// 文字表示中に上から項目が表示されないようにする
	if (!_menuBackPngFlg)
	{
		if (mouse->GetPos().x >= 0 && mouse->GetPos().x <= 0 + 150 &&
			mouse->GetPos().y >= 0 && mouse->GetPos().y <= 0 + 75)
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
			if (mouse->GetPos().x >= menu_pair[i].first.x && mouse->GetPos().x <= menu_pair[i].first.x + buttonSize[i].x &&
				mouse->GetPos().y >= menu_pair[i].first.y && mouse->GetPos().y <= menu_pair[i].first.y + buttonSize[i].y)
			{
				// クリック音
				PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);

				_menu = menu_pair[i].second;
				_menuSelPngFlg = false;	// 文字消す
			}
		}
	}

	GameContinue(mouse);
}

void Menu::MenuButton_Enemy(const std::shared_ptr<MouseCtl>& mouse)
{
	// クリックしていないときはreturnする
	if (!mouse->GetClickTrg())
	{
		return;
	}

	if (mouse->GetPos().x >= 0 && mouse->GetPos().x <= 0 + 200 &&
		mouse->GetPos().y >= 0 && mouse->GetPos().y <= 0 + 100)
	{
		// クリック音
		PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);

		_menu = MENU::ITEM;
		_menuBackPngFlg = true;
	}

	GameContinue(mouse);
}

void Menu::SetItem(const ITEM& item, const int& png)
{
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
}

bool Menu::GetMenuFlg(void)const
{
	return _menuSelPngFlg;
}

MENU Menu::GetMenu(void)const
{
	return _menu;
}

int Menu::GetCanHaveItem(void)const
{
	int canHave = 0;
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

int Menu::GetEquipDamage(void)const
{
	return _equipDamage;
}

int Menu::GetEquipGuard(void)const
{
	return _equipGuard;
}

int Menu::GetPowUp(void)const
{
	return _powUpNum;
}

void Menu::SetPowUp(const int& num)
{
	// 攻撃するごとに-1されていく
	_powUpNum = num;
}

bool Menu::GetNonDamageFlg(void)const
{
	return _nonDamageFlg;
}

void Menu::SetNonDamageFlg(const bool& flag)
{
	_nonDamageFlg = flag;
}

bool Menu::GetMenuBackPngFlg(void)const
{
	return _menuBackPngFlg;
}

bool Menu::GetEscapeFlg(void)const
{
	return _escapeFlg;
}

void Menu::SetEscapeFlg(const bool& flag)
{
	_escapeFlg = flag;
}

bool Menu::GetAppraisalFlg(void)const
{
	return _AppraisalFlg;
}

void Menu::SetAppraisalFlg(const bool& flag)
{
	_AppraisalFlg = flag;
}

void Menu::Save(const std::shared_ptr<Player>& player)
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
		if (file == NULL)
		{
			return; // エラー
		}
		fprintf(file, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n", player->GetNowLevel(), player->GetMaxHP(), player->GetHP(), player->GetAttackDamage(), player->GetDifense(), player->GetNextLevel(), player->GetMoney(), player->GetConditionTurn(), player->GetCondition());
		fprintf(file, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", itemBox[0]._item, itemBox[1]._item, itemBox[2]._item, itemBox[3]._item, itemBox[4]._item, itemBox[5]._item, itemBox[6]._item, itemBox[7]._item, itemBox[8]._item, itemBox[9]._item, itemBox[10]._item, itemBox[11]._item);
		fclose(file);
	}
}

void Menu::Load()
{
	if (MessageBox(			// メッセージ
		NULL,
		"ロードしますか?",
		"確認ダイアログ",
		MB_OKCANCEL
	) == IDOK)
	{
		// ロードをする
		// ファイルを読み込む
		int FileHandle;
		FileHandle = FileRead_open("data/save1.csv");
		if (FileHandle == NULL)
		{
			return;	// エラー
		}

		FileRead_scanf(FileHandle, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n", &Player::saveData[0], &Player::saveData[1], &Player::saveData[2], &Player::saveData[3], &Player::saveData[4], &Player::saveData[5], &Player::saveData[6], &Player::saveData[7], &Player::saveData[8]);
		FileRead_scanf(FileHandle, "%d,%d,%d,%d,%d,%d,%d,%d,%d", &itemBox[0]._item, &itemBox[1]._item, &itemBox[2]._item, &itemBox[3]._item, &itemBox[4]._item, &itemBox[5]._item, &itemBox[6]._item, &itemBox[7]._item, &itemBox[8]._item, &itemBox[9]._item, &itemBox[10]._item, &itemBox[11]._item);

		//ファイルを閉じる
		FileRead_close(FileHandle);

		Player::loadFlg = true;
		_loadFlg = true;
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