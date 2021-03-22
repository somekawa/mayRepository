#pragma once
#include <array>

using KEY_ARRAY = std::array<char, 256>; //	array���g�����L�[����

// �L�[�̏��
enum KEY_TYPE
{
	KEY_TYPE_NOW,	// ���݂̉������
	KEY_TYPE_OLD,	// 1�t���[���O�̉������
	KEY_TYPE_MAX
};

class GameCtl
{
public:
	GameCtl();
	~GameCtl();
	bool Update(void);								// ��Ԃ��X�V����
	const KEY_ARRAY &GetCtl(KEY_TYPE type) const;	// �L�[�̓��͏�Ԃ�Ԃ�(������const�͒��ɏ����Ă�����̑S��const����������Ƃ����Ӗ�)
private:
	KEY_ARRAY data;									// ���݂̏��
	KEY_ARRAY dataOld;								// 1�t���[���O�̏��
};