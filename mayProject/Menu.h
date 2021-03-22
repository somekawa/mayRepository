#pragma once
#include <map>
#include <string>
#include <memory>
#include "Common/VECTOR2.h"
#include "Item/ITEMCLASS.h"

class MouseCtl;

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
class MouseCtl;

class Menu
{
public:
	Menu();
	~Menu();

	void Update(GameScene* game, const std::shared_ptr<Player>& player, const std::shared_ptr<Monster>& monster, const std::shared_ptr<Cards>& cards,const std::shared_ptr<MouseCtl>& mouse);	
	void Draw(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item, const std::shared_ptr<Monster>& monster);		
	void MenuButton_NonEnemy(const std::shared_ptr<MouseCtl>& mouse);						// ���j���[�{�^������(��퓬��)
	void MenuButton_Enemy(const std::shared_ptr<MouseCtl>& mouse);							// ���j���[�{�^������(�퓬��)

	void SetItem(const ITEM& item, const int& png);	// �擾�����A�C�e�����i�[����
	bool GetMenuFlg(void)const;						// ���j���[��ʂ��\����Ԃ��擾����
	MENU GetMenu(void)const;						// ���݂̃��j���[���ڂ��擾����
	int GetCanHaveItem(void)const;					// �㉽�A�C�e�������Ă邩���ׂ�

	int GetEquipDamage(void)const;					// ��������ɂ��_���[�W�̑����ʂ��擾����
	int GetEquipGuard(void)const;					// �����h��ɂ��_���[�W�̌y���ʂ��擾����

	int GetPowUp(void)const;						// �p���[�A�b�v�A�C�e���̌��ʂ��擾����
	void SetPowUp(const int& num);					// �U�������-1���������Ă����Ƃ��Ɏg��

	bool GetNonDamageFlg(void)const;				// ���G���ʂ̃A�C�e�����g�p��Ԃ��擾����
	void SetNonDamageFlg(const bool& flag);			// �G����̍U�����󂯂���ɖ��G���ʂ̏�Ԃ������Ƃ��Ɏg��

	bool GetMenuBackPngFlg(void)const;				// �A�C�e���w�i���`�悳��Ă��邩�擾����

	bool GetEscapeFlg(void)const;					// �퓬���瓦����A�C�e���̏����擾����		
	void SetEscapeFlg(const bool& flag);			// �퓬���瓦����A�C�e���̏���ݒ肷��

	bool GetAppraisalFlg(void)const;				// �󔠂̒��g���Ӓ肷��A�C�e���̏����擾����
	void SetAppraisalFlg(const bool& flag);			// �󔠂̒��g���Ӓ肷��A�C�e���̏���ݒ肷��

	void Save(const std::shared_ptr<Player>& player);		// �Z�[�u
	static void Load(void);									// ���[�h
private:
	bool Init(void);								// ������
	void PngInit(void);								// �摜�֌W������

	MENU _menu;										// ���j���[���ڂ̕ۑ��p�ϐ�
	ITEM _itemAction;								// �A�C�e���g�p���̌��ʂ𕪂���Ƃ��Ɏg��
	ITEM _itemExplanation;							// ���j���[�Ő������o���Ƃ��Ɏg��
	VECTOR2 _equipSwordPos;							// ���������"E"��t����Ƃ��Ɏg��
	VECTOR2 _equipShieldPos;						// �����h���"E"��t����Ƃ��Ɏg��
	VECTOR2 _choicePos;								// �I�𒆂̃A�C�e���ɐF�g��\�����邽�߂Ɏg��(�킩��₷�����邽��)

	/*���j���[�֌W*/
	int _chooseNum;									// �I�𒆂̃A�C�e���̏ꏊ�ԍ�				
	bool _menuBackPngFlg;							// ���j���[��ʂ̔w�i�摜��\�����邩�̃t���O
	bool _menuSelPngFlg;							// ���j���[���ڂ�\�����邩�̃t���O
	std::pair<VECTOR2, MENU>menu_pair[static_cast<int>(MENU::MAX)];		// ���j���[����
	VECTOR2 buttonSize[static_cast<int>(MENU::MAX)];// ���j���[���ڃ{�^���T�C�Y

	/*�A�C�e���֌W*/
	int _powUpNum;									// �ꎞ�I�ɍU���͂��㏸����A�C�e���̌��ʗp
	int _equipDamage;								// ����ɂ��U���͂̏㏸
	int _equipGuard;								// ���ɂ��h��͂̏㏸
	bool _useOrThrowAway;							// �A�C�e���I������true
	bool _nonNeedFlg;								// �A�C�e�����g���Ă����ʂ��Ȃ��Ƃ���true
	bool _nonDamageFlg;								// ���G���ʂ̃A�C�e���p�t���O
	bool _escapeFlg;								// �����̃A�C�e���p�t���O
	bool _AppraisalFlg;								// �󔠂̒��g���Ӓ肷��A�C�e���p�t���O

	/*�摜�֌W*/
	int _menuSelPNG[3];
	std::map<std::string, int> _menuImages;

	int _soundSE[5];
	static bool _loadFlg;							// true���ɏ����A�C�e�����{�b�N�X�Ɋi�[���鏈�����s��
};