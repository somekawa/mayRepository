#include <string>
#include "Dxlib.h"
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
	//std::string imageのところ
	//std::string name
	//std::string .pngのところ
	// image + name + .pngみたいに組み合わせる?
	//
	//std::string a = "image/";
	//std::string b = "kinoko";
	//std::string c = ".png";
	//auto d = a + b + c;
	//_enemyPNG = LoadGraph(d.c_str());

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

	//ファイルを読み込む
	//auto FileHandle = FileRead_open("csv/enemyData.csv");
	//if (FileHandle == NULL)
	//{
	//	return -1; //エラー時の処理
	//}

	//FileRead_scanf
	//	はファイルの中身をscanfを使うような要領で読み込んでいきます。
	//	例えばファイルの中身が「1 2 3」だったら
	//	FileRead_scanf(FileHandle, "%d %d %d", &a, &b, &c);
	//とすることでaに１、bに2、cに3が入ります。

	for (int i = 0; i < MONSTER_CNT; i++)
	{
		FileRead_scanf(FileHandle, "%d,%d,%d,%d,%d",&enemy_status[i].turn, &enemy_status[i].HP, &enemy_status[i].attack, &enemy_status[i].keikenti, &enemy_status[i].money);
	}

	//ファイルを閉じる
	FileRead_close(FileHandle);

	if (SelectScene::modeTest == MODE::NORMAL)
	{
		// 敵の分割読み込み
		std::string monster = "image/monster/mons.png";
		LoadDivGraph(monster.c_str(), 5, 5, 1, 400, 400, _enemyPNG);
	}
	else if (SelectScene::modeTest == MODE::HARD)
	{
		// 敵の分割読み込み
		std::string monster = "image/monster/mons2.png";
		LoadDivGraph(monster.c_str(), 5, 5, 1, 400, 400, _enemyPNG);
	}

	//// 敵の分割読み込み
	//std::string monster = "image/monster/mons.png";
	//LoadDivGraph(monster.c_str(), 5, 5, 1, 400, 400, _enemyPNG);

	// 敵の魂アイテム分割読み込み
	std::string item = "image/item_monster.png";
	LoadDivGraph(item.c_str(), 4, 2, 2, 193, 198, _eneItemPNG);

	// ボスの読み込み
	std::string boss = "image/monster/Boss.png";
	_bossPNG = LoadGraph(boss.c_str());

	//std::string a = "image/monster";
	//std::string b = "monster_";
	//std::string c = ".png";
	//auto d = a + b + enemy_status[0].nameNum + c;
	//_enemyPNG = LoadGraph(d.c_str());
	//
	//_enemyHP = enemy_status[0].HP;
	//_enemyMaxHP = _enemyHP;
	//_enemyMaxTurn = enemy_status[0].turn;
	_state = ENEMY_STATE::NON;
	_dropItem = false;
	_dropNum = -1;
	return true;
}

void Enemy_weak::Draw(void)
{
	// ClsDrawScreen();
	// 画面描画(画像中央を中心にする為にRotaGraphを使用中)
	// 敵が存在する間だけ描画するように設定
	if (_state == ENEMY_STATE::EXIST)
	{
		// 最後の引数がtrueじゃなかったら透過されない
		DrawRotaGraph(450, 270, 1.0f, 0, _enemyPNG[_enemyNum], true);
	}

	// 敵が死亡したときのドロップアイテムの描画処理
	if (_state == ENEMY_STATE::DEATH)
	{
		if (_dropItem)
		{
			DrawRotaGraph(450, 270, 1.0f, 0, _eneItemPNG[_dropNum], true);
		}
	}
	// ScreenFlip();
}

void Enemy_weak::BossDraw(void)
{
	DrawRotaGraph(450, 270, 1.0f, 0, _bossPNG, true);
}

void Enemy_weak::Damage(int damageNum)
{
	_enemyHP -= damageNum;
	if (_enemyHP <= 0 && _state == ENEMY_STATE::EXIST)
	{
		_enemyHP = 0;
		_state = ENEMY_STATE::DEATH;

		// 確率でフラグを立てて、trueだったらアイテムをプレイヤーに渡す
		int randNum = GetRand(2);	// 0~2
		_dropNum = GetRand(1); // 0 1
		int dropItemNum = _dropNum + 11;  // 11 12に変換

		if (randNum == 0)
		{
			// アイテムドロップする
			_dropItem = true;
			//_drop = ITEM::ENEMY_2;
			_drop = static_cast<ITEM>(dropItemNum);
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
	return _state;
}

void Enemy_weak::SetEnemyState(ENEMY_STATE st)
{
	_state = st;
}

int Enemy_weak::GetAttack(void)
{
	//return enemy_status[_enemyNum].attack;
	return _attackDamage;
}

int Enemy_weak::GetMaxTurn(void)
{
	return _enemyMaxTurn;
}

void Enemy_weak::SetEnemyNum(int num, int plLv)
{
	//std::string a = "image/monster/";
	//std::string b = "monster_";
	//std::string c = ".png";
	//auto d = a + b + enemy_status[num].nameNum + c;
	//_enemyPNG = LoadGraph(d.c_str());

	// ダメージ量と体力をプレイヤーレベルで調整できるようにする
	_attackDamage = enemy_status[num].attack + (plLv * 2);
	_enemyHP = enemy_status[num].HP + (plLv * 3);
	_enemyMaxHP = _enemyHP;
	_enemyMaxTurn = enemy_status[num].turn;
	_enemyNum = num;
	_state = ENEMY_STATE::EXIST;
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
	return _drop;
}