#pragma once

// �A�C�e���̎��
enum class ITEM {
	NON,
	POTION,
	DETOX,
	KYOUKA_POW,		// �ꎞ�I�ɍU���͏㏸(5�^�[���Ƃ�)
	HEART,			// �̗͑���
	SWORD,
	SWORD_LV2,		
	SWORD_LV3,
	SHIELD,
	SHIELD_LV2,	
	SHIELD_LV3,
	ENEMY_1,		// �h���b�v�A�C�e���͂�������(�Œ�_���[�W20)
	ENEMY_2,		// ���̍U���̂ݖ�����
	POTION_BIG,		// �󔠂݂̂���łĂ�����
	MAX
};

// enum�̏��Ԃ���ёւ�����csv�����ёւ���

ITEM begin(ITEM);
ITEM end(ITEM);
ITEM operator++(ITEM& item);
ITEM operator* (ITEM& item);