#include <string>
#include <math.h>
#include "Dxlib.h"
#include "Enemy_weak.h"
#include "Menu.h"
#include "GameScene.h"
#include "Player.h"

#define SKILL_CHARGE 3

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

	//player_status.now_level = 1;
	player_status.maxHP = 35;
	player_status.plHP = player_status.maxHP;
	player_status.attackDamage = 3;
	//player_status.attackDamage = 999;
	player_status.defense = 0;
	player_status.next_level = 10;
	player_status.money = 1000;
	player_status.conditionTurnNum = 0;
	player_status.condition = CONDITION::FINE;
	//_plHP = player_status[_nowNum].maxHP;
	//_plHP = 10;

	_skillCharge = SKILL_CHARGE;
	_skillFlg = false;
	_skillBackFlg = false;

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

	// やめるの文字画像
	std::string cancel = "image/cancel.png";
	_skillCancelPNG = LoadGraph(cancel.c_str());
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
		// 782,564
		DrawRotaGraph(750 + 32, 530 + 32, 1.0f, 0, _skillIconPNG, true);
		DrawGraph(820, 530, _skillAnnouncePNG, true);
	}

	// スキルアイコンを押されたら背景を描画する
	if (_skillBackFlg)
	{
		DrawGraph(250, 90, _skillBackPNG, true);
		DrawGraph(300, 150, _skillAttackIconPNG, true);
		DrawFormatString(300,250, 0x000000, "攻撃スキル:\n%dダメージ", player_status.attackDamage * 10 + menu->GetEquipDamage());
		DrawGraph(375, 300, _skillCancelPNG, true);
	}
}

void Player::ClickUpDate(Monster* monster, Menu* menu, GameScene* game)
{
	int x = 0;
	int y = 0;
	auto Mouse = GetMouseInput();                //マウスの入力状態取得
	// スキル使用可能時のマウスクリック位置とアイコン(円)との当たり判定
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

	if (_skillBackFlg)
	{
		// やめるボタンとの当たり判定
		if (x >= 375 && x <= 375 + 150 && y >= 300 && y <= 300 + 65)
		{
			_skillBackFlg = false;
		}

		// 攻撃アイコンとの当たり判定
		if (x >= 300 && x <= 300 + 100 && y >= 150 && y <= 150 + 100)
		{
			// 攻撃系(基礎攻撃力*10+武器威力で一定のダメージを与えられる)
			monster->Damage(player_status.attackDamage * 10 + menu->GetEquipDamage());
			game->blinkFlg = true;
			// フラグと回数を元に戻す
			_skillFlg = false;
			_skillBackFlg = false;
			_skillCharge = SKILL_CHARGE;
		}
	}
}

void Player::UpDate(void)
{
	// スキルが使用可能な時にフラグを立てて、当たり判定を行う
	if (_skillCharge <= 0)
	{
		_skillFlg = true;
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

	// レベルが連続で上がる処理としてwhileが必要
	while (player_status.next_level <= 0)
	{
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

void Player::SetSkillCharge(void)
{
	// スキルチャージが0より大きいときは減らしていく
	if (_skillCharge > 0)
	{
		_skillCharge--;
	}
}

int Player::GetSkillCharge(void)
{
	return _skillCharge;
}