#pragma once

// �A�C�e���̎��
enum class ITEM {
	NON,
	POTION,			// ����
	DETOX,			// ��ō�
	POW_UP,			// �ꎞ�I�ɍU���͏㏸(5�^�[��)
	HEART,			// �̗͑���
	SKILL_FAST,		// �v���C���[�X�L�������܂ł̎��Ԃ𑁂߂邱�Ƃ��ł���
	GLASSES,		// �󔠂��g���b�v���ǂ����Ӓ�ł���
	SWORD,			// ����
	SWORD_LV2,		
	SWORD_LV3,
	SHIELD,			// �h��
	SHIELD_LV2,	
	SHIELD_LV3,
	ENEMY_1,		// �h���b�v�A�C�e���͂�������(�Œ�_���[�W20)
	ENEMY_2,		// ���̍U���̂ݖ�����
	ENEMY_3,		// �G�^�[�����ő�l�܂Ŋ����߂�
	ENEMY_4,		// �퓬����m���ɓ�������(����G�C�x���g�ƃ{�X�ȊO)
	POTION_BIG,		// �󔠂݂̂���łĂ�����
	MAX
};

// enum�̏��Ԃ���ёւ�����csv�����ёւ���

ITEM begin(ITEM);
ITEM end(ITEM);
ITEM operator++(ITEM& item);
ITEM operator* (ITEM& item);