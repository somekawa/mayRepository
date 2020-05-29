#pragma once
#include "Monster.h"
#include "ITEMCLASS.h"

#define MONSTER_CNT 6

class Cards;

class Enemy_weak :
	public Monster
{
public:
	Enemy_weak();
	~Enemy_weak();
	bool Init(void);					// ������
	void pngInit(void);					// �摜������
	void update(void);					// �A�b�v�f�[�g�֐�
	void Draw(void);					// �`��(�G���G�ƃh���b�v�A�C�e��)
	void BossDraw(void);				// �G���G�ƕ`�悷��摜�̑傫�����Ⴄ�̂ŕʊ֐�
	void Damage(int damageNum,Cards* cards);	// �v���C���[����G�ւ̃_���[�W�p�֐�
	ENEMY_STATE GetEnemyState(void);	// ���݂̓G�̏�Ԃ��擾����֐�
	void SetEnemyState(ENEMY_STATE st);	// �C�x���g��Ԃ�NON�ɖ߂������Ƃ��Ɏg���֐�
	int GetAttack(void);				// �G�̍U���͂̎擾�p�֐�
	int GetMaxTurn(void);				// �ő�ҋ@�^�[�����̎擾�p�֐�
	void SetEnemyNum(int num, int plLv);// �G���J�E���g�����Ƃ��ɓG�̏���ݒ肷��(�v���C���[���x���ɍ��킹�ĕω�)
	float GetHPBar(void);				// HP�o�[�p�̒l�v�Z�p
	int GetKeikenti(void);				// �|���ꂽ�Ƃ��Ɍo���l���擾����֐�
	int GetMoney(void);					// �|���ꂽ�Ƃ��ɂ������擾����֐�
	bool GetDropFlg(void);				// �h���b�v�A�C�e�����擾����֐�
	void SetDropFlg(bool flag);			// �h���b�v�A�C�e���̐ݒ肷��֐�
	ITEM GetDrop(void);					// �h���b�v�A�C�e���̎�ނ��擾����֐�
	int GetEnemyNum(void);				// �G�̔ԍ����擾����֐�
private:
	int _enemyHP;						// ����HP
	int _enemyMaxHP;					// �ő�HP(�ʏ�̗̑�+�v���C���[���x���ł̕␳)
	int _enemyMaxTurn;					// �ő�ҋ@�^�[����
	int _enemyNum;						// �G�ԍ��ۑ��p
	int _attackDamage;					// �G�̍U���͕ۑ��p(�ʏ�̍U����+�v���C���[���x���ł̕␳)
	ENEMY_STATE _enemyState;			// ���݂̓G�̏�Ԑݒ�p

	int _bossFogCnt;					// �{�X�̃X���[�N�����p�ϐ�

	// �h���b�v�A�C�e���֌W
	bool _dropItem;						// �h���b�v�A�C�e���p�t���O
	int _dropItemNum;					// �h���b�v�A�C�e���̉摜�ԍ�
	ITEM _dropItemSyurui;				// �h���b�v�A�C�e���̎��

	int _se;							// SE

	// �摜�֌W
	int _enemyPNG[MONSTER_CNT];			// �G���摜
	int _bossPNG;						// �{�X�摜
	int _eneItemPNG[4];					// �h���b�v�A�C�e���摜
	int fogPNG;						// �{�X�p�X���[�N�摜
};