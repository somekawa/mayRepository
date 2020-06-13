#include "DxLib.h"
#include "Enemy_weak.h"
#include "Cards.h"

#define CARD_SIZE_X 126		// �J�[�h1���̉��T�C�Y
#define CARD_SIZE_Y 180

struct cardsPNG
{
	VECTOR2 pos;		// ���W
	CARD_MEMBER syurui;	// ���
	int num;			// �ԍ�
	int pngSel;			// �摜
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
	_mouse = GetMouseInput();						 //�}�E�X�̓��͏�Ԏ擾
	GetMousePoint(&_cursorPos.x, &_cursorPos.y);	 //�}�E�X�̍��W�擾
	if (!_clickFlg)
	{
		if (_mouse & MOUSE_INPUT_LEFT) {			 //�}�E�X�̍��{�^����������Ă�����
			// �摜�ƃ}�E�X�N���b�N�̓����蔻��
			for (int i = 0; i <= 5; i++)
			{
				if (_cursorPos.x >= card[i].pos.x - CARD_SIZE_X / 2 && _cursorPos.x <= card[i].pos.x + CARD_SIZE_X / 2 && _cursorPos.y >= card[i].pos.y - CARD_SIZE_Y / 2 && _cursorPos.y <= card[i].pos.y + CARD_SIZE_Y / 2)
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

CARDS_SYURUI Cards::GetCardsSyurui(void)const
{
	return _cardsSyurui;
}

void Cards::SetCardsSyurui(const CARDS_SYURUI& cards)
{
	_cardsSyurui = cards;
}

bool Cards::Init(void)
{
	_se = LoadSoundMem("sound/se/cards.mp3");

	_turnNum = 3;	// �G�ɂ���đ���
	_damageNum = 0;
	_gaurdNum = 0;
	_sumGaurdNum = 0;
	_healNum = 0;

	_cardsSyurui = CARDS_SYURUI::NON;

	// �J�[�h�̕����ǂݍ���
	std::string cardpng = "image/card.png";
	LoadDivGraph(cardpng.c_str(), 9, 1, 9, 126, 180, _cardPNG);

	_clickFlg = false;
	_cursorPos = VECTOR2(0, 0);

	// �{�b�N�X�ʒu�̌���
	for (int i = 0; i <= 4; i++)
	{
		_setCardPos[i] = 140 * (i + 1);
		// �L�[�����݂��Ȃ��Ƃ��ɍ\�z����
		_cardMap.try_emplace(_setCardPos[i], CARD_MEMBER::NON);
	}

	for (auto member : CARD_MEMBER())
	{
		_cardPngSel_pair[static_cast<int>(member) - 1] = std::make_pair(_cardPNG[static_cast<int>(member) - 1], member);
	}

	for (int i = 0; i <= 4; i++)
	{
		card[i].pos = VECTOR2(_setCardPos[i] - 140 / 2, 500);
		//0 3 6�̃J�[�h�݂̂�������
		int rand = GetRand(8);
		int userand = rand % 3 * 3;
		card[i].pngSel = _cardPngSel_pair[userand].first;
		card[i].syurui = _cardPngSel_pair[userand].second;
		card[i].num = i;
		// �J�[�h���Z�b�g����Ƃ���Ɏ�ʏ�������
		_cardMap[_setCardPos[i]] = card[i].syurui;
	}

	_id = -1;
	return true;
}

void Cards::Draw(Player* player, Menu* menu)
{
	//ClsDrawScreen();

	for (int i = 0; i <= 4; i++)
	{
		// �����ŃJ�[�h�����ׂ�Draw���Ă��܂��ƁA
		// �N���b�N�����J�[�h���E���̃J�[�h��肵���ɕ`�悳��Ă��܂��̂ł킯��
		//Draw(card[i].pos, card[i].pngSel);
		if (_id >= 0)
		{
			// ���N���b�N���ď������Ă���J�[�h������Ƃ�
			// ���N���b�N����Ă���J�[�h�ȊO�̕`��
			if (_id != i)
			{
				// �����J�[�h�̎�ނ��قȂ�Ƃ�(�����\��)
				if (card[i].syurui != card[_id].syurui)
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
		else
		{
			// �ʏ�͂�����Draw
			DrawRotaGraph(card[i].pos.x, card[i].pos.y, 1.0f, 0, card[i].pngSel, false);
		}
	}

	// �c��
	for (int i = 141; i < 702;)
	{
		DrawLine(i, 400, i, 600, GetColor(255, 0, 0), 2);
		i += 140;
	}

	// ����
	DrawLine(0, 400, 900, 400, GetColor(255, 0, 0), 2);

	// id�������Ă���Ƃ��͂��̃J�[�h����ԏ�ɂ���悤�ɂ�����Draw����
	// ���N���b�N����Ă���J�[�h�̕`��
	if (_id >= 0)
	{
		DrawRotaGraph(card[_id].pos.x, card[_id].pos.y, 1.0f, 0, card[_id].pngSel, false);
	}

	// �J�[�h���̕\��
	// GetHeal�Ƃ��̐����͂܂�usecard�̊֐��ɓ���O�Ȃ̂Ŏg���Ȃ�
	if (_id != -1)
	{
		if (card[_id].pos.y > 0 && card[_id].pos.y <= 300)
		{
			std::string moji;
			if (static_cast<int>(card[_id].syurui) == 1 || static_cast<int>(card[_id].syurui) == 2 || static_cast<int>(card[_id].syurui) == 3)
			{
				moji = "�U��";
				//auto a = (static_cast<int>(_cards->GetCardMem()) + (static_cast<int>(_cards->GetCardMem()) - 1)) * (static_cast<int>(_cards->GetCardMem()) + (static_cast<int>(_cards->GetCardMem()) - 1));
				DrawFormatString(card[_id].pos.x - 80, card[_id].pos.y + 90, GetColor(255, 255, 255), "%s:�G��%d�̃_���[�W", moji.c_str(), (static_cast<int>(card[_id].syurui) + (static_cast<int>(card[_id].syurui) - 1)) * (static_cast<int>(card[_id].syurui) + (static_cast<int>(card[_id].syurui) - 1)) + player->GetAttackDamage() + menu->GetEquipDamage() + menu->GetPowUp());
			}
			if (static_cast<int>(card[_id].syurui) == 4 || static_cast<int>(card[_id].syurui) == 5 || static_cast<int>(card[_id].syurui) == 6)
			{
				// 1 3 5
				// 1 3 8
				// 4 5 6
				// 4 / 3 = 1 * 3 = 3 - 3 = 0
				// 5 / 3 = 1 * 3 = 3 - 3 = 0
				// 6 / 3 = 2 * 3 = 6 - 3 = 3
				//auto tes = ((static_cast<int>(card[_id].syurui) % 4) + 1) + (static_cast<int>(card[_id].syurui) % 4) + (static_cast<int>(card[_id].syurui) / 3 * 3 - 3);
				moji = "�h��";
				DrawFormatString(card[_id].pos.x - 80, card[_id].pos.y + 90, GetColor(255, 255, 255), "%s:�_���[�W%d���̌y��", moji.c_str(), ((static_cast<int>(card[_id].syurui) % 4) + 1) + (static_cast<int>(card[_id].syurui) % 4) + (static_cast<int>(card[_id].syurui) / 3 * 3 - 3));
			}
			if (static_cast<int>(card[_id].syurui) == 7 || static_cast<int>(card[_id].syurui) == 8 || static_cast<int>(card[_id].syurui) == 9)
			{
				// 1 3 5
				// 1 3 8
				// 7 8 9
				// 7 / 3 = 2 * 3 = 6 - 6 = 0
				// 8 / 3 = 2 * 3 = 6 - 6 = 0
				// 9 / 3 = 3 * 3 = 9 - 6 = 3
				//auto tes = ((static_cast<int>(card[_id].syurui) % 7) + 1) + (static_cast<int>(card[_id].syurui) % 7) + (static_cast<int>(card[_id].syurui) / 3 * 3 - 6);
				moji = "��";
				DrawFormatString(card[_id].pos.x - 80, card[_id].pos.y + 90, GetColor(255, 255, 255), "%s:�̗�%d���̉�", moji.c_str(), ((static_cast<int>(card[_id].syurui) % 7) + 1) + (static_cast<int>(card[_id].syurui) % 7) + (static_cast<int>(card[_id].syurui) / 3 * 3 - 6));
			}
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
	else if (card[id].pos.x > 700)
	{
		card[id].pos.x = 700;
	}

	// ��������ɂ��������D���g���������s��
	if (card[id].pos.y <= 300)
	{
		// ���N���b�N��������
		if (_clickFlg && !(_mouse & MOUSE_INPUT_LEFT))
		{
			UseCard(id);
			// ���̈ʒu�ɖ߂�(�߂��Ȃ�������J�[�h�������Ǝg�p������ԂɂȂ邩��ʒu��߂��Ȃ��Ƃ����Ȃ�)
			card[id].pos.x = _setCardPos[card[id].num] - 140 / 2;
			card[id].pos.y = 500;
			_clickFlg = false;
		}
	}

	if (card[id].pos.y > 300)
	{
		// ���N���b�N��������
		if (_clickFlg && !(_mouse & MOUSE_INPUT_LEFT))
		{
			// ���[
			if (card[id].pos.x >= 0 && card[id].pos.x <= _setCardPos[0])
			{
				// ���[�ɉ��������Ă��Ȃ��Ƃ�
				if (_cardMap[_setCardPos[0]] == CARD_MEMBER::NON)
				{
					// num������������O��non�ɂ���
					_cardMap[_setCardPos[card[id].num]] = CARD_MEMBER::NON;

					// �ړ������Ă���
					card[id].pos.x = _setCardPos[0] - 140 / 2;
					card[id].pos.y = 500;
					card[id].num = 0;

					// �V�����ړ������ꏊ�ɃJ�[�h�̎�ނ�o�^
					_cardMap[_setCardPos[0]] = card[id].syurui;
				}
				else if (_cardMap[_setCardPos[0]] == card[id].syurui && card[id].num != 0)
				{
					// 3�ȏ�̃J�[�h�����Ȃ��悤�ɂ���
					if (card[id].syurui == CARD_MEMBER::ATTACK_3 || card[id].syurui == CARD_MEMBER::GUARD_3 || card[id].syurui == CARD_MEMBER::HEAL_3)
					{
						// ���̈ʒu�ɖ߂�
						card[id].pos.x = _setCardPos[card[id].num] - 140 / 2;
						card[id].pos.y = 500;
					}
					else
					{
						// �����J�[�h�̎�ނŏd�˂鎞�ɍs������
						PlaySoundMem(_se, DX_PLAYTYPE_BACK, true);

						card[id].pos.x = _setCardPos[card[id].num] - 140 / 2;
						card[id].pos.y = 500;

						card[0].pngSel = _cardPngSel_pair[static_cast<int>(card[id].syurui)].first;
						card[0].syurui = _cardPngSel_pair[static_cast<int>(card[id].syurui)].second;
						_cardMap[_setCardPos[0]] = card[0].syurui;

						// ���������̃X�y�[�X�ɐV�����J�[�h�̐������s��
						//card[id] = card[card[id].num];
						// ���ɏo��J�[�h�̓����_���ɂ���
						//0 3 6�̃J�[�h�݂̂�������
						int rand = GetRand(8);
						int userand = rand % 3 * 3;
						card[id].pngSel = _cardPngSel_pair[userand].first;
						card[id].syurui = _cardPngSel_pair[userand].second;
						_cardMap[_setCardPos[id]] = card[id].syurui;
						_turnNum -= 1;
					}
				}
				else
				{
					// �ړ������邱�Ƃ��ł��Ȃ��Ƃ��Ƃ��Ɍ��̈ʒu�ɖ߂�
					card[id].pos.x = _setCardPos[card[id].num] - 140 / 2;
					card[id].pos.y = 500;
				}
				_clickFlg = false;
			}

			for (int i = 0; i <= 3; i++)
			{
				if (card[id].pos.x > _setCardPos[i] && card[id].pos.x <= _setCardPos[i + 1])
				{
					if (_cardMap[_setCardPos[i + 1]] == CARD_MEMBER::NON)
					{
						// num������������O��non�ɂ���
						_cardMap[_setCardPos[card[id].num]] = CARD_MEMBER::NON;

						// �ړ������Ă���
						card[id].pos.x = _setCardPos[i + 1] - 140 / 2;
						card[id].pos.y = 500;
						card[id].num = i + 1;

						// �V�����ړ������ꏊ�ɃJ�[�h�̎�ނ�o�^
						_cardMap[_setCardPos[i + 1]] = card[id].syurui;
					}
					else if (_cardMap[_setCardPos[i + 1]] == card[id].syurui && card[id].num != i + 1)
					{
						// 3�ȏ�̃J�[�h�����Ȃ��悤�ɂ���
						if (card[id].syurui == CARD_MEMBER::ATTACK_3 || card[id].syurui == CARD_MEMBER::GUARD_3 || card[id].syurui == CARD_MEMBER::HEAL_3)
						{
							// ���̈ʒu�ɖ߂�
							card[id].pos.x = _setCardPos[card[id].num] - 140 / 2;
							card[id].pos.y = 500;
						}
						else
						{
							// �����J�[�h�̎�ނŏd�˂鎞�ɍs������
							PlaySoundMem(_se, DX_PLAYTYPE_BACK, true);

							card[id].pos.x = _setCardPos[card[id].num] - 140 / 2;
							card[id].pos.y = 500;

							// _cardPngSel_pair��non����n�܂�킯�ł͂Ȃ��̂ŁA+1���Ȃ��Ă���
							card[i + 1].pngSel = _cardPngSel_pair[static_cast<int>(card[id].syurui)].first;
							card[i + 1].syurui = _cardPngSel_pair[static_cast<int>(card[id].syurui)].second;
							_cardMap[_setCardPos[i + 1]] = card[i + 1].syurui;

							// ���������̃X�y�[�X�ɐV�����J�[�h�̐������s��
							//card[id] = card[card[id].num];
							// ���ɏo��J�[�h�̓����_���ɂ���
							//0 3 6�̃J�[�h�݂̂�������
							int rand = GetRand(8);
							int userand = rand % 3 * 3;
							card[id].pngSel = _cardPngSel_pair[userand].first;
							card[id].syurui = _cardPngSel_pair[userand].second;
							_cardMap[_setCardPos[id]] = card[id].syurui;
							_turnNum -= 1;
						}
					}
					else
					{
						// ���̈ʒu�ɖ߂�
						card[id].pos.x = _setCardPos[card[id].num] - 140 / 2;
						card[id].pos.y = 500;
					}
					_clickFlg = false;
				}
			}
		}
	}
}

void Cards::UseCard(int id)
{
	// �g�p�����J�[�h��attack�̂Ƃ�
	if (card[id].syurui == CARD_MEMBER::ATTACK_1 || card[id].syurui == CARD_MEMBER::ATTACK_2 || card[id].syurui == CARD_MEMBER::ATTACK_3)
	{
		_cardsSyurui = CARDS_SYURUI::ATTACK;

		// �G��HP�����炷����
		// �_���[�W�́A�J�[�h�̑傫��+�v���C���[�̊�b�U���� +����̍U���͂�-2�`+2�̕ϓ��l������Ƃ���������������Ȃ�
		//_damageNum = static_cast<int>(card[id].syurui) * 2;
		if (card[id].syurui == CARD_MEMBER::ATTACK_1)
		{
			//1
			_damageNum = 1*1;
		}
		else if (card[id].syurui == CARD_MEMBER::ATTACK_2)
		{
			//9
			_damageNum = 3*3;
		}
		else
		{
			//25
			_damageNum = 5*5;
		}

		// �G�̑ҋ@�^�[�� -1
		_turnNum -= 1;

		// ���������̃X�y�[�X�ɐV�����J�[�h�̐������s��
		// ���ɏo��J�[�h�̓����_���ɂ���
		// 0 3 6�̃J�[�h�݂̂�������
		int rand = GetRand(8);
		int userand = rand % 3 * 3;
		card[id].pngSel = _cardPngSel_pair[userand].first;
		card[id].syurui = _cardPngSel_pair[userand].second;
		_cardMap[_setCardPos[id]] = card[id].syurui;
		return;
	}

	// �g�p�����J�[�h��guard�̂Ƃ�
	if (card[id].syurui == CARD_MEMBER::GUARD_1 || card[id].syurui == CARD_MEMBER::GUARD_2 || card[id].syurui == CARD_MEMBER::GUARD_3)
	{
		_cardsSyurui = CARDS_SYURUI::GUARD;

		// �G�̑ҋ@�^�[�� -1
		_turnNum -= 1;

		// �󂯂�U����1/3�Ƃ��ŃJ�b�g�ł���悤�ɂ���
		if (card[id].syurui == CARD_MEMBER::GUARD_1)
		{
			_gaurdNum = 1;
		}
		else if(card[id].syurui == CARD_MEMBER::GUARD_2)
		{
			_gaurdNum = 3;
		}
		else
		{
			_gaurdNum = 8;
		}

		// ������̃^�[���œ������ʂ̃J�[�h���g�p����Ƃ��ɍ��v���K�v�ɂȂ�
		_sumGaurdNum += _gaurdNum;

		// �g�����J�[�h�̌��ʂ��o���āA�Ⴄ�J�[�h���g�p�����ꏊ�ɏo��

		// ���������̃X�y�[�X�ɐV�����J�[�h�̐������s��
		// ���ɏo��J�[�h�̓����_���ɂ���
		// 0 3 6�̃J�[�h�݂̂�������
		int rand = GetRand(8);
		int userand = rand % 3 * 3;
		card[id].pngSel = _cardPngSel_pair[userand].first;
		card[id].syurui = _cardPngSel_pair[userand].second;
		_cardMap[_setCardPos[id]] = card[id].syurui;
		return;
	}

	// �g�p�����J�[�h��heal�̂Ƃ�
	if (card[id].syurui == CARD_MEMBER::HEAL_1 || card[id].syurui == CARD_MEMBER::HEAL_2 || card[id].syurui == CARD_MEMBER::HEAL_3)
	{
		_cardsSyurui = CARDS_SYURUI::HEAL;

		// �G�̑ҋ@�^�[�� -1
		_turnNum -= 1;

		// �v���C���[�̍ő�HP�̉����ŉ񕜂���
		if (card[id].syurui == CARD_MEMBER::HEAL_1)
		{
			_healNum = 1;
		}
		else if(card[id].syurui == CARD_MEMBER::HEAL_2)
		{
			_healNum = 3;
		}
		else
		{
			_healNum = 8;
		}

		// �g�����J�[�h�̌��ʂ��o���āA�Ⴄ�J�[�h���g�p�����ꏊ�ɏo��

		// ���������̃X�y�[�X�ɐV�����J�[�h�̐������s��
		// ���ɏo��J�[�h�̓����_���ɂ���
		// 0 3 6�̃J�[�h�݂̂�������
		int rand = GetRand(8);
		int userand = rand % 3 * 3;
		card[id].pngSel = _cardPngSel_pair[userand].first;
		card[id].syurui = _cardPngSel_pair[userand].second;
		_cardMap[_setCardPos[id]] = card[id].syurui;
		return;
	}
}