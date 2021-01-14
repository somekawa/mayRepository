#pragma once
#include <map>
#include <string>

// �v���C���[�̏��
enum class CONDITION {
	FINE,	// ���N
	POISON	// ��
};

enum class SKILL {
	NON,
	SWORD,
	GUARD,
	HEAL
};

class Monster;
class Menu;
class GameScene;
class Cards;
class MouseCtl;

class Player
{
public:
	Player();
	~Player();
	void ClickUpDate(Monster* monster, Menu* menu, GameScene* game, Cards* cards);	// �N���b�N���݂̂̃A�b�v�f�[�g�֐�
	void UpDate(void);						// �ʏ�A�b�v�f�[�g�֐�
	void Draw(Menu* menu);					// �`��
	void BattleDraw(Menu* menu);			// �퓬���̂ݕK�v�ȉ摜�̕`��
	void SkillDraw(void);					// �X�L���`��
	void SetHP(const int& hpNum);			// ���݂̗̑͂�ݒ肷��
	int GetHP(void)const;					// ���݂̗̑͂��擾����
	void SetMaxHP(const int& hpNum);		// �ő�̗͂�ݒ肷��(�̗͑����܂ŕK�v)
	int GetMaxHP(void)const;				// �ő�̗͂��擾����
	float GetHPBar(void)const;				// �̗̓o�[�p�̒l�v�Z
	int GetAttackDamage(void)const;			// �U���͂��擾����
	void SetDifense(const int& num);		// �h��͂�ݒ肷��(�C�x���g�ŕK�v)
	int GetDifense(void)const;				// �h��͂��擾����
	void SetNextLevel(const int& num);		// ���̃��x���܂łɕK�v�Ȍo���l��ݒ肷��
	int GetNextLevel(void)const;			// ���̃��x���܂łɕK�v�Ȍo���l���擾����
	int GetNowLevel(void)const;				// ���݂̃��x�����擾����
	void SetMoney(const int& num);			// ��������ݒ肷��
	int GetMoney(void)const;				// ���݂̏��������擾����
	void SetCondition(const CONDITION& con);// ���݂̏�Ԃ�ݒ肷��
	CONDITION GetCondition(void)const;		// ���݂̏�Ԃ��擾����
	void SetConditionTurn(const int& num);	// ��Ԉُ킩��̕��A���Ԃ�ݒ肷��
	int GetConditionTurn(void)const;		// ��Ԉُ킩��̕��A���Ԃ��擾����
	void SetSkillCharge(const int& num);	// �X�L���`���[�W���Ԃ����炷
	int GetSkillCharge(void)const;			// �X�L���`���[�W���Ԃ��擾
	bool GetSkillBackFlg(void)const;		// �X�L���I����ʂ��\������Ă��邩���擾����
	void SetBarrierNum(const int& num);		// �X�L���̃o���A�l��ݒ�
	int GetBarrierNum(void)const;			// �X�L���̃o���A�l���擾
	void SetLevelUpAnounceFlg(const bool& flag);// ���x�����オ�������Ƃ�ʒm����t���O��ݒ肷��
	bool GetLevelUpAnounceFlg(void)const;	 // ���x�����オ�������Ƃ�ʒm����t���O���擾����

	static int saveData[9];				// ���[�h�Ŏn�߂�Ƃ��ɓǂݍ��񂾃f�[�^��ۑ�����z��
	static bool _loadFlg;				// ���[�h�Ŏn�߂�Ƃ���init�ŃX�e�[�^�X��ǂݍ��܂Ȃ��悤�ɂ���Ƃ��K�v
private:
	void Init(void);					// ������
	void PngInit(void);					// �摜������

	bool _levelUpAnounceFlg;			// ���x�����オ�����Ƃ���true�ɂȂ�

	// �X�L���֌W
	int _skillCharge;					// �X�L���`���[�W
	bool _skillFlg;						// �X�L�����g�p�\�ɂȂ�����true
	bool _skillBackFlg;					// �X�L���A�C�R��������true
	int _pngLight;						// �A�C�R���̖��邳�����p�ϐ�	
	bool _lightFlg;						// �A�C�R���̖��邳�����p�t���O
	bool _seSkillOnceFlg;				// �X�L�������\���Ɉ�x����SE��炷�Ƃ��Ɏg�p����

	// �o���A�֌W
	int _barrierMaxNum;					// �o���A�̍ő�l
	int _barrierNum;					// �o���A�̒l(����l*�v���C���[���x��)

	// �X�L���摜�֌W
	std::map<std::string, int> skillImages;
	std::map<std::string, int> _drawHandle;

	// �A�j���[�V�����֌W
	SKILL _skill = SKILL::NON;
	int _skillAnimSword[12];
	int _skillAnimGuard[10];
	int _skillAnimHeal[10];
	int _animCnt;						// �A�j���[�V�����J�E���^
	int _animUpDateSpeedCnt;			// �A�j���[�V�����X�V���x�����J�E���^

	int _soundSE[6];					// SE

	MouseCtl* mouse;
};