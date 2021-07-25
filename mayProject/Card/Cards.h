#pragma once
#include <map>
#include <array>
#include <memory>
#include "CARD_MEMBER.h"
#include "../Player.h"
#include "../Menu.h"
#include "../Common/VECTOR2.h"

// �J�[�h�̈З͌v�Z��
#define NUM_CAL(X) 3 * 3 - (static_cast<int>(X) - 1)

class Cards
{
public:
	Cards();
	~Cards();
	void Update(void);						
	void Draw(const std::shared_ptr<Player>& player, const std::shared_ptr<Menu>& menu);	
	void SetTurn(const int& turn);			// �G�̍U���܂łɓ������Ă����񐔂̐ݒ�
	int  GetTurn(void)const;				// �c��ړ��񐔂̎擾
	void SetDamage(const int& damage);		// �v���C���[�̍U����A0�ɖ߂��̂ɕK�v
	int  GetDamage(void)const;				// �v���C���[�̍U���͂�+����J�[�h�̈З͎擾�p�֐�
	void SetGuard(const int& guard);		// �v���C���[�̖h���A0�ɖ߂��̂ɕK�v
	int  GetGuard(void) const;				// �v���C���[�̖h��͂�+����J�[�h�̈З͎擾�p�֐�
	void SetHeal(const int& heal);			// �v���C���[�̉񕜌�A0�ɖ߂��̂ɕK�v
	int  GetHeal(void)  const;				// �v���C���[�̍ő�HP�ɑ΂��ĉ����񕜂��邩�̐��l���擾����֐�
	CARDS_TYPE GetCardsType(void) const;	// �g�p�����J�[�h�̎�ނ��擾����
	void SetCardsType(const CARDS_TYPE& cards);// �g�p���NON�ɖ߂��Ƃ��Ɏg��
private:
	bool Init(void);						
	void SetCardPos(int id,int posId);		// �J�[�h�̍��W�ݒ�
	void Move(int id);						// �J�[�h�̓���(�茳�őg�ݍ��킹��)
	void UseCard(int id);					// �J�[�h��G�⎩���ɑ΂��Ďg�p����
	void CreateCard(int id);				// �J�[�h�̐�����d�˂��ۂ̏������s��

	// �}�E�X�֌W
	int _mouse;								// �}�E�X�̏��
	VECTOR2 _cursorPos;						// �}�E�X�J�[�\���̍��W�ۑ��p�ϐ�

	VECTOR2 _pngPos;						// �摜�̍��W�ۑ��p�ϐ�
	bool _clickFlg;							// �}�E�X�����N���b�N�������Ƃ𔻒肷��t���O(��������Ɏg��)
	int  _setCardPos[5];					// �J�[�h���u�����ł��낤�ꏊ�̕ۑ��p�z��(X���W�ōl����)
	int  _cardPNG[static_cast<int>(CARD_MEMBER::MAX)-1];	// �J�[�h�摜
	int  _id;								// �J�[�h��id

	int _turnNum;							// �J�[�h�𓮂�������-1
	int _damageNum;							// �J�[�h�̍U���͕ۑ��p�ϐ�
	int _gaurdNum;							// �J�[�h�̖h��͕ۑ��p�ϐ�
	int _sumGaurdNum;						// �h��J�[�h�A���g�p���ɖh��͂����Z���Ă����ׂ̕ϐ�
	int _healNum;							// �J�[�h�̉񕜗͕ۑ��p�ϐ�

	CARDS_TYPE _cardsType;					// �g�p�����J�[�h�̎�ޏ���ۑ�����
	std::map<int, CARD_MEMBER> _cardMap;	// �J�[�h���W�̐ݒ�p
	std::map<CARD_MEMBER, int> _cardPower;	// �J�[�h�З͂̐ݒ�p
	std::pair<int, CARD_MEMBER>_cardPngSel_pair[static_cast<int>(CARD_MEMBER::MAX)];

	int _se;								// �d�˂��Ƃ���SE
};