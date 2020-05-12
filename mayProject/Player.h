#pragma once

// �v���C���[�̏��
enum class CONDITION {
	FINE,	// ���N
	POISON	// ��
};

class Player
{
public:
	Player();
	~Player();
	void SetHP(int hpNum);				// ���݂̗̑͂�ݒ肷��
	int GetHP(void);					// ���݂̗̑͂��擾����
	void SetMaxHP(int hpNum);			// �ő�̗͂�ݒ肷��(�̗͑����܂ŕK�v)
	int GetMaxHP(void);					// �ő�̗͂��擾����
	float GetHPBar(void);				// �̗̓o�[�p�̒l�v�Z
	int GetAttackDamage(void);			// �U���͂��擾����
	void SetDifense(int num);			// �h��͂�ݒ肷��(�C�x���g�ŕK�v)
	int GetDifense(void);				// �h��͂��擾����
	void SetNextLevel(int num);			// ���̃��x���܂łɕK�v�Ȍo���l��ݒ肷��
	int GetNextLevel(void);				// ���̃��x���܂łɕK�v�Ȍo���l���擾����
	int GetNowLevel(void);				// ���݂̃��x�����擾����
	void SetMoney(int num);				// ��������ݒ肷��
	int GetMoney(void);					// ���݂̏��������擾����
	void SetCondition(CONDITION con);	// ���݂̏�Ԃ�ݒ肷��
	CONDITION GetCondition(void);		// ���݂̏�Ԃ��擾����
	void SetConditionTurn(int num);		// ��Ԉُ킩��̕��A���Ԃ�ݒ肷��
	int GetConditionTurn(void);			// ��Ԉُ킩��̕��A���Ԃ��擾����
private:
	//int _plHP;						// ���݂̗̑�
	//int _conditionTurnNum;				// ��Ԉُ�̎���
	//int _nowNum;						// ���̃v���C���[�X�e�[�^�X�̔ԍ�(���x���A�b�v�ōX�V����)
	//int _money;						// ������(�G��|���Ǝ�ɓ���A�����w������ƌ���)
};