#include <string>
#include <math.h>
#include "Dxlib.h"
#include "Enemy_weak.h"
#include "Menu.h"
#include "GameScene.h"
#include "Player.h"

// static変数の実体<型>クラス名::変数名 = 初期化;
bool Player::loadFlg = false;

int Player::saveTestNum[9] = { 0,0,0,0,0,0,0,0,0 };

// スキルチャージ時間の最大値
#define SKILL_CHARGE 10

struct player
{
	int now_level;			// 今のレベル
	int maxHP;				// 最大HP
	int plHP;				// 現在の体力
	int attackDamage;		// 攻撃力
	int defense;			// 防御力 drinkイベント成功時しか上昇しない
	int next_level;			// 次までのレベル
	int money;				// 所持金(敵を倒すと手に入り、物を購入すると減る)
	int conditionTurnNum;	// 状態異常の時間
	CONDITION condition;	// 健康状態
} player_status/*[40]*/;	// 最大レベルは40かな

Player::Player()
{
	Init();
}

Player::~Player()
{
	DeleteSoundMem(_seLevelUp);
}

void Player::Init(void)
{
	//ファイルを読み込む
//auto FileHandle = FileRead_open("csv/playerData.csv");
//if (FileHandle == NULL)
//{
//	return; //エラー時の処理
//}
//
//for (int i = 0; i <= 1; i++)
//{
//	FileRead_scanf(FileHandle, "%d,%d,%d,%d", &player_status[i].now_level, &player_status[i].maxHP, &player_status[i].attackDamage, &player_status[i].next_level);
//}
//
////ファイルを閉じる
//FileRead_close(FileHandle);
//
//_nowNum = 0;

	if (!loadFlg)
	{
		player_status.now_level = 1;
		player_status.maxHP = 35;
		player_status.plHP = player_status.maxHP;
		//player_status.attackDamage = 3;
		player_status.attackDamage = 999;
		player_status.defense = 0;
		player_status.next_level = 10;
		player_status.money = 2000;
		player_status.conditionTurnNum = 0;
		player_status.condition = CONDITION::FINE;
		//_plHP = player_status[_nowNum].maxHP;
		//_plHP = 10;
	}
	else
	{
		player_status.now_level = saveTestNum[0];
		player_status.maxHP = saveTestNum[1];
		player_status.plHP = saveTestNum[2];
		player_status.attackDamage = saveTestNum[3];
		player_status.defense = saveTestNum[4];
		player_status.next_level = saveTestNum[5];
		player_status.money = saveTestNum[6];
		player_status.conditionTurnNum = saveTestNum[7];
		player_status.condition = static_cast<CONDITION>(saveTestNum[8]);
	}

	_skillCharge = SKILL_CHARGE;
	_skillFlg = false;
	_skillBackFlg = false;

	_pngLight = 50;
	_lightFlg = false;

	_barrierMaxNum = 0;
	_barrierNum = 0;

	_seLevelUp = LoadSoundMem("sound/se/levelup.mp3");

	pngInit();
}

void Player::pngInit(void)
{
	// スキルアイコン
	std::string skillicon = "image/skillicon.png";
	_skillIconPNG = LoadGraph(skillicon.c_str());

	// スキル使えるというアナウンス
	std::string chargeAnnounce = "image/chargeAnnounce.png";
	_skillAnnouncePNG = LoadGraph(chargeAnnounce.c_str());

	// スキル背景
	std::string skillBack = "image/skillBack.png";
	_skillBackPNG = LoadGraph(skillBack.c_str());

	// 攻撃系スキルアイコン
	std::string skill_attack = "image/skill_attack.png";
	_skillAttackIconPNG = LoadGraph(skill_attack.c_str());

	// 防御系スキルアイコン
	std::string skill_barrier = "image/skill_barrier.png";
	_skillBarrierIconPNG = LoadGraph(skill_barrier.c_str());

	// 回復系スキルアイコン
	std::string skill_heal = "image/skill_heal.png";
	_skillHealIconPNG = LoadGraph(skill_heal.c_str());

	// やめるの文字画像
	std::string cancel = "image/cancel.png";
	_skillCancelPNG = LoadGraph(cancel.c_str());

	// 力こぶのアイコン画像
	std::string muscle = "image/muscle.png";
	_skillMuscleIconPNG = LoadGraph(muscle.c_str());

	// バリアバーの背景画像
	std::string barrier_back = "image/barrier_back.png";
	_barrierBarBackPNG = LoadGraph(barrier_back.c_str());

	// バリアバー画像
	std::string barrier_bar = "image/barrier_bar.png";
	_barrierBarPNG = LoadGraph(barrier_bar.c_str());
}

void Player::Draw(Menu* menu)
{
	if (_skillCharge != 0)
	{
		// スキルチャージ時間
		DrawFormatString(750, 530, 0xffffff, "スキルまで:%d", _skillCharge);
	}
	else
	{
		// 描画ブレンドモードを加算合成にする
		SetDrawBlendMode(DX_BLENDMODE_ADD, _pngLight);
		// 782,564
		DrawRotaGraph(750 + 32, 530 + 32, 1.0f, 0, _skillIconPNG, true);
		// 描画ブレンドモードをノーブレンドにする
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		DrawRotaGraph(750 + 32, 530 + 32, 1.0f, 0, _skillMuscleIconPNG, true);
		DrawGraph(820, 530, _skillAnnouncePNG, true);
	}

	// スキルアイコンを押されたら背景を描画する
	if (_skillBackFlg)
	{
		DrawGraph(250, 90, _skillBackPNG, true);
		DrawGraph(290, 150, _skillAttackIconPNG, true);
		DrawGraph(410, 150, _skillBarrierIconPNG, true);
		DrawGraph(530, 150, _skillHealIconPNG, true);
		DrawFormatString(280,250, 0x000000, "攻撃スキル:\n%dダメージ", player_status.attackDamage * 10 + menu->GetEquipDamage());
		DrawFormatString(410,250, 0x000000, "防御スキル:\n耐久%dの\nバリア展開", 20 + 3 * player_status.now_level);
		DrawFormatString(530, 250, 0x000000, "回復スキル:\n体力を全回復");
		DrawGraph(385, 320, _skillCancelPNG, true);
	}

	// バリアバーの表示
	if (_barrierNum > 0)
	{
		DrawFormatString(600, 325, 0xffffff, "バリア耐久:%d/%d", _barrierNum, _barrierMaxNum);

		int pgx = 150;
		int pgy = 25;
		DrawExtendGraph(600, 350, 600 + pgx, 350 + pgy, _barrierBarBackPNG, true);
		DrawExtendGraph(600, 350, 600 + pgx * ((float)_barrierNum / (float)_barrierMaxNum), 350 + pgy, _barrierBarPNG, true);
	}
}

void Player::ClickUpDate(Monster* monster, Menu* menu, GameScene* game)
{
	int x = 0;
	int y = 0;
	auto Mouse = GetMouseInput();                //マウスの入力状態取得
	// スキル使用可能時のマウスクリック位置とアイコン(円)との当たり判定
	// アイテム画面中はスキルチャージアイコンを押せない
	if (!menu->GetMenuBackPngFlg())
	{
		if (_skillFlg)
		{
			GetMousePoint(&x, &y);					     //マウスの座標取得

			// 782,564(アイコン描画位置)
			float a = x - 782;
			float b = y - 564;
			float c = sqrt(a * a + b * b);

			// 当たり判定(当たっているとき)
			if (c <= 34)
			{
				// スキルはターン消費なしで行える動作
				_skillBackFlg = true;
				// 攻撃系(基礎攻撃力*10+武器威力で一定のダメージを与えられる)
				//monster->Damage(player_status.attackDamage * 10 + menu->GetEquipDamage());
				//game->blinkFlg = true;
				// フラグと回数を元に戻す
				//_skillFlg = false;
				//_skillCharge = SKILL_CHARGE;
			}
		}
	}

	if (_skillBackFlg)
	{
		// やめるボタンとの当たり判定
		if (x >= 385 && x <= 385 + 150 && y >= 320 && y <= 320 + 65)
		{
			_skillBackFlg = false;
		}

		auto lambda = [&]() {
			// フラグと回数を元に戻す
			_skillFlg = false;
			_skillBackFlg = false;
			_skillCharge = SKILL_CHARGE;
		};

		// 攻撃アイコンとの当たり判定
		if (x >= 290 && x <= 290 + 100 && y >= 150 && y <= 150 + 100)
		{
			// 攻撃系(基礎攻撃力*10+武器威力で一定のダメージを与えられる)
			monster->Damage(player_status.attackDamage * 10 + menu->GetEquipDamage());
			game->blinkFlg = true;
			lambda();
			//// フラグと回数を元に戻す
			//_skillFlg = false;
			//_skillBackFlg = false;
			//_skillCharge = SKILL_CHARGE;
		}

		// 防御アイコンとの当たり判定
		if (x >= 410 && x <= 410 + 100 && y >= 150 && y <= 150 + 100)
		{
			_barrierMaxNum = 20 + 3 * player_status.now_level;
			_barrierNum = 20 + 3 * player_status.now_level;
			// 防御系(特定値*プレイヤーレベル)
			lambda();
			//// フラグと回数を元に戻す
			//_skillFlg = false;
			//_skillBackFlg = false;
			//_skillCharge = SKILL_CHARGE;
		}

		// 回復アイコンとの当たり判定
		if (x >= 530 && x <= 530 + 100 && y >= 150 && y <= 150 + 100)
		{
			player_status.plHP = player_status.maxHP;
			// 回復系(全回復もしかしたらリジェネ風に変更するかも)
			lambda();
			//// フラグと回数を元に戻す
			//_skillFlg = false;
			//_skillBackFlg = false;
			//_skillCharge = SKILL_CHARGE;
		}
	}
}

void Player::UpDate(void)
{
	// スキルが使用可能な時にフラグを立てて、当たり判定を行う
	if (_skillCharge <= 0)
	{
		_skillFlg = true;

		// 明るくしたり暗くする処理
		if (!_lightFlg)
		{
			if (_pngLight <= 255)
			{
				_pngLight += 5;
				if (_pngLight == 255)
				{
					_lightFlg = true;
				}
			}
		}

		if (_lightFlg)
		{
			_pngLight -= 5;
			if (_pngLight <= 50)
			{
				_lightFlg = false;
			}
		}
	}
}

void Player::SetHP(int hpNum)
{
	player_status.plHP = hpNum;
	// 最大HPを超えないように気を付ける
	//if (_plHP >= player_status[_nowNum].maxHP)
	//{
	//	_plHP = player_status[_nowNum].maxHP;
	//}
	if (player_status.plHP >= player_status.maxHP)
	{
		player_status.plHP = player_status.maxHP;
	}

	// 0を下回らないようにする
	if (player_status.plHP <= 0)
	{
		player_status.plHP = 0;
	}
}

int Player::GetHP(void)
{
	return player_status.plHP;
}

void Player::SetMaxHP(int hpNum)
{
	player_status.maxHP = hpNum;
}

int Player::GetMaxHP(void)
{
	//return player_status[_nowNum].maxHP;
	return player_status.maxHP;
}

float Player::GetHPBar(void)
{
	//return (float)_plHP / (float)player_status[_nowNum].maxHP;
	return (float)player_status.plHP / (float)player_status.maxHP;
}

int Player::GetAttackDamage(void)
{
	//return player_status[_nowNum].attackDamage;
	return player_status.attackDamage;
}

void Player::SetDifense(int num)
{
	player_status.defense = num;
}

int Player::GetDifense(void)
{
	return player_status.defense;
}

void Player::SetNextLevel(int num)
{
	//player_status[_nowNum].next_level = num;
	//if (player_status[_nowNum].next_level <= 0)
	//{
	//	// ステータスアップ
	//	_nowNum++;
	//}
	player_status.next_level = num;
	bool seFlg = false;

	// レベルが連続で上がる処理としてwhileが必要
	while (player_status.next_level <= 0)
	{
		if (!seFlg)
		{
			PlaySoundMem(_seLevelUp, DX_PLAYTYPE_BACK, true);
			seFlg = true;
		}
		// ステータスアップ
		//_nowNum++;
		player_status.attackDamage += 1;
		player_status.maxHP += 3;
		player_status.now_level++;

		// 次のレベルまでの経験値は少しずつ高くしていきたい
		player_status.next_level += 10 * player_status.now_level;

	}

	//if (player_status.next_level <= 0)
	//{
	//	// ステータスアップ
	//	//_nowNum++;
	//	player_status.attackDamage += 2;
	//	player_status.maxHP += 10;
	//	player_status.now_level++;
	//
	//	// 次のレベルまでの経験値は少しずつ高くしていきたい
	//	player_status.next_level += 10 * player_status.now_level;
	//}
}

int Player::GetNextLevel(void)
{
	//return player_status[_nowNum].next_level;
	return player_status.next_level;
}

int Player::GetNowLevel(void)
{
	//return player_status[_nowNum].now_level;
	return player_status.now_level;
}

void Player::SetMoney(int num)
{
	player_status.money = num;
}

int Player::GetMoney(void)
{
	return player_status.money;
}

void Player::SetCondition(CONDITION con)
{
	player_status.condition = con;
	if (player_status.condition == CONDITION::POISON)
	{
		player_status.conditionTurnNum = 5;
	}
}

CONDITION Player::GetCondition(void)
{
	return player_status.condition;
}

void Player::SetConditionTurn(int num)
{
	player_status.conditionTurnNum = num;
}

int Player::GetConditionTurn(void)
{
	return player_status.conditionTurnNum;
}

void Player::SetSkillCharge(int num)
{
	// スキルチャージが0より大きいときは減らしていく
	_skillCharge = num;
}

int Player::GetSkillCharge(void)
{
	return _skillCharge;
}

bool Player::GetSkillBackFlg(void)
{
	return _skillBackFlg;
}

void Player::SetBarrierNum(int num)
{
	_barrierNum = num;
}

int Player::GetBarrierNum(void)
{
	return _barrierNum;
}

void Player::SaveTest(void)
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
		fopen_s(&file, "data/saveTest.csv", "wb");
		fprintf(file, "%d,%d,%d,%d,%d,%d,%d,%d,%d", player_status.now_level, player_status.maxHP, player_status.plHP, player_status.attackDamage, player_status.defense, player_status.next_level, player_status.money, player_status.conditionTurnNum, player_status.condition);
		fclose(file);
	}
}

void Player::LoadTest(void)
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
		FileHandle = FileRead_open("data/saveTest.csv");
		if (FileHandle == NULL)
		{
			return;
		}

		FileRead_scanf(FileHandle, "%d,%d,%d,%d,%d,%d,%d,%d,%d", &player_status.now_level, &player_status.maxHP, &player_status.plHP, &player_status.attackDamage, &player_status.defense, &player_status.next_level, &player_status.money, &player_status.conditionTurnNum, &player_status.condition);

		//ファイルを閉じる
		FileRead_close(FileHandle);

		loadFlg = true;
	}
}