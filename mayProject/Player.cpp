#include <math.h>
#include "Dxlib.h"
#include "Enemy_weak.h"
#include "Menu.h"
#include "GameScene.h"
#include "Player.h"
#include "MouseCtl.h"

// static�ϐ��̎���<�^>�N���X��::�ϐ��� = ������;
bool Player::_loadFlg = false;
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
	for (int i = 0; i < 6; i++)
	{
		DeleteSoundMem(_soundSE[i]);
	}
	delete mouse;
}

void Player::Init(void)
{
	if (!_loadFlg)
	{
		player_status.now_level = 1;
		player_status.maxHP = 35;
		player_status.plHP = player_status.maxHP;
		player_status.attackDamage = 3;
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

	mouse = new MouseCtl();

	// �X�L���֌W
	_skillCharge = SKILL_CHARGE;
	_skillFlg = false;
	_skillBackFlg = false;
	_pngLight = 50;
	_lightFlg = false;
	_seSkillOnceFlg = false;
	_animCnt = 0;
	_animUpDateSpeedCnt = 0;

	// �o���A�֌W
	_barrierMaxNum = 0;
	_barrierNum = 0;

	_levelUpAnounceFlg = false;

	_soundSE[0] = LoadSoundMem("sound/se/click.mp3");
	_soundSE[1] = LoadSoundMem("sound/se/levelup.mp3");
	_soundSE[2] = LoadSoundMem("sound/se/skill.mp3");
	_soundSE[3] = LoadSoundMem("sound/se/skill_sword_short.mp3");
	_soundSE[4] = LoadSoundMem("sound/se/skill_barrier.mp3");
	_soundSE[5] = LoadSoundMem("sound/se/skill_heal.mp3");

	// ���ʂ�������
	ChangeVolumeSoundMem(128, _soundSE[1]);

	PngInit();
}

void Player::PngInit(void)
{
	skillImages.try_emplace("skillIcon", LoadGraph("image/skillicon.png"));		// �X�L���A�C�R��
	skillImages.try_emplace("announce" , LoadGraph("image/chargeAnnounce.png"));	// �X�L���g����Ƃ����A�i�E���X
	skillImages.try_emplace("skillBack", LoadGraph("image/skillBack.png"));		// �X�L���w�i
	skillImages.try_emplace("attack"   , LoadGraph("image/skill_attack.png"));	// �U���n�X�L���A�C�R��
	skillImages.try_emplace("barrier"  , LoadGraph("image/skill_barrier.png"));	// �h��n�X�L���A�C�R��
	skillImages.try_emplace("heal"     , LoadGraph("image/skill_heal.png"));		// �񕜌n�X�L���A�C�R��
	skillImages.try_emplace("cancel"   , LoadGraph("image/cancel.png"));			// ��߂�̕����摜
	skillImages.try_emplace("muscle"   , LoadGraph("image/muscle.png"));			// �͂��Ԃ̃A�C�R���摜

	_drawHandle.try_emplace("barrier_back", LoadGraph("image/barrier_back.png"));
	_drawHandle.try_emplace("barrier_bar", LoadGraph("image/barrier_bar.png"));
	_drawHandle.try_emplace("barrier_waku", LoadGraph("image/barrier_waku.png"));

	_drawHandle.try_emplace("hpbar_pl", LoadGraph("image/hpbar_pl.png"));
	_drawHandle.try_emplace("hpbar_plPoison", LoadGraph("image/hpbar_plPoison.png"));
	_drawHandle.try_emplace("hpbar_back", LoadGraph("image/hpbar_back.png"));
	_drawHandle.try_emplace("hpbar_waku", LoadGraph("image/hpbar_waku.png"));

	// �X�L���A�j���[�V����(��)
	std::string swordAnim = "image/anim/swordAnim.png";
	LoadDivGraph(swordAnim.c_str(), 12, 1, 12, 640, 240, _skillAnimSword);
	// �X�L���A�j���[�V����(�o���A)
	std::string gaurdAnim = "image/anim/gaurdAnim.png";
	LoadDivGraph(gaurdAnim.c_str(), 10, 5, 2, 240, 240, _skillAnimGuard);
	// �X�L���A�j���[�V����(��)
	std::string healAnim = "image/anim/healAnim.png";
	LoadDivGraph(healAnim.c_str(), 10, 10, 1, 240, 240, _skillAnimHeal);
}

void Player::ClickUpDate(Monster* monster, Menu* menu, GameScene* game, Cards* cards)
{
	mouse->UpDate();
	// �X�L���g�p�\���̃}�E�X�N���b�N�ʒu�ƃA�C�R��(�~)�Ƃ̓����蔻��
	// �A�C�e����ʒ��̓X�L���`���[�W�A�C�R���������Ȃ�
	if (!menu->GetMenuBackPngFlg() && player_status.plHP > 0)
	{
		if (_skillFlg)
		{
			//// 782,564(�A�C�R���`��ʒu)
			//float a = x - 782;
			//float b = y - 564;
			//float c = sqrt(a * a + b * b);
			//
			//// �����蔻��(�������Ă���Ƃ�)
			//if (c <= 34)
			//{
			//	PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
			//	// �X�L���̓^�[������Ȃ��ōs���铮��
			//	_skillBackFlg = true;
			//}

			// 782,564(�A�C�R���`��ʒu)
			float a = mouse->GetPos().x - 782.0f;
			float b = mouse->GetPos().y - 564.0f;
			// �����蔻��(�������Ă���Ƃ�)
			if (sqrt(a * a + b * b) <= 34)
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
		if (mouse->GetPos().x >= 385 && mouse->GetPos().x <= 385 + 150 && mouse->GetPos().y >= 320 && mouse->GetPos().y <= 320 + 65)
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
		if (mouse->GetPos().x >= 290 && mouse->GetPos().x <= 290 + 100 && mouse->GetPos().y >= 150 && mouse->GetPos().y <= 150 + 100)
		{
			PlaySoundMem(_soundSE[3], DX_PLAYTYPE_BACK, true);
			_skill = SKILL::SWORD;
			// �U���n(��b�U����*5+����З͂ň��̃_���[�W��^������)
			monster->Damage(player_status.attackDamage * 5 + menu->GetEquipDamage(), cards);
			game->blinkFlg = true;
			lambda();
		}

		// �h��A�C�R���Ƃ̓����蔻��
		if (mouse->GetPos().x >= 410 && mouse->GetPos().x <= 410 + 100 && mouse->GetPos().y >= 150 && mouse->GetPos().y <= 150 + 100)
		{
			PlaySoundMem(_soundSE[4], DX_PLAYTYPE_BACK, true);
			_skill = SKILL::GUARD;
			// �h��n(����l*�v���C���[���x��)
			_barrierMaxNum = 20 + 3 * player_status.now_level;
			_barrierNum = 20 + 3 * player_status.now_level;
			lambda();
		}

		// �񕜃A�C�R���Ƃ̓����蔻��
		if (mouse->GetPos().x >= 530 && mouse->GetPos().x <= 530 + 100 && mouse->GetPos().y >= 150 && mouse->GetPos().y <= 150 + 100)
		{
			PlaySoundMem(_soundSE[5], DX_PLAYTYPE_BACK, true);
			_skill = SKILL::HEAL;
			// �񕜌n(�S��+��Ԉُ��)
			player_status.plHP = player_status.maxHP;
			player_status.condition = CONDITION::FINE;
			player_status.conditionTurnNum = 0;
			lambda();
		}
	}
}

void Player::UpDate(void)
{
	if (_skill != SKILL::NON)
	{
		_animUpDateSpeedCnt++;
		if (_animUpDateSpeedCnt % 3 == 0)
		{
			_animCnt++;
		}
	}
	else
	{
		_animUpDateSpeedCnt = 0;
		_animCnt = 0;
	}

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
	// HP�o�[�֘A�摜�T�C�Y
	int posx = 750;
	int posy = 450;
	int plHPBar;
	if (player_status.condition == CONDITION::POISON)
	{
		// �ŏ�Ԃ̎���HP�o�[�̐F���������ɂ���
		plHPBar = _drawHandle["hpbar_plPoison"];
	}
	else
	{
		// �ʏ��Ԃ̎���HP�o�[�̐F
		plHPBar = _drawHandle["hpbar_pl"];
	}
	DrawExtendGraph(posx, posy, posx + 130, posy + 33, _drawHandle["hpbar_back"], true);
	DrawExtendGraph(posx + 3, posy + 4, posx + 3 + 125 * ((float)player_status.plHP / (float)player_status.maxHP), posy + 4 + 25, plHPBar, true);
	DrawExtendGraph(posx, posy, posx + 130, posy + 33, _drawHandle["hpbar_waku"], true);

	// �E���ē��\��
	DrawFormatString(750, 425, 0xffffff, "�̗�:%d / %d", player_status.plHP, player_status.maxHP);
	if (player_status.conditionTurnNum != 0)
	{
		DrawFormatString(750, 483, 0xffffff, "�ŉ񕜂܂�:%d", player_status.conditionTurnNum);
	}
	if (menu->GetPowUp() != 0)
	{
		DrawFormatString(750, 507, 0xffffff, "�U������:+%d", menu->GetPowUp());
	}
}

void Player::BattleDraw(Menu* menu)
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
		DrawRotaGraph(750 + 32, 530 + 32, 1.0f, 0, skillImages["skillIcon"], true);
		// �`��u�����h���[�h���m�[�u�����h�ɂ���
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		DrawRotaGraph(750 + 32, 530 + 32, 1.0f, 0, skillImages["muscle"], true);
		DrawGraph(820, 530, skillImages["announce"], true);
	}

	// �X�L���A�C�R���������ꂽ��w�i��`�悷��
	if (_skillBackFlg)
	{
		int iconIntervalX = 120;	// �A�C�R���̕`��Ԋu
		DrawGraph(250, 90, skillImages["skillBack"], true);
		DrawGraph(290, 150, skillImages["attack"], true);
		DrawGraph(290 + iconIntervalX, 150, skillImages["barrier"], true);
		DrawGraph(290 + iconIntervalX * 2, 150, skillImages["heal"], true);
		DrawFormatString(280,250, 0x000000, "�U���X�L��:\n%d�_���[�W", player_status.attackDamage * 5 + menu->GetEquipDamage());
		DrawFormatString(410,250, 0x000000, "�h��X�L��:\n�ϋv%d��\n�o���A�W�J", 20 + 3 * player_status.now_level);
		DrawFormatString(530, 250, 0x000000, "�񕜃X�L��:\n�̗�+��Ԉُ�\n�S��");
		DrawGraph(385, 320, skillImages["cancel"], true);
	}

	// �o���A�o�[�̕\��
	if (_barrierNum > 0)
	{
		int posx = 600;
		int posy = 350;
		DrawFormatString(600, 325, 0xffffff, "�o���A�ϋv:%d/%d", _barrierNum, _barrierMaxNum);
		DrawExtendGraph(posx, posy, posx + 150, posy + 33, _drawHandle["barrier_back"], true);
		DrawExtendGraph(posx+3, posy+4, posx+3 + 145 * ((float)_barrierNum / (float)_barrierMaxNum), posy+4 + 25, _drawHandle["barrier_bar"], true);
		DrawExtendGraph(posx, posy, posx + 150, posy + 33, _drawHandle["barrier_waku"], true);
	}
}

void Player::SkillDraw(void)
{
	if (_skill == SKILL::SWORD)
	{
		if (_animCnt <= 11)
		{
			DrawRotaGraph(300, 300, 2.0f, 0, _skillAnimSword[_animCnt], true);
		}
		else
		{
			_skill = SKILL::NON;
		}
	}

	if (_skill == SKILL::GUARD)
	{
		if (_animCnt <= 9)
		{
			DrawRotaGraph(450, 250, 2.0f, 0, _skillAnimGuard[_animCnt], true);
		}
		else
		{
			_skill = SKILL::NON;
		}
	}

	if (_skill == SKILL::HEAL)
	{
		if (_animCnt <= 9)
		{
			// �s���߉摜�̂��߉��Z�����ŕ`�悷��K�v������
			SetDrawBlendMode(DX_BLENDMODE_ADD, 256);
			DrawRotaGraph(450, 250, 2.0f, 0, _skillAnimHeal[_animCnt], true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 1);
		}
		else
		{
			_skill = SKILL::NON;
		}
	}

}

void Player::SetHP(const int& hpNum)
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

int Player::GetHP(void)const
{
	return player_status.plHP;
}

void Player::SetMaxHP(const int& hpNum)
{
	player_status.maxHP = hpNum;
}

int Player::GetMaxHP(void)const
{
	return player_status.maxHP;
}

float Player::GetHPBar(void)const
{
	return (float)player_status.plHP / (float)player_status.maxHP;
}

int Player::GetAttackDamage(void)const
{
	return player_status.attackDamage;
}

void Player::SetDifense(const int& num)
{
	player_status.defense = num;
}

int Player::GetDifense(void)const
{
	return player_status.defense;
}

void Player::SetNextLevel(const int& num)
{
	player_status.next_level = num;
	bool seFlg = false;

	// ���x�����A���ŏオ�鏈���Ƃ���while���K�v
	while (player_status.next_level <= 0)
	{
		_levelUpAnounceFlg = true;
		if (!seFlg)
		{
			PlaySoundMem(_soundSE[1], DX_PLAYTYPE_BACK, true);
			seFlg = true;
		}
		// �X�e�[�^�X�A�b�v
		player_status.attackDamage += 2;
		player_status.maxHP += 3;
		player_status.now_level++;

		// ���̃��x���܂ł̌o���l�͏�������������
		player_status.next_level += 10 * player_status.now_level;
	}
}

int Player::GetNextLevel(void)const
{
	return player_status.next_level;
}

int Player::GetNowLevel(void)const
{
	return player_status.now_level;
}

void Player::SetMoney(const int& num)
{
	player_status.money = num;
}

int Player::GetMoney(void)const
{
	return player_status.money;
}

void Player::SetCondition(const CONDITION& con)
{
	player_status.condition = con;
	if (player_status.condition == CONDITION::POISON)
	{
		player_status.conditionTurnNum = 5;
	}
}

CONDITION Player::GetCondition(void)const
{
	return player_status.condition;
}

void Player::SetConditionTurn(const int& num)
{
	player_status.conditionTurnNum = num;
}

int Player::GetConditionTurn(void)const
{
	return player_status.conditionTurnNum;
}

void Player::SetSkillCharge(const int& num)
{
	_skillCharge = num;
}

int Player::GetSkillCharge(void)const
{
	return _skillCharge;
}

bool Player::GetSkillBackFlg(void)const
{
	return _skillBackFlg;
}

void Player::SetBarrierNum(const int& num)
{
	_barrierNum = num;
}

int Player::GetBarrierNum(void)const
{
	return _barrierNum;
}

void Player::SetLevelUpAnounceFlg(const bool& flag)
{
	_levelUpAnounceFlg = flag;
}

bool Player::GetLevelUpAnounceFlg(void)const
{
	return _levelUpAnounceFlg;
}