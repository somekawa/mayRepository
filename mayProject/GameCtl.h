#pragma once
#include <array>

using KEY_ARRAY = std::array<char, 256>; //	array���g�����L�[����

enum KEY_TYPE
{
	KEY_TYPE_NOW,
	KEY_TYPE_OLD,
	KEY_TYPE_MAX
};

class GameCtl
{
public:
	GameCtl();
	~GameCtl();
	bool Update(void);								// ��Ԃ��X�V����
	const KEY_ARRAY &GetCtl(KEY_TYPE type) const;	// �L�[�̓��͏�Ԃ�Ԃ�
	// std::array<char, 256>��typedef�ŏȗ���
	// ������const�͒��ɏ����Ă���̂����ׂ�const�Ƃ����Ӗ�
private:
	KEY_ARRAY data;									// ���݂̏��
	KEY_ARRAY dataOld;								// 1�t���[���O�̏��
};