#pragma once
#include <map>
#include <array>
#include "CARD_MEMBER.h"
#include "Player.h"
#include "Menu.h"
#include "VECTOR2.h"

class Cards
{
public:
	Cards();
	~Cards();
	void Update(void);						// �A�b�v�f�[�g�֐�
	void Draw(Player* player,Menu* menu);	// �`��
	void SetTurn(int turn);					// �G�̍U���܂łɓ������Ă����񐔂̐ݒ�
	int GetTurn(void);						// �c��ړ��񐔂̎擾
	void SetDamage(int damage);				// �v���C���[�̍U����A0�ɖ߂��̂ɕK�v
	int GetDamage(void);					// �v���C���[�̍U���͂�+����J�[�h�̈З͎擾�p�֐�
	void SetGuard(int guard);				// �v���C���[�̖h���A0�ɖ߂��̂ɕK�v
	int GetGuard(void);						// �v���C���[�̖h��͂�+����J�[�h�̈З͎擾�p�֐�
	void SetHeal(int heal);					// �v���C���[�̉񕜌�A0�ɖ߂��̂ɕK�v
	int GetHeal(void);						// �v���C���[�̍ő�HP�ɑ΂��ĉ����񕜂��邩�̐��l���擾����֐�
	CARDS_SYURUI GetCardsSyurui(void);		// �g�p�����J�[�h�̎�ނ��擾����
	void SetCardsSyurui(CARDS_SYURUI cards);// �g�p���NON�ɖ߂��Ƃ��Ɏg��
private:
	bool Init(void);						// ������
	void Move(int id);						// �J�[�h�̓���(�茳�őg�ݍ��킹��)
	void UseCard(int id);					// �J�[�h��G�⎩���ɑ΂��Ďg�p����

	// �}�E�X�֌W
	int _mouse;								// �}�E�X�̏��
	VECTOR2 _cursorPos;						// �}�E�X�J�[�\���̍��W�ۑ��p�ϐ�

	VECTOR2 _pngPos;						// �摜�̍��W�ۑ��p�ϐ�
	bool _clickFlg;							// �}�E�X�����N���b�N�������Ƃ𔻒肷��t���O(��������Ɏg��)
	int _setCardPos[5];						// �J�[�h���u�����ł��낤�ꏊ�̕ۑ��p�z��(X���W�ōl����)
	int _cardPNG[static_cast<int>(CARD_MEMBER::MAX)-1];	// �J�[�h�摜
	int _id;								// �J�[�h��id

	int _turnNum;							// �J�[�h�𓮂�������-1
	int _damageNum;							// �J�[�h�̍U���͕ۑ��p�ϐ�
	int _gaurdNum;							// �J�[�h�̖h��͕ۑ��p�ϐ�
	int _sumGaurdNum;						// �h��J�[�h�A���g�p���ɖh��͂�+���Ă������߂̕ϐ�
	int _healNum;							// �J�[�h�̉񕜗͕ۑ��p�ϐ�

	CARDS_SYURUI _cardsSyurui;				// �g�p�����J�[�h�̎�ޏ���ۑ�����

	// �J�[�h�̒u���ꏊ(_setCardPos[5])���L�[�ACARD_MODE��l�Ƃ��Ĉ����A�z�z��
	// ����ŁA�m�肽���ꏊ���w�肷�邱�Ƃɂ���ăJ�[�h�̎�ނ�m�邱�Ƃ��ł���B�����B����Ȃ��B
	std::map<int, CARD_MEMBER> _cardMap;

	// CARD_MODE��_cardPNG���Z�b�g�ōl������
	// CARD_MODE�����ɁA�摜�����߂���悤�ɂ�����
	// �@CARD_MODE�����܂�
	// �ACARD_MODE�ɍ������摜��pngSel�ɓ������
	// �BpngSel��������Draw�����
	std::pair<int, CARD_MEMBER>_cardPngSel_pair[static_cast<int>(CARD_MEMBER::MAX)];

	int _se;								// �d�˂��Ƃ���SE
};