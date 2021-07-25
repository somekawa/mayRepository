#include <DxLib.h>
#include "../Enemy/Enemy_weak.h"
#include "Cards.h"

#define CARD_SET_NUM  5		// �퓬���ɕ`�悳���J�[�h�̖���
#define CARD_SIZE_X 126		// �J�[�h1���̉��T�C�Y
#define CARD_SIZE_Y 180		// �J�[�h1���̏c�T�C�Y
#define CARD_SET_LINE 140   // �J�[�h��ݒu����ۂ̐�
#define CARD_INIT_POS_Y 500	// �J�[�h��y���W�����ʒu

// �J�[�h�Ɋւ�����
struct cardsPNG
{
	VECTOR2 pos;			// ���W
	CARD_MEMBER cardMem;	// ���
	int num;				// �ԍ�
	int pngSel;				// �摜
} card[9];

Cards::Cards()
{
	Init();
}

Cards::~Cards()
{
	DeleteSoundMem(_se);
}

void Cards::Update(void)
{
	_mouse = GetMouseInput();						 // �}�E�X�̓��͏�Ԏ擾
	GetMousePoint(&_cursorPos.x, &_cursorPos.y);	 // �}�E�X�̍��W�擾
	if (!_clickFlg)
	{
		if (_mouse & MOUSE_INPUT_LEFT) // �}�E�X�̍��{�^����������Ă�����
		{			
			// �摜�ƃ}�E�X�N���b�N�̓����蔻��
			for (int i = 0; i <= CARD_SET_NUM; i++)
			{
				if (_cursorPos.x >= card[i].pos.x - CARD_SIZE_X / 2 && _cursorPos.x <= card[i].pos.x + CARD_SIZE_X / 2 &&
					_cursorPos.y >= card[i].pos.y - CARD_SIZE_Y / 2 && _cursorPos.y <= card[i].pos.y + CARD_SIZE_Y / 2)
				{
					_clickFlg = true;
					_id = i;
				}
			}
		}
		else
		{
			// ���N���b�N���Ă��Ȃ��Ƃ���id��-1�ɂ��Ă���
			_id = -1;
		}
	}

	// ���N���b�N�ŉ�����Ă���Ԃ�����������悤�ɂ��鏈��
	if (_clickFlg)
	{
		card[_id].pos = VECTOR2(_cursorPos);
	}
	else
	{
		_id = -1;
	}

	// �J�[�h�̓����̏���
	if (_id >= 0)
	{
		Move(_id);
	}
}

void Cards::SetTurn(const int& turn)
{
	_turnNum = turn;
}

int Cards::GetTurn(void)const
{
	return _turnNum;
}

void Cards::SetDamage(const int& damage)
{
	_damageNum = damage;
}

int Cards::GetDamage(void)const
{
	return _damageNum;
}

void Cards::SetGuard(const int& guard)
{
	_gaurdNum = guard;
	_sumGaurdNum = guard;
}

int Cards::GetGuard(void)const
{
	return _sumGaurdNum;
}

void Cards::SetHeal(const int& heal)
{
	_healNum = heal;
}

int Cards::GetHeal(void) const
{
	return _healNum;
}

CARDS_TYPE Cards::GetCardsType(void)const
{
	return _cardsType;
}

void Cards::SetCardsType(const CARDS_TYPE& cards)
{
	_cardsType = cards;
}

bool Cards::Init(void)
{
	_se = LoadSoundMem("sound/se/cards.mp3");

	_turnNum = 3;	// �G�ɂ���đ���
	_damageNum = 0;
	_gaurdNum  = 0;
	_sumGaurdNum = 0;
	_healNum = 0;

	_cardsType = CARDS_TYPE::NON;

	// �J�[�h�̕����ǂݍ���
	std::string cardpng = "image/card.png";
	LoadDivGraph(cardpng.c_str(), 9, 1, 9, CARD_SIZE_X, CARD_SIZE_Y, _cardPNG);

	_clickFlg = false;
	_cursorPos = VECTOR2(0, 0);

	// �J�[�h�З̓e�[�u��
	const int cardPowerTbl[static_cast<int>(CARD_MEMBER::MAX) - static_cast<int>(CARD_MEMBER::ATTACK_1)] = {
		1,9,25,1,3,8,1,3,8
	};

	int num = 0;
	for (auto member : CARD_MEMBER())
	{
		// �摜��enum class�̏�����v������
		_cardPngSel_pair[static_cast<int>(member) - 1] = std::make_pair(_cardPNG[static_cast<int>(member) - 1], member);
		// �J�[�h�̈З͂�ݒ肷��
		_cardPower.try_emplace(member, cardPowerTbl[num]);
		num++;
	}

	for (int i = 0; i < CARD_SET_NUM; i++)
	{
		// �{�b�N�X�ʒu�̌���
		_setCardPos[i] = CARD_SET_LINE * (i + 1);
		// �L�[�����݂��Ȃ��Ƃ��ɍ\�z����
		_cardMap.try_emplace(_setCardPos[i], CARD_MEMBER::NON);

		// �J�[�h�ʒu�̌���
		card[i].pos = VECTOR2(_setCardPos[i] - CARD_SET_LINE / 2, CARD_INIT_POS_Y);
		card[i].num = i;
		CreateCard(i);
	}

	_id = -1;
	return true;
}

void Cards::SetCardPos(int id, int posId)
{
	// ���W��߂�
	card[id].pos.x = _setCardPos[card[posId].num] - CARD_SET_LINE / 2;
	card[id].pos.y = CARD_INIT_POS_Y;
}

void Cards::Draw(const std::shared_ptr<Player>& player, const std::shared_ptr<Menu>& menu)
{
	//ClsDrawScreen();

	for (int i = 0; i < CARD_SET_NUM; i++)
	{
		// �����ŃJ�[�h�����ׂ�Draw���Ă��܂��ƁA
		// �N���b�N�����J�[�h���E���̃J�[�h��肵���ɕ`�悳��Ă��܂��̂ł킯��
		if (_id < 0)
		{
			// �ʏ�͂�����Draw
			DrawRotaGraph(card[i].pos.x, card[i].pos.y, 1.0f, 0, card[i].pngSel, false);
		}
		else
		{
			// ���N���b�N���ď������Ă���J�[�h������Ƃ�
			// ���N���b�N����Ă���J�[�h�ȊO�̕`��
			if (_id != i)
			{
				// �����J�[�h�̎�ނ��قȂ�Ƃ�(�����\��)
				if (card[i].cardMem != card[_id].cardMem)
				{
					// �`��u�����h���[�h�ύX
					SetDrawBlendMode(DX_BLENDMODE_ADD, 100);
					DrawRotaGraph(card[i].pos.x, card[i].pos.y, 1.0f, 0, card[i].pngSel, false);
					// �`��u�����h���[�h���m�[�u�����h�ɂ���
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				}
				else
				{
					// �����J�[�h�̎�ނ������Ƃ�
					DrawRotaGraph(card[i].pos.x, card[i].pos.y, 1.0f, 0, card[i].pngSel, false);
				}
			}
		}
	}

	// �c��
	for (int i = CARD_SET_LINE + 1; i <= (CARD_SET_LINE * CARD_SET_NUM) + 1;)
	{
		DrawLine(i, 400, i, 600, GetColor(255, 0, 0), 2);
		i += CARD_SET_LINE;
	}

	// ����
	DrawLine(0, 400, 900, 400, GetColor(255, 0, 0), 2);

	// id�������Ă���Ƃ��͂��̃J�[�h����ԏ�ɂ���悤�ɂ�����Draw����
	// ���N���b�N����Ă���J�[�h�̕`��
	if (_id >= 0)
	{
		DrawRotaGraph(card[_id].pos.x, card[_id].pos.y, 1.0f, 0, card[_id].pngSel, false);
	}

	const VECTOR2 _offset(-80,90);

	if (_id == -1)
	{
		return;		// _id��-1�̍ۂ�return
	}

	// �J�[�h���̕\��
	if (card[_id].pos.y > 0 && card[_id].pos.y <= 300)
	{
		std::string cardType;
		const int cardMember = static_cast<int>(card[_id].cardMem);	// ���쒆�̃J�[�h�̏����擾
		// �擾�����J�[�h�̎�ނɂ���Čv�Z�������ʂ��
		if (cardMember >= static_cast<int>(CARD_MEMBER::ATTACK_1) && cardMember <= static_cast<int>(CARD_MEMBER::ATTACK_3))
		{
			cardType = "�U��";
			DrawFormatString(card[_id].pos.x + _offset.x, card[_id].pos.y + _offset.y, 0xffffff, "%s:�G��%d�̃_���[�W", cardType.c_str(),
					(cardMember + (cardMember - 1))
				* (cardMember + (cardMember - 1)) 
				+  player->GetAttackDamage() + menu->GetEquipDamage() + menu->GetPowUp());
		}
		else if (cardMember >= static_cast<int>(CARD_MEMBER::GUARD_1) && cardMember <= static_cast<int>(CARD_MEMBER::GUARD_3))
		{
			cardType = "�h��";
			DrawFormatString(card[_id].pos.x + _offset.x, card[_id].pos.y + _offset.y, 0xffffff, "%s:�_���[�W%d���̌y��", cardType.c_str(),
					((cardMember % static_cast<int>(CARD_MEMBER::GUARD_1)) + 1)
				+ (cardMember % static_cast<int>(CARD_MEMBER::GUARD_1)) + (cardMember / NUM_CAL(CARD_MEMBER::GUARD_1)));
		}
		else if (cardMember >= static_cast<int>(CARD_MEMBER::HEAL_1) &&  cardMember <= static_cast<int>(CARD_MEMBER::HEAL_3))
		{
			cardType = "��";
			DrawFormatString(card[_id].pos.x + _offset.x, card[_id].pos.y + _offset.y, 0xffffff, "%s:�̗�%d���̉�", cardType.c_str(),
					((cardMember % static_cast<int>(CARD_MEMBER::HEAL_1)) + 1)
				+ (cardMember % static_cast<int>(CARD_MEMBER::HEAL_1)) + (cardMember / NUM_CAL(CARD_MEMBER::HEAL_1)));
		}
		else
		{
			return;		// �����ɔ��ŗ�����G���[
		}
	}

	//ScreenFlip();
}

void Cards::Move(int id)
{
	// �͈͊O����
	if (card[id].pos.x < 0)
	{
		card[id].pos.x = 0;
	}
	else if (card[id].pos.x > CARD_SET_LINE * CARD_SET_NUM)
	{
		card[id].pos.x = CARD_SET_LINE * CARD_SET_NUM;
	}

	// ��������ɂ��������D���g���������s��
	if (card[id].pos.y <= 300)
	{
		// ���N���b�N��������
		if (_clickFlg && !(_mouse & MOUSE_INPUT_LEFT))
		{
			UseCard(id);
			SetCardPos(id,id);
			_clickFlg = false;
		}
		return;
	}

	// �����J�[�h�̎�ނŏd�˂鎞�ɍs������
	auto cardStackLambda = [&](int num) {
		SetCardPos(id,id);

		if (card[id].cardMem == CARD_MEMBER::ATTACK_3 || card[id].cardMem == CARD_MEMBER::GUARD_3 || card[id].cardMem == CARD_MEMBER::HEAL_3)
		{
			return;
		}
		else
		{
			PlaySoundMem(_se, DX_PLAYTYPE_BACK, true);

			// _cardPngSel_pair��non����n�܂�킯�ł͂Ȃ��̂ŁA+1���Ȃ��Ă���
			card[num].pngSel  = _cardPngSel_pair[static_cast<int>(card[id].cardMem)].first;
			card[num].cardMem = _cardPngSel_pair[static_cast<int>(card[id].cardMem)].second;
			_cardMap[_setCardPos[num]] = card[num].cardMem;
			CreateCard(id);
			_turnNum -= 1;		// �G�̑ҋ@�^�[�� -1
		}
	};

	// �J�[�h�ړ������̋��ʕ���
	auto cardMove = [&](int num) {
		// �g���ɉ��������Ă��Ȃ��Ƃ�
		if (_cardMap[_setCardPos[num]] == CARD_MEMBER::NON)
		{
			// num������������O��non�ɂ���
			_cardMap[_setCardPos[card[id].num]] = CARD_MEMBER::NON;

			// �ړ������Ă���
			SetCardPos(id, num);
			card[id].num = num;

			// �V�����ړ������ꏊ�ɃJ�[�h�̎�ނ�o�^
			_cardMap[_setCardPos[num]] = card[id].cardMem;
		}
		else if (_cardMap[_setCardPos[num]] == card[id].cardMem && card[id].num != num)
		{
			cardStackLambda(num);
		}
		else
		{
			SetCardPos(id, id);
		}
		_clickFlg = false;
	};

	// ���N���b�N��������
	if (_clickFlg && !(_mouse & MOUSE_INPUT_LEFT))
	{
		// ���[
		if (card[id].pos.x >= 0 && card[id].pos.x <= _setCardPos[0])
		{
			cardMove(0);
		}

		for (int i = 0; i <= 3; i++)
		{
			if (card[id].pos.x > _setCardPos[i] && card[id].pos.x <= _setCardPos[i + 1])
			{
				cardMove(i + 1);
			}
		}
	}
}

void Cards::UseCard(int id)
{
	// �g�p�����J�[�h��attack�̂Ƃ�
	if (card[id].cardMem == CARD_MEMBER::ATTACK_1 || card[id].cardMem == CARD_MEMBER::ATTACK_2 || card[id].cardMem == CARD_MEMBER::ATTACK_3)
	{
		_cardsType = CARDS_TYPE::ATTACK;

		// �G��HP�����炷����
		_damageNum = _cardPower[card[id].cardMem];

		CreateCard(id);
		_turnNum -= 1;		// �G�̑ҋ@�^�[�� -1
		return;
	}
	// �g�p�����J�[�h��guard�̂Ƃ�
	else if (card[id].cardMem == CARD_MEMBER::GUARD_1 || card[id].cardMem == CARD_MEMBER::GUARD_2 || card[id].cardMem == CARD_MEMBER::GUARD_3)
	{
		_cardsType = CARDS_TYPE::GUARD;

		// �󂯂�U���������Ŗh��ł���悤�ɂ���
		_gaurdNum = _cardPower[card[id].cardMem];

		// ������̃^�[���œ������ʂ̃J�[�h���g�p����Ƃ��ɍ��v���K�v�ɂȂ�
		_sumGaurdNum += _gaurdNum;

		CreateCard(id);
		_turnNum -= 1;		// �G�̑ҋ@�^�[�� -1
		return;
	}
	// �g�p�����J�[�h��heal�̂Ƃ�
	else if (card[id].cardMem == CARD_MEMBER::HEAL_1 || card[id].cardMem == CARD_MEMBER::HEAL_2 || card[id].cardMem == CARD_MEMBER::HEAL_3)
	{
		_cardsType = CARDS_TYPE::HEAL;

		// �v���C���[�̍ő�HP�̊����ŉ񕜂���
		_healNum = _cardPower[card[id].cardMem];

		CreateCard(id);
		_turnNum -= 1;		// �G�̑ҋ@�^�[�� -1
		return;
	}
	else
	{
		return;	// �G���[
	}
}

void Cards::CreateCard(int id)
{
	// ���������̃X�y�[�X�ɐV�����J�[�h�̐������s��
	// 0 3 6�̃J�[�h�݂̂�������
	const int userand = (GetRand(8) % 3) * 3;
	card[id].pngSel   = _cardPngSel_pair[userand].first;
	card[id].cardMem  = _cardPngSel_pair[userand].second;
	_cardMap[_setCardPos[id]] = card[id].cardMem;
}