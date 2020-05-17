#include <string>
#include "Dxlib.h"
#include "SelectScene.h"
#include "Enemy_weak.h"

struct enemy
{
	int turn;	 // �ҋ@�^�[����
	int HP;		 // �̗�
	int attack;	 // �U����
	int keikenti;// �o���l
	int money;	 // �|�������ɗ��Ƃ�����
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
	//std::string image�̂Ƃ���
	//std::string name
	//std::string .png�̂Ƃ���
	// image + name + .png�݂����ɑg�ݍ��킹��?
	//
	//std::string a = "image/";
	//std::string b = "kinoko";
	//std::string c = ".png";
	//auto d = a + b + c;
	//_enemyPNG = LoadGraph(d.c_str());

	int FileHandle;
	if (SelectScene::modeTest == MODE::NORMAL)
	{
		//�t�@�C����ǂݍ���
		FileHandle = FileRead_open("csv/enemyData_NORMAL.csv");
		if (FileHandle == NULL)
		{
			return false; //�G���[���̏���
		}
	}
	else if (SelectScene::modeTest == MODE::HARD)
	{
		//�t�@�C����ǂݍ���
		FileHandle = FileRead_open("csv/enemyData_HARD.csv");
		if (FileHandle == NULL)
		{
			return false; //�G���[���̏���
		}
	}

	//�t�@�C����ǂݍ���
	//auto FileHandle = FileRead_open("csv/enemyData.csv");
	//if (FileHandle == NULL)
	//{
	//	return -1; //�G���[���̏���
	//}

	//FileRead_scanf
	//	�̓t�@�C���̒��g��scanf���g���悤�ȗv�̂œǂݍ���ł����܂��B
	//	�Ⴆ�΃t�@�C���̒��g���u1 2 3�v��������
	//	FileRead_scanf(FileHandle, "%d %d %d", &a, &b, &c);
	//�Ƃ��邱�Ƃ�a�ɂP�Ab��2�Ac��3������܂��B

	for (int i = 0; i < MONSTER_CNT; i++)
	{
		FileRead_scanf(FileHandle, "%d,%d,%d,%d,%d",&enemy_status[i].turn, &enemy_status[i].HP, &enemy_status[i].attack, &enemy_status[i].keikenti, &enemy_status[i].money);
	}

	//�t�@�C�������
	FileRead_close(FileHandle);

	if (SelectScene::modeTest == MODE::NORMAL)
	{
		// �G�̕����ǂݍ���
		std::string monster = "image/monster/mons.png";
		LoadDivGraph(monster.c_str(), 5, 5, 1, 400, 400, _enemyPNG);
	}
	else if (SelectScene::modeTest == MODE::HARD)
	{
		// �G�̕����ǂݍ���
		std::string monster = "image/monster/mons2.png";
		LoadDivGraph(monster.c_str(), 5, 5, 1, 400, 400, _enemyPNG);
	}

	//// �G�̕����ǂݍ���
	//std::string monster = "image/monster/mons.png";
	//LoadDivGraph(monster.c_str(), 5, 5, 1, 400, 400, _enemyPNG);

	// �G�̍��A�C�e�������ǂݍ���
	std::string item = "image/item_monster.png";
	LoadDivGraph(item.c_str(), 4, 2, 2, 193, 198, _eneItemPNG);

	// �{�X�̓ǂݍ���
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
	// ��ʕ`��(�摜�����𒆐S�ɂ���ׂ�RotaGraph���g�p��)
	// �G�����݂���Ԃ����`�悷��悤�ɐݒ�
	if (_state == ENEMY_STATE::EXIST)
	{
		// �Ō�̈�����true����Ȃ������瓧�߂���Ȃ�
		DrawRotaGraph(450, 270, 1.0f, 0, _enemyPNG[_enemyNum], true);
	}

	// �G�����S�����Ƃ��̃h���b�v�A�C�e���̕`�揈��
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

		// �m���Ńt���O�𗧂ĂāAtrue��������A�C�e�����v���C���[�ɓn��
		int randNum = GetRand(2);	// 0~2
		_dropNum = GetRand(1); // 0 1
		int dropItemNum = _dropNum + 11;  // 11 12�ɕϊ�

		if (randNum == 0)
		{
			// �A�C�e���h���b�v����
			_dropItem = true;
			//_drop = ITEM::ENEMY_2;
			_drop = static_cast<ITEM>(dropItemNum);
		}
		else
		{
			// �A�C�e���h���b�v���Ȃ�
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

	// �_���[�W�ʂƑ̗͂��v���C���[���x���Œ����ł���悤�ɂ���
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