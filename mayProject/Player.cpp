#include <math.h>
#include "Dxlib.h"
#include "Enemy/Enemy_weak.h"
#include "Menu.h"
#include "Scene/GameScene.h"
#include "Player.h"
#include "Common/MouseCtl.h"

// static�ϐ��̎���<�^>�N���X��::�ϐ��� = ������;
bool Player::loadFlg = false;
int Player::saveData[9] = { 0,0,0,0,0,0,0,0,0 };

// �X�e�[�^�X
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
}

void Player::Init(void)
{
	if (!loadFlg)
	{
		player_status.now_level	   = 1;
		player_status.maxHP		   = 35;
		player_status.plHP		   = player_status.maxHP;
		player_status.attackDamage = 3;
		player_status.defense      = 0;
		player_status.next_level   = 10;
		player_status.money		   = 1000;
		player_status.conditionTurnNum = 0;
		player_status.condition = CONDITION::FINE;
	}
	else
	{
		player_status.now_level	   = saveData[0];
		player_status.maxHP		   = saveData[1];
		player_status.plHP	       = saveData[2];
		player_status.attackDamage = saveData[3];
		player_status.defense      = saveData[4];
		player_status.next_level   = saveData[5];
		player_status.money		   = saveData[6];
		player_status.conditionTurnNum = saveData[7];
		player_status.condition = static_cast<CONDITION>(saveData[8]);
	}

	_mouse = std::make_unique<MouseCtl>();

	// �X�L���֌W
	_skill = SKILL::NON;
	_skillCharge  = SKILL_CHARGE;
	_skillFlg     = false;
	_skillBackFlg = false;
	_pngLight = 50;
	_lightFlg = false;
	_seSkillOnceFlg = false;
	_animCnt = 0;
	_animUpDateSpeedCnt = 0;

	// �o���A�֌W
	_barrierMaxNum = 0;
	_barrierNum    = 0;

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
	_skillImages.try_emplace("skillIcon", LoadGraph("image/skillicon.png"));		// �X�L���A�C�R��
	_skillImages.try_emplace("announce" , LoadGraph("image/chargeAnnounce.png"));	// �X�L���g����Ƃ����A�i�E���X
	_skillImages.try_emplace("skillBack", LoadGraph("image/skillBack.png"));		// �X�L���w�i
	_skillImages.try_emplace("attack"   , LoadGraph("image/skill_attack.png"));	// �U���n�X�L���A�C�R��
	_skillImages.try_emplace("barrier"  , LoadGraph("image/skill_barrier.png"));	// �h��n�X�L���A�C�R��
	_skillImages.try_emplace("heal"     , LoadGraph("image/skill_heal.png"));		// �񕜌n�X�L���A�C�R��
	_skillImages.try_emplace("cancel"   , LoadGraph("image/cancel.png"));			// ��߂�̕����摜
	_skillImages.try_emplace("muscle"   , LoadGraph("image/muscle.png"));			// �͂��Ԃ̃A�C�R���摜

	_drawHandle.try_emplace("barrier_back", LoadGraph("image/barrier_back.png"));
	_drawHandle.try_emplace("barrier_bar" , LoadGraph("image/barrier_bar.png"));
	_drawHandle.try_emplace("barrier_waku", LoadGraph("image/barrier_waku.png"));

	_drawHandle.try_emplace("hpbar_pl", LoadGraph("image/hpbar_pl.png"));
	_drawHandle.try_emplace("hpbar_plPoison", LoadGraph("image/hpbar_plPoison.png"));
	_drawHandle.try_emplace("hpbar_back", LoadGraph("image/hpbar_back.png"));
	_drawHandle.try_emplace("hpbar_waku", LoadGraph("image/hpbar_waku.png"));

	// �X�L���A�j���[�V����(��)
	const std::string swordAnim = "image/anim/swordAnim.png";
	LoadDivGraph(swordAnim.c_str(), 12, 1, 12, 640, 240, _skillAnimSword);
	// �X�L���A�j���[�V����(�o���A)
	const std::string gaurdAnim = "image/anim/gaurdAnim.png";
	LoadDivGraph(gaurdAnim.c_str(), 10, 5, 2, 240, 240, _skillAnimGuard);
	// �X�L���A�j���[�V����(��)
	const std::string healAnim  = "image/anim/healAnim.png";
	LoadDivGraph(healAnim.c_str(), 10, 10, 1, 240, 240, _skillAnimHeal);
}

void Player::ClickUpDate(const std::shared_ptr<Monster>& monster, const std::shared_ptr<Menu>& menu, GameScene* game, const std::shared_ptr<Cards>& cards)
{
	_mouse->UpDate();
	// �X�L���g�p�\���̃}�E�X�N���b�N�ʒu�ƃA�C�R��(�~)�Ƃ̓����蔻��
	// �A�C�e����ʒ��̓X�L���`���[�W�A�C�R���������Ȃ�
	if (!menu->GetMenuBackPngFlg() && (player_status.plHP > 0))
	{
		if (_skillFlg)
		{
			const float tmpx = _mouse->GetPos().x - 782.0f;
			const float tmpy = _mouse->GetPos().y - 564.0f;
			// �����蔻��(�������Ă���Ƃ�)
			if (sqrt(tmpx * tmpx + tmpy * tmpy) <= 34)
			{
				PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
				// �X�L���̓^�[������Ȃ��ōs���铮��
				_skillBackFlg = true;
			}
		}
	}

	if (!_skillBackFlg)
	{
		return;	// �X�L���{�^�����񉟉��Ȃ�return
	}

	// ��߂�{�^���Ƃ̓����蔻��
	if (_mouse->GetPos().x >= 385 && _mouse->GetPos().x <= 385 + 150 &&
		_mouse->GetPos().y >= 320 && _mouse->GetPos().y <= 320 + 65)
	{
		PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
		_skillBackFlg = false;
	}

	auto SkillResetLambda = [&]() {
		PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
		// �t���O�Ɖ񐔂����ɖ߂�
		_seSkillOnceFlg = false;
		_skillFlg = false;
		_skillBackFlg = false;
		_skillCharge = SKILL_CHARGE;
	};

	// �U���A�C�R���Ƃ̓����蔻��
	if (_mouse->GetPos().x >= 290 && _mouse->GetPos().x <= 290 + 100 &&
		_mouse->GetPos().y >= 150 && _mouse->GetPos().y <= 150 + 100)
	{
		PlaySoundMem(_soundSE[3], DX_PLAYTYPE_BACK, true);
		_skill = SKILL::SWORD;
		// �U���n(��b�U����*5+����З͂ň��̃_���[�W��^������)
		monster->Damage(player_status.attackDamage * 5 + menu->GetEquipDamage(), cards);
		game->blinkFlg = true;
		SkillResetLambda();
	}

	// �h��A�C�R���Ƃ̓����蔻��
	if (_mouse->GetPos().x >= 410 && _mouse->GetPos().x <= 410 + 100 &&
		_mouse->GetPos().y >= 150 && _mouse->GetPos().y <= 150 + 100)
	{
		PlaySoundMem(_soundSE[4], DX_PLAYTYPE_BACK, true);
		_skill = SKILL::GUARD;
		// �h��n(����l*�v���C���[���x��)
		_barrierMaxNum = 20 + 3 * player_status.now_level;
		_barrierNum = 20 + 3 * player_status.now_level;
		SkillResetLambda();
	}

	// �񕜃A�C�R���Ƃ̓����蔻��
	if (_mouse->GetPos().x >= 530 && _mouse->GetPos().x <= 530 + 100 &&
		_mouse->GetPos().y >= 150 && _mouse->GetPos().y <= 150 + 100)
	{
		PlaySoundMem(_soundSE[5], DX_PLAYTYPE_BACK, true);
		_skill = SKILL::HEAL;
		// �񕜌n(�S��+��Ԉُ��)
		player_status.plHP = player_status.maxHP;
		player_status.condition = CONDITION::FINE;
		player_status.conditionTurnNum = 0;
		SkillResetLambda();
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

	if (_skillCharge > 0)
	{
		return;		// �X�L���̃`���[�W�����܂肫���Ă��Ȃ��Ƃ���return
	}

	// �X�L�����g�p�\�Ȏ��Ƀt���O�𗧂ĂāA�����蔻����s��
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
	else
	{
		_pngLight -= 5;
		if (_pngLight <= 50)
		{
			_lightFlg = false;
		}
	}
}

void Player::Draw(const std::shared_ptr<Menu>& menu)
{
	// HP�o�[�֘A�摜�T�C�Y
	const int posx = 750;
	const int posy = 450;
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
	// float����int�ɂ�������m�F���Ă���
	DrawExtendGraph(posx + 3, posy + 4, posx + 3 + 125 * (static_cast<float>(player_status.plHP) / static_cast<float>(player_status.maxHP)), posy + 4 + 25, plHPBar, true);
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

void Player::BattleDraw(const std::shared_ptr<Menu>& menu)
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
		DrawRotaGraph(750 + 32, 530 + 32, 1.0f, 0, _skillImages["skillIcon"], true);
		// �`��u�����h���[�h���m�[�u�����h�ɂ���
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		DrawRotaGraph(750 + 32, 530 + 32, 1.0f, 0, _skillImages["muscle"], true);
		DrawGraph(820, 530, _skillImages["announce"], true);
	}

	// �X�L���A�C�R���������ꂽ��w�i��`�悷��
	if (_skillBackFlg)
	{
		int iconIntervalX = 120;	// �A�C�R���̕`��Ԋu
		DrawGraph(250, 90, _skillImages["skillBack"], true);
		DrawGraph(290, 150, _skillImages["attack"], true);
		DrawGraph(290 + iconIntervalX, 150, _skillImages["barrier"], true);
		DrawGraph(290 + iconIntervalX * 2, 150, _skillImages["heal"], true);
		DrawFormatString(280,250, 0x000000, "�U���X�L��:\n%d�_���[�W", player_status.attackDamage * 5 + menu->GetEquipDamage());
		DrawFormatString(410,250, 0x000000, "�h��X�L��:\n�ϋv%d��\n�o���A�W�J", 20 + 3 * player_status.now_level);
		DrawFormatString(530, 250, 0x000000, "�񕜃X�L��:\n�̗�+��Ԉُ�\n�S��");
		DrawGraph(385, 320, _skillImages["cancel"], true);
	}

	// �o���A�o�[�̕\��
	if (_barrierNum > 0)
	{
		int posx = 600;
		int posy = 350;
		DrawFormatString(600, 325, 0xffffff, "�o���A�ϋv:%d/%d", _barrierNum, _barrierMaxNum);
		DrawExtendGraph(posx, posy, posx + 150, posy + 33, _drawHandle["barrier_back"], true);
		// float����int�ɂ�������m�F���Ă���
		DrawExtendGraph(posx+3, posy+4, posx+3 + 145 * (static_cast<float>(_barrierNum) / static_cast<float>(_barrierMaxNum)), posy+4 + 25, _drawHandle["barrier_bar"], true);
		DrawExtendGraph(posx, posy, posx + 150, posy + 33, _drawHandle["barrier_waku"], true);
	}
}

void Player::SkillDraw(void)
{
	switch (_skill)
	{
	case SKILL::NON:
		break;
	case SKILL::SWORD:
		if (_animCnt <= 11)
		{
			DrawRotaGraph(300, 300, 2.0f, 0, _skillAnimSword[_animCnt], true);
		}
		else
		{
			_skill = SKILL::NON;
		}
		break;
	case SKILL::GUARD:
		if (_animCnt <= 9)
		{
			DrawRotaGraph(450, 250, 2.0f, 0, _skillAnimGuard[_animCnt], true);
		}
		else
		{
			_skill = SKILL::NON;
		}
		break;
	case SKILL::HEAL:
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
		break;
	default:
		break;
	}

	//if (_skill == SKILL::SWORD)
	//{
	//	if (_animCnt <= 11)
	//	{
	//		DrawRotaGraph(300, 300, 2.0f, 0, _skillAnimSword[_animCnt], true);
	//	}
	//	else
	//	{
	//		_skill = SKILL::NON;
	//	}
	//}

	//if (_skill == SKILL::GUARD)
	//{
	//	if (_animCnt <= 9)
	//	{
	//		DrawRotaGraph(450, 250, 2.0f, 0, _skillAnimGuard[_animCnt], true);
	//	}
	//	else
	//	{
	//		_skill = SKILL::NON;
	//	}
	//}

	//if (_skill == SKILL::HEAL)
	//{
	//	if (_animCnt <= 9)
	//	{
	//		// �s���߉摜�̂��߉��Z�����ŕ`�悷��K�v������
	//		SetDrawBlendMode(DX_BLENDMODE_ADD, 256);
	//		DrawRotaGraph(450, 250, 2.0f, 0, _skillAnimHeal[_animCnt], true);
	//		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 1);
	//	}
	//	else
	//	{
	//		_skill = SKILL::NON;
	//	}
	//}
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
	return static_cast<float>(player_status.plHP) / static_cast<float>(player_status.maxHP);
}

void Player::SetAttackDamage(const int& num)
{
	player_status.attackDamage = num;
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