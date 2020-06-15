#pragma once
#include <map>
#include <string>
#include "VECTOR2.h"
#include "ITEMCLASS.h"

// ���j���[��ʂ̍���
enum class MENU {
	NON,
	STATUS,
	ITEM,
	TO_GAME,
	TO_TITLE,
	SAVE,
	MAX
};

class GameScene;
class Player;
class Item;
class Monster;
class Cards;

class Menu
{
public:
	Menu();
	~Menu();

	void Update(GameScene* game,Player* player, Monster* monster, Cards* cards);	// �A�b�v�f�[�g�֐�
	void Draw(Player* player, Item* item, Monster* monster);		// �`��
	void MenuButton_NonEnemy(void);			// ���j���[�{�^������(��퓬��)
	void MenuButton_Enemy(void);			// ���j���[�{�^������(�퓬��)

	void Setitem(const ITEM& item, const int& png);	// �擾�����A�C�e�����i�[����
	bool GetMenuFlg(void)const;				// ���j���[��ʂ��\����Ԃ��擾����
	MENU GetMenu(void)const;				// ���݂̃��j���[���ڂ��擾����
	int GetCanHaveItem(void)const;			// �㉽�A�C�e�������Ă邩���ׂ�

	int GetEquipDamage(void)const;			// ��������ɂ��_���[�W�̑����ʂ��擾����
	int GetEquipGuard(void)const;			// �����h��ɂ��_���[�W�̌y���ʂ��擾����

	int GetPowUp(void)const;				// �p���[�A�b�v�A�C�e���̌��ʂ��擾����
	void SetPowUp(const int& num);			// �U�������-1���������Ă����Ƃ��Ɏg��

	bool GetNonDamageFlg(void)const;		// ���G���ʂ̃A�C�e�����g�p��Ԃ��擾����
	void SetNonDamageFlg(const bool& flag);	// �G����̍U�����󂯂���ɖ��G���ʂ̏�Ԃ������Ƃ��Ɏg��

	bool GetMenuBackPngFlg(void)const;		// �A�C�e���w�i���`�悳��Ă��邩�擾����

	bool GetEscapeFlg(void)const;			// �퓬���瓦����A�C�e���̏����擾����		
	void SetEscapeFlg(const bool& flag);	// �퓬���瓦����A�C�e���̏���ݒ肷��

	bool GetMeganeFlg(void)const;			// �󔠂̒��g���Ӓ肷��A�C�e���̏����擾����
	void SetMeganeFlg(const bool& flag);	// �󔠂̒��g���Ӓ肷��A�C�e���̏���ݒ肷��

	void Save(Player* player);			// �Z�[�u
	static void Load(void);				// ���[�h
private:
	void Init(void);					// ������
	void PngInit(void);					// �摜�֌W������

	/*�}�E�X�֌W*/
	VECTOR2 _cursorPos;					// �}�E�X�J�[�\���̍��W�ۑ��p�ϐ�
	int _mouse;							// �}�E�X�̓��͏�Ԏ擾

	MENU _menu;							// ���j���[���ڂ̕ۑ��p�ϐ�
	ITEM _itemAction;					// �A�C�e���g�p���̌��ʂ𕪂���Ƃ��Ɏg��
	ITEM _itemSetumei;					// ���j���[�Ő������o���Ƃ��Ɏg��
	VECTOR2 _equipSwordPos;				// ���������"E"��t����Ƃ��Ɏg��
	VECTOR2 _equipShieldPos;			// �����h���"E"��t����Ƃ��Ɏg��
	VECTOR2 _choicePos;					// �I�𒆂̃A�C�e���ɐF�g��\�����邽�߂Ɏg��(�킩��₷�����邽��)

	/*���j���[�֌W*/
	int _chooseNum;						// �I�𒆂̃A�C�e���̏ꏊ�ԍ�				
	bool _menuBackPngFlg;				// ���j���[��ʂ̔w�i�摜��\�����邩�̃t���O
	bool _menuSelPngFlg;				// ���j���[���ڂ�\�����邩�̃t���O
	// ���j���[����
	std::pair<VECTOR2, MENU>menu_pair[static_cast<int>(MENU::MAX)];
	VECTOR2 buttonSize[static_cast<int>(MENU::MAX)];	// ���j���[���ڃ{�^���T�C�Y

	/*�A�C�e���֌W*/
	int _powUpNum;						// �ꎞ�I�ɍU���͂��㏸����A�C�e���̌��ʗp
	int _equipDamage;					// ����ɂ��U���͂̏㏸
	int _equipGuard;					// ���ɂ��h��͂̏㏸
	bool _useOrThrowAway;				// �A�C�e���I������true
	bool _nonNeedFlg;					// �A�C�e�����g���Ă����ʂ��Ȃ��Ƃ���true
	bool _nonDamageFlg;					// ���G���ʂ̃A�C�e���p�t���O
	bool _escapeFlg;					// �����̃A�C�e���p�t���O
	bool _meganeFlg;					// �󔠂̒��g���Ӓ肷��A�C�e���p�t���O

	/*�摜�֌W*/
	// ���j���[�摜
	int _menuSelPNG[3];
	std::map<std::string, int> menuImages;

	int _soundSE[5];
	static bool loadFlg;
};