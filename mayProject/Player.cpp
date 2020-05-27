#include <string>
#include <math.h>
#include "Dxlib.h"
#include "Enemy_weak.h"
#include "Menu.h"
#include "GameScene.h"
#include "Player.h"

// static�ϐ��̎���<�^>�N���X��::�ϐ��� = ������;
bool Player::loadFlg = false;
int Player::saveData[9] = { 0,0,0,0,0,0,0,0,0 };

// �X�L���`���[�W���Ԃ̍ő�l
#define SKILL_CHARGE 10

struct player
{
	int now_level;			// ���̃��x��
	int maxHP;				// �ő�HP
	int plHP;				// ���݂̗̑�
	int attackDamage;		// �U����
	int defense;			// �h��� drink�C�x���g�����������㏸���Ȃ�
	int next_level;			// ���܂ł̃��x��
	int money;				// ������(�G��|���Ǝ�ɓ���A�����w������ƌ���)
	int conditionTurnNum;	// ��Ԉُ�̎���
	CONDITION condition;	// ���N���
} player_status;

Player::Player()
{
	Init();
}

Player::~Player()
{
	// ���֌W
	for (int i = 0; i < 3; i++)
	{
		DeleteSoundMem(_soundSE[i]);
	}
}

void Player::Init(void)
{
	//�t�@�C����ǂݍ���
//auto FileHandle = FileRead_open("csv/playerData.csv");
//if (FileHandle == NULL)
//{
//	return; //�G���[���̏���
//}
//
//for (int i = 0; i <= 1; i++)
//{
//	FileRead_scanf(FileHandle, "%d,%d,%d,%d", &player_status[i].now_level, &player_status[i].maxHP, &player_status[i].attackDamage, &player_status[i].next_level);
//}
//
////�t�@�C�������
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
		player_status.money = 1000;
		player_status.conditionTurnNum = 0;
		player_status.condition = CONDITION::FINE;
	}
	else
	{
		player_status.now_level = saveData[0];
		player_status.maxHP = saveData[1];
		player_status.plHP = saveData[2];
		player_status.attackDamage = saveData[3];
		player_status.defense = saveData[4];
		player_status.next_level = saveData[5];
		player_status.money = saveData[6];
		player_status.conditionTurnNum = saveData[7];
		player_status.condition = static_cast<CONDITION>(saveData[8]);
	}

	// �X�L���֌W
	_skillCharge = SKILL_CHARGE;
	_skillFlg = false;
	_skillBackFlg = false;
	_pngLight = 50;
	_lightFlg = false;
	_seSkillOnceFlg = false;

	// �o���A�֌W
	_barrierMaxNum = 0;
	_barrierNum = 0;

	_soundSE[0] = LoadSoundMem("sound/se/click.mp3");
	_soundSE[1] = LoadSoundMem("sound/se/levelup.mp3");
	_soundSE[2] = LoadSoundMem("sound/se/skill.mp3");

	pngInit();
}

void Player::pngInit(void)
{
	// �X�L���A�C�R��
	std::string skillicon = "image/skillicon.png";
	_skillIconPNG = LoadGraph(skillicon.c_str());

	// �X�L���g����Ƃ����A�i�E���X
	std::string chargeAnnounce = "image/chargeAnnounce.png";
	_skillAnnouncePNG = LoadGraph(chargeAnnounce.c_str());

	// �X�L���w�i
	std::string skillBack = "image/skillBack.png";
	_skillBackPNG = LoadGraph(skillBack.c_str());

	// �U���n�X�L���A�C�R��
	std::string skill_attack = "image/skill_attack.png";
	_skillAttackIconPNG = LoadGraph(skill_attack.c_str());

	// �h��n�X�L���A�C�R��
	std::string skill_barrier = "image/skill_barrier.png";
	_skillBarrierIconPNG = LoadGraph(skill_barrier.c_str());

	// �񕜌n�X�L���A�C�R��
	std::string skill_heal = "image/skill_heal.png";
	_skillHealIconPNG = LoadGraph(skill_heal.c_str());

	// ��߂�̕����摜
	std::string cancel = "image/cancel.png";
	_skillCancelPNG = LoadGraph(cancel.c_str());

	// �͂��Ԃ̃A�C�R���摜
	std::string muscle = "image/muscle.png";
	_skillMuscleIconPNG = LoadGraph(muscle.c_str());

	// �o���A�o�[�̔w�i�摜
	std::string barrier_back = "image/barrier_back.png";
	_barrierBarBackPNG = LoadGraph(barrier_back.c_str());

	// �o���A�o�[�摜
	std::string barrier_bar = "image/barrier_bar.png";
	_barrierBarPNG = LoadGraph(barrier_bar.c_str());
}

void Player::ClickUpDate(Monster* monster, Menu* menu, GameScene* game, Cards* cards)
{
	int x = 0;
	int y = 0;
	auto Mouse = GetMouseInput();     // �}�E�X�̓��͏�Ԏ擾
	// �X�L���g�p�\���̃}�E�X�N���b�N�ʒu�ƃA�C�R��(�~)�Ƃ̓����蔻��
	// �A�C�e����ʒ��̓X�L���`���[�W�A�C�R���������Ȃ�
	if (!menu->GetMenuBackPngFlg())
	{
		if (_skillFlg)
		{
			GetMousePoint(&x, &y);	 // �}�E�X�̍��W�擾

			// 782,564(�A�C�R���`��ʒu)
			float a = x - 782;
			float b = y - 564;
			float c = sqrt(a * a + b * b);

			// �����蔻��(�������Ă���Ƃ�)
			if (c <= 34)
			{
				PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
				// �X�L���̓^�[������Ȃ��ōs���铮��
				_skillBackFlg = true;
			}
		}
	}

	if (_skillBackFlg)
	{
		// ��߂�{�^���Ƃ̓����蔻��
		if (x >= 385 && x <= 385 + 150 && y >= 320 && y <= 320 + 65)
		{
			PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
			_skillBackFlg = false;
		}

		auto lambda = [&]() {
			PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
			// �t���O�Ɖ񐔂����ɖ߂�
			_seSkillOnceFlg = false;
			_skillFlg = false;
			_skillBackFlg = false;
			_skillCharge = SKILL_CHARGE;
		};

		// �U���A�C�R���Ƃ̓����蔻��
		if (x >= 290 && x <= 290 + 100 && y >= 150 && y <= 150 + 100)
		{
			// �U���n(��b�U����*10+����З͂ň��̃_���[�W��^������)
			monster->Damage(player_status.attackDamage * 10 + menu->GetEquipDamage(), cards);
			game->blinkFlg = true;
			lambda();
		}

		// �h��A�C�R���Ƃ̓����蔻��
		if (x >= 410 && x <= 410 + 100 && y >= 150 && y <= 150 + 100)
		{
			// �h��n(����l*�v���C���[���x��)
			_barrierMaxNum = 20 + 3 * player_status.now_level;
			_barrierNum = 20 + 3 * player_status.now_level;
			lambda();
		}

		// �񕜃A�C�R���Ƃ̓����蔻��
		if (x >= 530 && x <= 530 + 100 && y >= 150 && y <= 150 + 100)
		{
			// �񕜌n(�S�񕜂����������烊�W�F�l���ɕύX���邩��)
			player_status.plHP = player_status.maxHP;
			lambda();
		}
	}
}

void Player::UpDate(void)
{
	// �X�L�����g�p�\�Ȏ��Ƀt���O�𗧂ĂāA�����蔻����s��
	if (_skillCharge <= 0)
	{
		_skillFlg = true;
		if (!_seSkillOnceFlg)
		{
			PlaySoundMem(_soundSE[2], DX_PLAYTYPE_BACK, true);
			_seSkillOnceFlg = true;
		}

		// �A�C�R�����邳��������
		if (!_lightFlg)
		{
			if (_pngLight <= 255)
			{
				_pngLight += 5;
				if (_pngLight >= 255)
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

void Player::Draw(Menu* menu)
{
	if (_skillCharge != 0)
	{
		// �X�L���`���[�W����
		DrawFormatString(750, 530, 0xffffff, "�X�L���܂�:%d", _skillCharge);
	}
	else
	{
		// �`��u�����h���[�h�����Z�����ɂ���
		SetDrawBlendMode(DX_BLENDMODE_ADD, _pngLight);
		// 782,564
		DrawRotaGraph(750 + 32, 530 + 32, 1.0f, 0, _skillIconPNG, true);
		// �`��u�����h���[�h���m�[�u�����h�ɂ���
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		DrawRotaGraph(750 + 32, 530 + 32, 1.0f, 0, _skillMuscleIconPNG, true);
		DrawGraph(820, 530, _skillAnnouncePNG, true);
	}

	// �X�L���A�C�R���������ꂽ��w�i��`�悷��
	if (_skillBackFlg)
	{
		int iconIntervalX = 120;	// �A�C�R���̕`��Ԋu
		DrawGraph(250, 90, _skillBackPNG, true);
		DrawGraph(290, 150, _skillAttackIconPNG, true);
		DrawGraph(290 + iconIntervalX, 150, _skillBarrierIconPNG, true);
		DrawGraph(290 + iconIntervalX * 2, 150, _skillHealIconPNG, true);
		DrawFormatString(280,250, 0x000000, "�U���X�L��:\n%d�_���[�W", player_status.attackDamage * 10 + menu->GetEquipDamage());
		DrawFormatString(410,250, 0x000000, "�h��X�L��:\n�ϋv%d��\n�o���A�W�J", 20 + 3 * player_status.now_level);
		DrawFormatString(530, 250, 0x000000, "�񕜃X�L��:\n�̗͂�S��");
		DrawGraph(385, 320, _skillCancelPNG, true);
	}

	// �o���A�o�[�̕\��
	if (_barrierNum > 0)
	{
		DrawFormatString(600, 325, 0xffffff, "�o���A�ϋv:%d/%d", _barrierNum, _barrierMaxNum);

		int pgx = 150;
		int pgy = 25;
		DrawExtendGraph(600, 350, 600 + pgx, 350 + pgy, _barrierBarBackPNG, true);
		DrawExtendGraph(600, 350, 600 + pgx * ((float)_barrierNum / (float)_barrierMaxNum), 350 + pgy, _barrierBarPNG, true);
	}
}

void Player::SetHP(int hpNum)
{
	player_status.plHP = hpNum;
	// �ő�HP�𒴂��Ȃ��悤�ɋC��t����
	if (player_status.plHP >= player_status.maxHP)
	{
		player_status.plHP = player_status.maxHP;
	}

	// 0�������Ȃ��悤�ɂ���
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
	return player_status.maxHP;
}

float Player::GetHPBar(void)
{
	return (float)player_status.plHP / (float)player_status.maxHP;
}

int Player::GetAttackDamage(void)
{
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
	player_status.next_level = num;
	bool seFlg = false;

	// ���x�����A���ŏオ�鏈���Ƃ���while���K�v
	while (player_status.next_level <= 0)
	{
		if (!seFlg)
		{
			PlaySoundMem(_soundSE[1], DX_PLAYTYPE_BACK, true);
			seFlg = true;
		}
		// �X�e�[�^�X�A�b�v
		player_status.attackDamage += 1;
		player_status.maxHP += 3;
		player_status.now_level++;

		// ���̃��x���܂ł̌o���l�͏�������������
		player_status.next_level += 10 * player_status.now_level;
	}
}

int Player::GetNextLevel(void)
{
	return player_status.next_level;
}

int Player::GetNowLevel(void)
{
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