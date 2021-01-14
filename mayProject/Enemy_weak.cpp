#include <string>
#include "Dxlib.h"
#include "GameScene.h"
#include "SelectScene.h"
#include "Enemy_weak.h"

#define PI 3.141592653589793

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
	int FileHandle;
	if (SelectScene::modeSelect == MODE::NORMAL)
	{
		//�t�@�C����ǂݍ���
		FileHandle = FileRead_open("csv/enemyData_NORMAL.csv");
		if (FileHandle == NULL)
		{
			return false; //�G���[���̏���
		}
	}
	else if (SelectScene::modeSelect == MODE::HARD)
	{
		//�t�@�C����ǂݍ���
		FileHandle = FileRead_open("csv/enemyData_HARD.csv");
		if (FileHandle == NULL)
		{
			return false; //�G���[���̏���
		}
	}

	for (int i = 0; i < MONSTER_CNT; i++)
	{
		FileRead_scanf(FileHandle, "%d,%d,%d,%d,%d", &enemy_status[i].turn, &enemy_status[i].HP, &enemy_status[i].attack, &enemy_status[i].keikenti, &enemy_status[i].money);
	}

	//�t�@�C�������
	FileRead_close(FileHandle);

	pngInit();
	_enemyState = ENEMY_STATE::NON;
	_bossFogCnt = 0;
	_dropItem = false;
	_dropItemNum = -1;
	_animCnt = 0;
	_animUpDateSpeedCnt = 0;
	_se = LoadSoundMem("sound/se/drop.mp3");
	return true;
}

void Enemy_weak::pngInit(void)
{
	if (SelectScene::modeSelect == MODE::NORMAL)
	{
		// �G�̕����ǂݍ���(NORMAL���I�����ꂽ��)
		std::string monster = "image/monster/mons.png";
		LoadDivGraph(monster.c_str(), 5, 5, 1, 400, 400, _enemyPNG);
	}
	else if (SelectScene::modeSelect == MODE::HARD)
	{
		// �G�̕����ǂݍ���(HARD���I�����ꂽ��)
		std::string monster = "image/monster/mons2.png";
		LoadDivGraph(monster.c_str(), 5, 5, 1, 400, 400, _enemyPNG);
	}

	// �h���b�v�A�C�e�������ǂݍ���
	std::string item = "image/item_monster.png";
	LoadDivGraph(item.c_str(), 4, 2, 2, 193, 198, _eneItemPNG);

	// �{�X�̓ǂݍ���
	std::string boss = "image/monster/Boss.png";
	_bossPNG = LoadGraph(boss.c_str());

	// �{�X�p�X���[�N�摜
	std::string fog = "image/fog.png";
	_fogPNG = LoadGraph(fog.c_str());

	// �U���p�G�t�F�N�g�摜
	std::string eneAttack = "image/eneAttack.png";
	LoadDivGraph(eneAttack.c_str(), 8, 5, 2, 240, 240, _AttackEffectPNG);
}

void Enemy_weak::update(void)
{
	if (_bossFogCnt > 0)
	{
		_bossFogCnt--;
	}
}

void Enemy_weak::Draw(void)
{
	// ClsDrawScreen();
	// �G�����݂���Ԃ����`�悷��悤�ɐݒ�
	if (_enemyState == ENEMY_STATE::EXIST)
	{
		// �Ō�̈�����true����Ȃ������瓧�߂���Ȃ�
		DrawRotaGraph(450, 300, 1.0f, 0, _enemyPNG[_enemyNum], true);
	}

	// �G�����S�����Ƃ��̃h���b�v�A�C�e���̕`�揈��
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
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _bossFogCnt);
	DrawGraph(0, 0, _fogPNG, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Enemy_weak::EffectDraw(void)
{
	if (_animCnt < 8)
	{
		DrawRotaGraph(450,300,5.0f,PI/5, _AttackEffectPNG[_animCnt], true);
		_animUpDateSpeedCnt++;
		if (_animUpDateSpeedCnt % 3 == 0)
		{	// �A�j���[�V�������x�̒���
			_animCnt++;
		}
	}
	else
	{
		_animUpDateSpeedCnt = 0;
		return;
	}
}

void Enemy_weak::Damage(int damageNum, Cards* cards)
{
	_enemyHP -= damageNum;
	if (_enemyHP <= 0 && _enemyState == ENEMY_STATE::EXIST)
	{
		// �{�X���j���̓t���O��true�ɂ��ē������W�ɗ��Ă��{�X���o�Ă��Ȃ��悤�ɂ���
		if (_enemyNum == 5)
		{
			GameScene::bossClearFlg = true;
		}

		GameScene::monsterFlg = false;
		_enemyHP = 0;
		_enemyState = ENEMY_STATE::DEATH;
		cards->SetTurn(3);
		// �m���Ńt���O�𗧂ĂāAtrue��������A�C�e�����v���C���[�ɓn��
		int randNum = GetRand(2);	// 0~2
		_dropItemNum = GetRand(3); // 0 1 2 3
		int ItemNum = _dropItemNum + 13;  // 13 14 15 16�ɕϊ�

		if (randNum == 0)
		{
			PlaySoundMem(_se, DX_PLAYTYPE_BACK, true);
			// �A�C�e���h���b�v����
			_dropItem = true;
			_dropItemSyurui = static_cast<ITEM>(ItemNum);
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
	// ����G�C�x���g
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
		{	// �{�X�̂Ƃ��ɂ̓X���[�N�̏���������
			_bossFogCnt = 256;
		}
		// �_���[�W�ʂƑ̗͂��v���C���[���x���Œ����ł���悤�ɂ���
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

int Enemy_weak::GetNowHP(void)
{
	return _enemyHP;
}

int Enemy_weak::GetMaxHP(void)
{
	return _enemyMaxHP;
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

void Enemy_weak::SetAnimCnt(int num)
{
	_animCnt = num;
}
