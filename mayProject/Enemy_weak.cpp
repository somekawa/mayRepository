#include <string>
#include "Dxlib.h"
#include "GameScene.h"
#include "SelectScene.h"
#include "Enemy_weak.h"

struct enemy
{
	int turn;	 // 待機ターン数
	int HP;		 // 体力
	int attack;	 // 攻撃力
	int keikenti;// 経験値
	int money;	 // 倒した時に落とすお金
} enemy_status[MONSTER_CNT];


Enemy_weak::Enemy_weak()
{
	Init();
}

Enemy_weak::~Enemy_weak()
{
}

bool Enemy_weak::Init(void)
{
	int FileHandle;
	if (SelectScene::modeTest == MODE::NORMAL)
	{
		//ファイルを読み込む
		FileHandle = FileRead_open("csv/enemyData_NORMAL.csv");
		if (FileHandle == NULL)
		{
			return false; //エラー時の処理
		}
	}
	else if (SelectScene::modeTest == MODE::HARD)
	{
		//ファイルを読み込む
		FileHandle = FileRead_open("csv/enemyData_HARD.csv");
		if (FileHandle == NULL)
		{
			return false; //エラー時の処理
		}
	}

	for (int i = 0; i < MONSTER_CNT; i++)
	{
		FileRead_scanf(FileHandle, "%d,%d,%d,%d,%d", &enemy_status[i].turn, &enemy_status[i].HP, &enemy_status[i].attack, &enemy_status[i].keikenti, &enemy_status[i].money);
	}

	//ファイルを閉じる
	FileRead_close(FileHandle);

	pngInit();
	_enemyState = ENEMY_STATE::NON;
	_bossFrogCnt = 0;
	_dropItem = false;
	_dropItemNum = -1;
	_se = LoadSoundMem("sound/se/drop.mp3");
	return true;
}

void Enemy_weak::pngInit(void)
{
	if (SelectScene::modeTest == MODE::NORMAL)
	{
		// 敵の分割読み込み(NORMALが選択された時)
		std::string monster = "image/monster/mons.png";
		LoadDivGraph(monster.c_str(), 5, 5, 1, 400, 400, _enemyPNG);
	}
	else if (SelectScene::modeTest == MODE::HARD)
	{
		// 敵の分割読み込み(HARDが選択された時)
		std::string monster = "image/monster/mons2.png";
		LoadDivGraph(monster.c_str(), 5, 5, 1, 400, 400, _enemyPNG);
	}

	// ドロップアイテム分割読み込み
	std::string item = "image/item_monster.png";
	LoadDivGraph(item.c_str(), 4, 2, 2, 193, 198, _eneItemPNG);

	// ボスの読み込み
	std::string boss = "image/monster/Boss.png";
	_bossPNG = LoadGraph(boss.c_str());

	// ボス用スモーク画像
	std::string frog = "image/frog.png";
	frogPNG = LoadGraph(frog.c_str());
}

void Enemy_weak::update(void)
{
	if (_bossFrogCnt > 0)
	{
		_bossFrogCnt--;
	}
}

void Enemy_weak::Draw(void)
{
	// ClsDrawScreen();
	// 敵が存在する間だけ描画するように設定
	if (_enemyState == ENEMY_STATE::EXIST)
	{
		// 最後の引数がtrueじゃなかったら透過されない
		DrawRotaGraph(450, 270, 1.0f, 0, _enemyPNG[_enemyNum], true);
	}

	// 敵が死亡したときのドロップアイテムの描画処理
	if (_enemyState == ENEMY_STATE::DEATH)
	{
		if (_dropItem)
		{
			DrawRotaGraph(450, 270, 1.0f, 0, _eneItemPNG[_dropItemNum], true);
		}
	}
	// ScreenFlip();
}

void Enemy_weak::BossDraw(void)
{
	DrawRotaGraph(450, 270, 1.0f, 0, _bossPNG, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _bossFrogCnt);
	DrawGraph(0, 0, frogPNG, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Enemy_weak::Damage(int damageNum, Cards* cards)
{
	_enemyHP -= damageNum;
	if (_enemyHP <= 0 && _enemyState == ENEMY_STATE::EXIST)
	{
		GameScene::monsterFlg = false;
		_enemyHP = 0;
		_enemyState = ENEMY_STATE::DEATH;
		cards->SetTurn(3);
		// 確率でフラグを立てて、trueだったらアイテムをプレイヤーに渡す
		int randNum = GetRand(0);	// 0~2
		_dropItemNum = 3;//GetRand(3); // 0 1 2 3
		int dropItemNum = _dropItemNum + 13;  // 13 14 15 16に変換

		if (randNum == 0)
		{
			PlaySoundMem(_se, DX_PLAYTYPE_BACK, true);
			// アイテムドロップする
			_dropItem = true;
			_dropItemSyurui = static_cast<ITEM>(dropItemNum);
		}
		else
		{
			// アイテムドロップしない
			_dropItem = false;
		}
	}
}

ENEMY_STATE Enemy_weak::GetEnemyState(void)
{
	return _enemyState;
}

void Enemy_weak::SetEnemyState(ENEMY_STATE st)
{
	_enemyState = st;
}

int Enemy_weak::GetAttack(void)
{
	return _attackDamage;
}

int Enemy_weak::GetMaxTurn(void)
{
	return _enemyMaxTurn;
}

void Enemy_weak::SetEnemyNum(int num, int plLv)
{
	// 特定敵イベント
	if (num == 6)
	{
		_attackDamage = 999;
		_enemyHP = 500;
		_enemyMaxHP = 500;
		_enemyMaxTurn = 3;
		_enemyState = ENEMY_STATE::EXIST;
		_enemyNum = num;
	}
	else
	{
		if (num == 5)
		{	// ボスのときにはスモークの準備をする
			_bossFrogCnt = 256;
		}
		// ダメージ量と体力をプレイヤーレベルで調整できるようにする
		_attackDamage = enemy_status[num].attack + (plLv * 2);
		_enemyHP = enemy_status[num].HP + (plLv * 3);
		_enemyMaxHP = _enemyHP;
		_enemyMaxTurn = enemy_status[num].turn;
		_enemyNum = num;
		_enemyState = ENEMY_STATE::EXIST;
	}
}

float Enemy_weak::GetHPBar(void)
{
	return (float)_enemyHP / (float)_enemyMaxHP;
}

int Enemy_weak::GetKeikenti(void)
{
	return enemy_status[_enemyNum].keikenti;
}

int Enemy_weak::GetMoney(void)
{
	return enemy_status[_enemyNum].money;
}

bool Enemy_weak::GetDropFlg(void)
{
	return _dropItem;
}

void Enemy_weak::SetDropFlg(bool flag)
{
	_dropItem = flag;
}

ITEM Enemy_weak::GetDrop(void)
{
	return _dropItemSyurui;
}

int Enemy_weak::GetEnemyNum(void)
{
	return _enemyNum;
}
