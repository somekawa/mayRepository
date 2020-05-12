#include <string>
#include "Dxlib.h"
#include "Player.h"

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

	player_status.now_level = 1;
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
}

Player::~Player()
{
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