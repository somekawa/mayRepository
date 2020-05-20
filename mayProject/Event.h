#pragma once
#include <iostream>
#include <utility>
#include <string>
#include "Menu.h"

class GameScene;
class Player;
class Menu;
class Item;
class Monster;

enum class EVENT_STATE {
	NON,
	ENEMY,		// �G
	YADO,		// �h��
	SYOUNIN,	// ���l
	BUTTON,		// �{�^��
	CHEST,		// ��
	DRINK,		// ���ݕ�
	TRAP,		// �����g���b�v
	MAX
};

class Event
{
public:
	Event();
	~Event();
	void UpDate(GameScene* game,Player* player, Menu* menu,Item* item, Monster* monster);
	void Draw(GameScene* game,Player* player, Menu* menu, Item* item);
	void SetEvent(EVENT_STATE state);	// �Q�[���V�[������C�x���g��ݒ肷��
	int GetNowEvent(void);				// ���݂̃C�x���g�ԍ����擾����
	void SetNowEvent(int num);			// �͂��߂����蒼���̂ɕK�v
	void SetFateNum(int num);			// �͂��߂����蒼���̂ɕK�v
	void SetReset(void);			// ���߂����蒼���Ƃ��ɕ󔠂̏�Ԃ����Z�b�g����
private:
	void Init(void);					// ������
	void pngInit(void);					// �摜�֌W������
	void Enemy(GameScene* game, Player* player, Monster* monster);
	void Yado(GameScene* game, Player* player);
	void Syounin(GameScene* game, Player* player, Menu* menu,Item* item);
	void Button(GameScene* game, Player* player);
	void Chest(GameScene* game, Player* player, Menu* menu, Item* item);
	void Drink(GameScene* game, Player* player);
	void Trap(GameScene* game, Player* player);

	EVENT_STATE _event;					// �C�x���g���p�ϐ�
	bool _healYadoFlg;					// �񕜂��h���ŗ��ނƂ���true
	bool _nonMoneyFlg;					// ������������Ȃ��Ƃ���true(�h���Ə��l�Ŏg�p)

	int _nowEvent;						// ���݂̃C�x���g

	bool _buyFlg;						// ������������Ƃ����Ƃ��ɃA�C�e����\�������邽�߂̃t���O
	bool _chooseFlg;					// �w������������true�ɂ���
	int _chooseNum;						// �I�𒆂̏��i�̔ԍ��ۑ��p(�ꏊ�Ƃ������킩��Ȃ�)
	ITEM _itemInfo;						// �A�C�e���̏��p

	// �{�^���E�󔠁E���ݕ��C�x���g�Ŏg�p
	bool _pushFlg;						// �{�^������������true�ɂ���
	int _fateNum;						// 0�̂Ƃ��͗ǂ����Ƃ��N����

	bool _getFlg;						// �A�C�e�����Ƃ�{�^��
	bool _anounceFlg;					// �A�C�e���������ς��Ƃ̂��m�点

	bool _itemNextPage = false;

	// �h���̉摜
	int _healHumanPNG;
	// ���l�̉摜
	int _syouninPNG;
	// ���b�Z�[�W�p�g�摜
	int _messagePNG;
	// ���l�̎������̌��g
	int _syouninWakuPNG;
	// �A�C�e���{�b�N�X�̍��g
	int _itemBoxPNG;
	// ���l�̑I�𒆂̃A�C�e�����킩��₷���Ȃ�摜
	int _itemChoicePNG;
	//�uSold Out�v�̕����摜
	int _soldOutPNG;
	// �󔠂̉摜
	int _chestPNG[2];
	// �󔠂���擾�ł���A�C�e���摜
	int _chsetItemPNG;
	// ��̕r�̉摜
	int _drinkPNG;
	// �I�����̕����摜
	int _sentakusiPNG[11];
	// ���̉摜
	int yajirusiPNG;
	// ��̕�
	int karaPNG;
	// �����g���b�v�̑�
	int zouPNG;

	// SE
	int _soundSE[6];
	int _seCnt;				// ���s������h�A���܂łȂ��Ƃ��Ɏg���ϐ�
	bool _soundWalk;		// ���s������h�A���܂łȂ��Ƃ��Ɏg���t���O

	int chestOpen[2];		// �J�������ǂ���
	int chestBingo[2];		// ���g��������
	VECTOR2 chestPos[2];	// �󔠂̈ʒu

	bool trapFlg = false;	// �����g���b�v�p�B��x�����������炻�̂܂܂ɂ��Ă���
	bool nowTrapFlg = false;// �������Ȃ����x�ȏ㎀�񂾏ꍇ
};