#pragma once

// �v���C���[�̏��
enum class CONDITION {
	FINE,	// ���N
	POISON	// ��
};

class Monster;
class Menu;
class GameScene;

class Player
{
public:
	Player();
	~Player();
	void ClickUpDate(Monster* monster, Menu* menu, GameScene* game);	// �N���b�N���݂̂̃A�b�v�f�[�g�֐�
	void UpDate(void);					// �ʏ�A�b�v�f�[�g�֐�
	void Draw(Menu* menu);				// �`��
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
	void SetSkillCharge(int num);		// �X�L���`���[�W���Ԃ����炷
	int GetSkillCharge(void);			// �X�L���`���[�W���Ԃ��擾
	bool GetSkillBackFlg(void);			// �X�L���I����ʂ��\������Ă��邩���擾����
	void SetBarrierNum(int num);		// �X�L���̃o���A�l��ݒ�
	int GetBarrierNum(void);			// �X�L���̃o���A�l���擾

	void SaveTest(void);
	static void LoadTest(void);

	static int saveTestNum[9];
	static bool loadFlg;				// ���[�h�Ŏn�߂�Ƃ���init�ŃX�e�[�^�X��ǂݍ��܂Ȃ��悤�ɂ���Ƃ��K�v

private:
	void Init(void);					// ������
	void pngInit(void);					// �摜������
	// �X�L���֌W
	int _skillCharge;					// �X�L���`���[�W
	bool _skillFlg;						// �X�L�����g�p�\�ɂȂ�����true
	bool _skillBackFlg;					// �X�L���A�C�R��������true
	int _skillIconPNG;					// �X�L���A�C�R��
	int _skillAnnouncePNG;				// �X�L���g�p�\�ē�
	int _skillBackPNG;					// �X�L���p�w�i�摜
	int _skillAttackIconPNG;			// �U���n�X�L���I��p�A�C�R��
	int _skillBarrierIconPNG;			// �h��n�X�L���I��p�A�C�R��
	int _skillHealIconPNG;				// �񕜌n�X�L���I��p�A�C�R��
	int _skillCancelPNG;				//�u��߂�v�̕����摜
	int _skillMuscleIconPNG;			// �͂��Ԃ̃A�C�R���摜
	int _pngLight;						// �A�C�R���̖��邳�����p�ϐ�	
	bool _lightFlg;						// �A�C�R���̖��邳�����p�t���O

	int _barrierMaxNum;					// �o���A�̍ő�l
	int _barrierNum;					// �o���A�̒l(����l*�v���C���[���x���̗\��)
	int _barrierBarBackPNG;				// �o���A�o�[�̔w�i�摜
	int _barrierBarPNG;					// �o���A�o�[�摜

	int _seLevelUp;
};