#pragma once
#include <iostream>
#include <utility>
#include <map>
#include <string>
#include "Menu.h"

class GameScene;
class MouseCtl;
class Player;
class Menu;
class Item;
class Monster;
class Cards;

enum class EVENT_STATE {
	NON,
	ENEMY,		// �G
	YADO,		// �h��
	SYOUNIN,	// ���l
	BUTTON,		// �{�^��
	CHEST,		// ��
	DRINK,		// ���ݕ�
	TRAP,		// �����g���b�v
	EVE_MONS,	// �C�x���g�����X�^�[
	GOAL,		// �o��
	MAX
};

class Event
{
public:
	Event();
	~Event();
	void UpDate(GameScene* game,Player* player, Menu* menu,Item* item, Monster* monster,Cards* cards, MouseCtl* mouse);
	void Draw(GameScene* game,Player* player, Menu* menu, Item* item);
	void SetEvent(const EVENT_STATE& state);// �Q�[���V�[������C�x���g��ݒ肷��
	void SetFateNum(const int& num);		// �͂��߂����蒼���̂ɕK�v
	void SetReset(void);					// �͂��߂����蒼���Ƃ��ɃC�x���g�̏�Ԃ����Z�b�g����
	bool GetEventMonsFlg(void)const;		// ����G�C�x���g�Ő키��I���������擾����֐�
	void SetEventMonsFlg(const bool& flag);	// �͂��߂����蒼���̂ɕK�v
	bool GetEventMonsEndFlg(void)const;		// ����G�C�x���g���I�����������擾����֐�
	void SetEventMonsEncountFlg(const bool& flag);	// ����G�C�x���g�̑�����Ԃ�ݒ肷��֐�
	bool GetEventMonsEncountFlg(void)const;	// ����G�C�x���g�ɑ������������擾����֐�
	void SetCautionFlg(const bool& flag);	// �����퓬�̈ē���ݒ肷��֐�
	bool GetCautionFlg(void)const;			// �����퓬�̈ē����擾����֐�
private:
	void Init(void);						// ������
	void pngInit(void);						// �摜�֌W������
	void Enemy(GameScene* game, Player* player, Monster* monster);
	void Yado(GameScene* game, Player* player,MouseCtl* mouse);
	void Syounin(GameScene* game, Player* player, Menu* menu,Item* item, MouseCtl* mouse);
	void Button(GameScene* game, Player* player, MouseCtl* mouse);
	void Chest(GameScene* game, Player* player, Menu* menu, Item* item, MouseCtl* mouse);
	void Drink(GameScene* game, Player* player, MouseCtl* mouse);
	void Trap(GameScene* game, Player* player, MouseCtl* mouse);
	void eventMons(GameScene* game, Monster* monster, Cards* cards, MouseCtl* mouse);

	EVENT_STATE _event;					// �C�x���g���p�ϐ�

	// �h���E���l
	bool _healYadoFlg;					// �񕜂��h���ŗ��ނƂ���true
	bool _nonMoneyFlg;					// ������������Ȃ��Ƃ���true(�h���Ə��l�Ŏg�p)
	bool _buyFlg;						// ������������Ƃ����Ƃ��ɃA�C�e����\�������邽�߂̃t���O
	bool _chooseFlg;					// �w������������true�ɂ���
	int _chooseNum;						// �I�𒆂̏��i�̔ԍ��ۑ��p(�ꏊ�Ƃ������킩��Ȃ�)
	ITEM _itemInfo;						// �A�C�e���̏��p
	bool _itemNextPage;					// ���i�y�[�W���߂�������true�ɂȂ�

	// �{�^���E�󔠁E���ݕ��C�x���g�Ŏg�p
	bool _pushFlg;						// �C�x���g������true�ɂ���
	int _fateNum;						// �C�x���g�̓��e��ۑ�����ϐ�

	bool _getFlg;						// �A�C�e�����Ƃ�{�^��
	bool _anounceFlg;					// �A�C�e���������ς��̂Ƃ��ɂ��m�点����

	bool _onceFlg;						// �����퓬���ɓG����`�悷��Ƃ��Ɏg�p
	bool _kyouseiButtleFlg;				// �����퓬�ē��摜�̕`��Ɏg�p

	// �󔠊֘A
	int _chestOpen[4];					// �J�������ǂ���
	int _chestBingo[4];					// ���g�̓�����(1)/�͂���(0)
	VECTOR2 _chestPos[4];				// �󔠂̈ʒu

	// �{�^��
	VECTOR2 _buttonDrink[4];			// �{�^���ƃh�����N�̍��W
	int _buttonNum;						// ���ݒn�ƃC�x���g�ꏊ����v�����Ƃ��ɂ��̒l��ۑ�����ϐ�
	bool _buttonPush[2];				// ���������ǂ���
	bool _buttonEventFlg;				// �{�^����������true�ɂ���

	// ���ݕ�
	int _drinkNum;						// ���ݒn�ƃC�x���g�ꏊ����v�����Ƃ��ɂ��̒l��ۑ�����ϐ�
	bool _drinking[2];					// ���񂾂��ǂ���
	bool _drinkEventFlg;				// ���ނ��Ƃɂ����Ƃ���true�ɂ���

	// �����g���b�v
	bool _trapFlg;						// 1�x�����������炻�̂܂�true�ɂ��Ă���
	bool _nowTrapFlg;					// �������Ȃ���1�x�ȏ㎀�񂾏ꍇ��true

	// ����G
	bool _eventMonsFlg;					// �키��I������true			
	bool _eventMonsEncountFlg;			// ��������true
	bool _eventMonsEndFlg;				// �����g���b�v�g�p���ē|�����ۂ�true

	std::map<int, ITEM> _chestItemMap;

	// �C�x���g�摜
	std::map<std::string, int> eventImages;
	// ���b�Z�[�W�p�g�摜
	int _messagePNG;
	// ���l�̎������̌��g
	int _syouninWakuPNG;
	// �A�C�e���{�b�N�X�̍��g
	int _itemBoxPNG;
	// ���l�̑I�𒆂̃A�C�e�����킩��₷���Ȃ�摜
	int _itemChoicePNG;
	// [0]:�J���ĂȂ���,[1]:�󔠂���G�o��
	int _chestPNG[2];
	// �I�����̕����摜
	int _sentakusiPNG[12];
	// ���i�y�[�W���ړ����邽�߂̖��̉摜
	int _yajirusiPNG;
	// SE
	int _soundSE[4];
};