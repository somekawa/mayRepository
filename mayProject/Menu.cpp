#include <DxLib.h>
#include <string>
#include "Menu.h"
#include "Player.h"
#include "Item.h"
#include "Monster.h"
#include "Cards.h"

struct item
{
	VECTOR2 pos;		// �{�b�N�X�̈ʒu
	ITEM _item;
	int png;			// �摜
}itemBox[12];

Menu::Menu()
{
	Init();
}

Menu::~Menu()
{
}

void Menu::Init(void)
{
	_menu = MENU::NON;
	_itemAction = ITEM::NON;
	_itemSetumei = ITEM::NON;

	// �����Ȃ��Ƃ���ɏo���Ƃ�
	_equipShieldPos = { -100,-1 };
	_equipSwordPos = { -100,-1 };
	_choicePos = { -100,-100 };

	/*���j���[�֌W*/
	_chooseNum = -1;
	_menuBackPngFlg = false;
	_menuSelPngFlg = false;

	// �X�e�[�^�X�E�A�C�e���E�Â���̃{�^���ʒu
	for (int i = 0; i <= 2; i++)
	{
		menu_pair[i].first = { 350,100 + (i * 100) };
		menu_pair[i].second = static_cast<MENU>(static_cast<int>(MENU::STATUS) + i);
		buttonSize[i] = { 200, 100 };
	}

	// �^�C�g���ւ̃{�^���ʒu
	menu_pair[3].first = { 300,425 };
	menu_pair[3].second = static_cast<MENU>(static_cast<int>(MENU::TO_TITLE));
	buttonSize[3] = { 140, 70 };

	// �Z�[�u�̃{�^���ʒu
	menu_pair[4].first = { 475,425 };
	menu_pair[4].second = static_cast<MENU>(static_cast<int>(MENU::SAVE));
	buttonSize[4] = { 140, 70 };

	/*�A�C�e���֌W*/
	_powUpNum = 0;
	_equipDamage = 0;
	_equipGuard = 0;
	_useOrThrowAway = false;
	_nonNeedFlg = false;
	_nonDamageFlg = false;

	// �A�C�e���{�b�N�X�̈ʒu�Ə����A�C�e���̏��
	for (int i = 0; i <= 11; i++)
	{
		itemBox[i].pos = { (((i % 3) + 3) * 100),((i / 3) + 1) * 100 - 30 };
		itemBox[i]._item = ITEM::NON;
	}

	pngInit();

	// SE
	_seClick = LoadSoundMem("sound/se/click.mp3");
}

void Menu::pngInit(void)
{
	// ���j���[�̃{�^��3��
	std::string menuButton = "image/menu/menuSel.png";
	LoadDivGraph(menuButton.c_str(), 3, 3, 1, 200, 100, _menuSelPNG);

	// �^�C�g���֖߂�{�^��
	std::string titleBackButton = "image/menuTitleBackButton.png";
	_menuTitleBackPNG = LoadGraph(titleBackButton.c_str());

	// �Z�[�u�{�^��
	std::string menuSave = "image/menuSave.png";
	_menuSavePNG = LoadGraph(menuSave.c_str());

	std::string menuback = "image/menu_window.png";
	_menuBackPNG = LoadGraph(menuback.c_str());

	// �A�C�e���{�b�N�X�w�i
	std::string itembox = "image/itembox.png";
	_itemBoxPNG = LoadGraph(itembox.c_str());

	// �I�𒆂̃A�C�e��
	std::string itemChoice = "image/itemChoice.png";
	_itemChoicePNG = LoadGraph(itemChoice.c_str());

	// �g�p�̕���
	std::string use = "image/use.png";
	_usePNG = LoadGraph(use.c_str());

	// �̂Ă�̕���
	std::string suteru = "image/suteru.png";
	_suteruPNG = LoadGraph(suteru.c_str());

	// �߂�̕���
	std::string back = "image/back.png";
	_backPNG = LoadGraph(back.c_str());

	// �����̌��摜
	std::string setumei = "image/setumei.png";
	_setumeiPNG = LoadGraph(setumei.c_str());

	// ���j���[�{�^��
	std::string menu = "image/menuButton.png";
	_menuPNG = LoadGraph(menu.c_str());
}

void Menu::Update(Player* player, Monster* monster, Cards* cards)
{
	if (_menu == MENU::SAVE)
	{
		// �Z�[�u�����������Ă݂�
	}

	// �Q�[����ʖ߂�
	if (_menu == MENU::TO_GAME || _menu == MENU::SAVE)
	{
		_menuBackPngFlg = false;
		_menuSelPngFlg = false;
		_menu = MENU::NON;
	}

	// �A�C�e����ʏ���
	if (_menu == MENU::ITEM)
	{
		if (_mouse & MOUSE_INPUT_LEFT) {		 // �}�E�X�̍��{�^����������Ă�����
			for (int i = 0; i <= 11; i++)
			{
				if (_cursorPos.x >= itemBox[i].pos.x && _cursorPos.x <= itemBox[i].pos.x + 100 && _cursorPos.y >= itemBox[i].pos.y && _cursorPos.y <= itemBox[i].pos.y + 100)
				{
					if (itemBox[i]._item != ITEM::NON)
					{
						// �A�C�e����I��
						_chooseNum = i;
						_choicePos = itemBox[i].pos;
						_itemSetumei = itemBox[i]._item;
						_useOrThrowAway = true;
						_nonNeedFlg = false;
					}

					//if (itemBox[i]._item == ITEM::POTION)
					//{
					//	// �v���C���[��HP���񕜂���
					//	_itemAction = ITEM::POTION;
					//	// �摜������(�񕜖������)
					//	itemBox[i]._item = ITEM::NON;
					//	itemBox[i].png = -1;
					//}
					//
					//if (itemBox[i]._item == ITEM::SWORD)
					//{
					//	_itemAction = ITEM::SWORD;
					//}
					//
					//if (itemBox[i]._item == ITEM::SHIELD)
					//{
					//	_itemAction = ITEM::SHIELD;
					//}
				}
			}

			// �I�𒆂̃A�C�e�����ǂ����邩
			if (_useOrThrowAway)
			{
				// �摜�������Ƃ���̋��ʏ���
				auto lambdaPNGSakujyo = [&]() {
					_itemSetumei = ITEM::NON;
					itemBox[_chooseNum]._item = ITEM::NON;
					itemBox[_chooseNum].png = -1;
					_chooseNum = -1;
					_useOrThrowAway = false;
					_choicePos = { -100,-100 };
				};

				// ���ʂ��Ȃ���t���O
				if (itemBox[_chooseNum]._item == ITEM::POTION || itemBox[_chooseNum]._item == ITEM::POTION_BIG)
				{
					if (player->GetHP() == player->GetMaxHP())
					{
						_nonNeedFlg = true;
					}
				}

				if (itemBox[_chooseNum]._item == ITEM::DETOX)
				{
					if (player->GetCondition() == CONDITION::FINE)
					{
						_nonNeedFlg = true;
					}
				}

				if (itemBox[_chooseNum]._item == ITEM::ENEMY_1)
				{
					if (monster->GetEnemyState() != ENEMY_STATE::EXIST)
					{
						_nonNeedFlg = true;
					}
				}

				if (itemBox[_chooseNum]._item == ITEM::ENEMY_2)
				{
					if (monster->GetEnemyState() != ENEMY_STATE::EXIST || _nonDamageFlg)
					{
						_nonNeedFlg = true;
					}
				}

				if (itemBox[_chooseNum]._item == ITEM::KYOUKA_POW)
				{
					if (_powUpNum == 5)
					{
						_nonNeedFlg = true;
					}
				}

				// �g��or����
				if (_cursorPos.x >= 50 && _cursorPos.x <= 50 + 150 && _cursorPos.y >= 400 && _cursorPos.y <= 400 + 75)
				{
					// ����(��)�����_��
					auto lambdaSword = [&](ITEM item) {
						_itemAction = item;
						_equipSwordPos = itemBox[_chooseNum].pos;
						_chooseNum = -1;
						_useOrThrowAway = false;
						_choicePos = { -100,-100 };
					};

					// ����(��)�����_��
					auto lambdaShield = [&](ITEM item) {
						_itemAction = item;
						_equipShieldPos = itemBox[_chooseNum].pos;
						_chooseNum = -1;
						_useOrThrowAway = false;
						_choicePos = { -100,-100 };
					};

					// ���i�A�C�e���Ɋւ���
					for (auto item : ITEM())
					{
						if (item >= ITEM::POTION && item <= ITEM::HEART)
						{
							if (itemBox[_chooseNum]._item == item)
							{
								if (!_nonNeedFlg)
								{
									_itemAction = item;
									lambdaPNGSakujyo();
								}
								else
								{
									_useOrThrowAway = false;
									_nonNeedFlg = false;
								}
							}
						}
					}

					//if (itemBox[_chooseNum]._item == ITEM::POTION)
					//{	
					//	if(!_nonNeedFlg)
					//	{
					//		// �v���C���[��HP���񕜂���
					//		_itemAction = ITEM::POTION;
					//		lambdaPNGSakujyo();
					//	}
					//	else
					//	{
					//		_useOrThrowAway = false;
					//		_nonNeedFlg = false;
					//	}
					//	
					//	// �摜������(�񕜖������)
					//	//_itemSetumei = ITEM::NON;
					//	//itemBox[_chooseNum]._item = ITEM::NON;
					//	//itemBox[_chooseNum].png = -1;
					//	//_chooseNum = -1;
					//	//_useOrThrowAway = false;
					//	//_choicePos = { -100,-100 };
					//}
					//if (itemBox[_chooseNum]._item == ITEM::DETOX)
					//{
					//	if (!_nonNeedFlg)
					//	{
					//		// �v���C���[�̓ł��񕜂���
					//		_itemAction = ITEM::DETOX;
					//		lambdaPNGSakujyo();
					//	}
					//	else
					//	{
					//		_useOrThrowAway = false;
					//		_nonNeedFlg = false;
					//	}
					//	// �摜������(�ł�����)
					//	//_itemSetumei = ITEM::NON;
					//	//itemBox[_chooseNum]._item = ITEM::NON;
					//	//itemBox[_chooseNum].png = -1;
					//	//_chooseNum = -1;
					//	//_useOrThrowAway = false;
					//	//_choicePos = { -100,-100 };
					//}
					//if (itemBox[_chooseNum]._item == ITEM::KYOUKA_POW)
					//{
					//	if (!_nonNeedFlg)
					//	{
					//		// �ꎞ�I�ɍU���̓A�b�v(5�^�[�����炢)
					//		_itemAction = ITEM::KYOUKA_POW;
					//		lambdaPNGSakujyo();
					//	}
					//	else
					//	{
					//		_useOrThrowAway = false;
					//		_nonNeedFlg = false;
					//	}
					//	// �摜������
					//	//_itemSetumei = ITEM::NON;
					//	//itemBox[_chooseNum]._item = ITEM::NON;
					//	//itemBox[_chooseNum].png = -1;
					//	//_chooseNum = -1;
					//	//_useOrThrowAway = false;
					//	//_choicePos = { -100,-100 };
					//}

					// ���Ɋւ���
					for (auto item : ITEM())
					{
						if (item >= ITEM::SWORD && item <= ITEM::SWORD_LV3)
						{
							if (itemBox[_chooseNum]._item == item)
							{
								lambdaSword(item);
							}
						}
					}

					// ���Ɋւ���
					for (auto item : ITEM())
					{
						if (item >= ITEM::SHIELD && item <= ITEM::SHIELD_LV2)
						{
							if (itemBox[_chooseNum]._item == item)
							{
								lambdaShield(item);
							}
						}
					}

					//if (itemBox[_chooseNum]._item == ITEM::SWORD)
					//{
					//	lambdaSword(ITEM::SWORD);
					//  _itemAction = ITEM::SWORD;
					//  _equipSwordPos = itemBox[_chooseNum].pos;
					//  _chooseNum = -1;
					//  _useOrThrowAway = false;
					//  _choicePos = { -100,-100 };
					//}
					//if (itemBox[_chooseNum]._item == ITEM::SWORD_LV2)
					//{
					//	lambdaSword(ITEM::SWORD_LV2);
					//	//_itemAction = ITEM::SWORD_LV2;
					//	//_equipSwordPos = itemBox[_chooseNum].pos;
					//	//_chooseNum = -1;
					//	//_useOrThrowAway = false;
					//	//_choicePos = { -100,-100 };
					//}

					//if (itemBox[_chooseNum]._item == ITEM::SHIELD)
					//{
					//	lambdaShield(ITEM::SHIELD);
					//	//_itemAction = ITEM::SHIELD;
					//	//_equipShieldPos = itemBox[_chooseNum].pos;
					//	//_chooseNum = -1;
					//	//_useOrThrowAway = false;
					//	//_choicePos = { -100,-100 };
					//}
					//if (itemBox[_chooseNum]._item == ITEM::SHIELD_LV2)
					//{
					//	lambdaShield(ITEM::SHIELD_LV2);
					//}

					// �h���b�v�A�C�e���Ɋւ���
					for (auto item : ITEM())
					{
						if (item >= ITEM::ENEMY_1 || item <= ITEM::POTION_BIG)
						{
							if (itemBox[_chooseNum]._item == item)
							{
								if (!_nonNeedFlg)
								{
									_itemAction = item;
									lambdaPNGSakujyo();
								}
								else
								{
									_useOrThrowAway = false;
									_nonNeedFlg = false;
								}
							}
						}
					}

					//if (itemBox[_chooseNum]._item == ITEM::ENEMY_1)
					//{
					//	if (!_nonNeedFlg)
					//	{
					//		// �G�ɌŒ�_���[�W20
					//		_itemAction = ITEM::ENEMY_1;
					//		lambdaPNGSakujyo();
					//	}
					//	else
					//	{
					//		_useOrThrowAway = false;
					//		_nonNeedFlg = false;
					//	}
					//	// �摜������
					//	//_itemSetumei = ITEM::NON;
					//	//itemBox[_chooseNum]._item = ITEM::NON;
					//	//itemBox[_chooseNum].png = -1;
					//	//_chooseNum = -1;
					//	//_useOrThrowAway = false;
					//	//_choicePos = { -100,-100 };
					//}
					//if (itemBox[_chooseNum]._item == ITEM::ENEMY_2)
					//{
					//	if (!_nonNeedFlg)
					//	{
					//		// ���̃^�[���͓G�̍U��������
					//		_itemAction = ITEM::ENEMY_2;
					//		lambdaPNGSakujyo();
					//	}
					//	else
					//	{
					//		_useOrThrowAway = false;
					//		_nonNeedFlg = false;
					//	}
					//}
					//if (itemBox[_chooseNum]._item == ITEM::POTION_BIG)
					//{
					//	if (!_nonNeedFlg)
					//	{
					//		// �v���C���[��HP���񕜂���
					//		_itemAction = ITEM::POTION_BIG;
					//		lambdaPNGSakujyo();
					//	}
					//	else
					//	{
					//		_useOrThrowAway = false;
					//		_nonNeedFlg = false;
					//	}
					//	// �摜������
					//	//_itemSetumei = ITEM::NON;
					//	//itemBox[_chooseNum]._item = ITEM::NON;
					//	//itemBox[_chooseNum].png = -1;
					//	//_chooseNum = -1;
					//	//_useOrThrowAway = false;
					//	//_choicePos = { -100,-100 };
					//}
				}

				// �̂Ă�
				if (_cursorPos.x >= 50 && _cursorPos.x <= 50 + 150 && _cursorPos.y >= 500 && _cursorPos.y <= 500 + 75)
				{

					// ���Ɋւ���
					for (auto item : ITEM())
					{
						if (item >= ITEM::SWORD && item <= ITEM::SWORD_LV3)
						{
							if (itemBox[_chooseNum]._item == item)
							{
								_equipSwordPos = { -100,-1 };
								_equipDamage = 0;
							}
						}
					}

					// ���Ɋւ���
					for (auto item : ITEM())
					{
						if (item >= ITEM::SHIELD && item <= ITEM::SHIELD_LV2)
						{
							if (itemBox[_chooseNum]._item == item)
							{
								_equipShieldPos = { -100,-1 };
								_equipGuard = 0;
							}
						}
					}

					//if (itemBox[_chooseNum]._item == ITEM::SHIELD)
					//{
					//	_equipShieldPos = { -100,-1 };
					//	_equipGuard = 0;
					//}
					//if (itemBox[_chooseNum]._item == ITEM::SWORD)
					//{
					//	_equipSwordPos = { -100,-1 };
					//	_equipDamage = 0;
					//}
					//if (itemBox[_chooseNum]._item == ITEM::SWORD_LV2)
					//{
					//	_equipSwordPos = { -100,-1 };
					//	_equipDamage = 0;
					//}
					//if (itemBox[_chooseNum]._item == ITEM::SHIELD_LV2)
					//{
					//	_equipShieldPos = { -100,-1 };
					//	_equipGuard = 0;
					//}

					lambdaPNGSakujyo();
					// �摜������
					//_itemSetumei = ITEM::NON;
					//itemBox[_chooseNum]._item = ITEM::NON;
					//itemBox[_chooseNum].png = -1;
					//_chooseNum = -1;
					//_useOrThrowAway = false;
					//_choicePos = { -100,-100 };
				}
			}
		}
	}

	// �퓬���̓A�C�e�����g�����тɉ�ʂ�߂�
	// �퓬���́A�A�C�e�����g�����ƂɓG�̑ҋ@�^�[��-1�ɂ���
	auto lambdaBattle = [&]() {
		// ���j���[��ʂ�����
		_menuBackPngFlg = false;
		_menuSelPngFlg = false;			// ��������
		_menu = MENU::NON;			// ��Ԃ�߂�
		_useOrThrowAway = false;	// �g���Ǝ̂Ă�̕����`�����
		cards->SetTurn(cards->GetTurn() - 1);
	};

	// �v���C���[�̉񕜖�ɂ���
	if (_itemAction == ITEM::POTION)
	{
		player->SetHP(player->GetHP() + 30);
		_itemAction = ITEM::NON;
		if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
		{
			lambdaBattle();
		}
	}

	// �v���C���[�̗̑͑����܂ɂ�鑝��
	if (_itemAction == ITEM::HEART)
	{
		player->SetMaxHP(player->GetMaxHP() + 10);
		_itemAction = ITEM::NON;
		if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
		{
			lambdaBattle();
		}
	}

	// �v���C���[�̉񕜖�ɂ���
	if (_itemAction == ITEM::POTION_BIG)
	{
		player->SetHP(player->GetHP() + 50);
		_itemAction = ITEM::NON;
		if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
		{
			lambdaBattle();
		}
	}

	// ��Ŗ�ɂ���Ԉُ��
	if (_itemAction == ITEM::DETOX)
	{
		player->SetCondition(CONDITION::FINE);
		player->SetConditionTurn(-1);
		_itemAction = ITEM::NON;
		if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
		{
			lambdaBattle();
		}
	}

	// �v���C���[�̑���
	// ���Ɋւ���
	for (auto item : ITEM())
	{
		if (item >= ITEM::SWORD && item <= ITEM::SWORD_LV3)
		{
			if (itemBox[_chooseNum]._item == item)
			{
				_equipDamage = (static_cast<int>(item) - 4) * 5;
				_itemAction = ITEM::NON;
				if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
				{
					lambdaBattle();
				}
			}
		}
	}

	// ���Ɋւ���
	for (auto item : ITEM())
	{
		if (item >= ITEM::SHIELD && item <= ITEM::SHIELD_LV2)
		{
			if (itemBox[_chooseNum]._item == item)
			{
				_equipGuard = (static_cast<int>(item) - 7) * 4;
				_itemAction = ITEM::NON;
				if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
				{
					lambdaBattle();
				}
			}
		}
	}

	// ��
	//if (_itemAction == ITEM::SWORD)
	//{
	//	_equipDamage = 2;
	//	_itemAction = ITEM::NON;
	//	if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
	//	{
	//		lambdaBattle();
	//	}
	//}
	//// ��2
	//if (_itemAction == ITEM::SWORD_LV2)
	//{
	//	_equipDamage = 10;
	//	_itemAction = ITEM::NON;
	//	if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
	//	{
	//		lambdaBattle();
	//	}
	//}
	//// ��
	//if (_itemAction == ITEM::SHIELD)
	//{
	//	_equipGuard = 3;
	//	_itemAction = ITEM::NON;
	//	if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
	//	{
	//		lambdaBattle();
	//	}
	//}
	//// ��2
	//if (_itemAction == ITEM::SHIELD_LV2)
	//{
	//	_equipGuard = 10;
	//	_itemAction = ITEM::NON;
	//	if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
	//	{
	//		lambdaBattle();
	//	}
	//}

	// �G���U���A�C�e��
	if (_itemAction == ITEM::ENEMY_1)
	{
		// �Œ�_���[�W20
		monster->Damage(20);
		_itemAction = ITEM::NON;

		//// ���j���[��ʂ�����
		//_menuBackPngFlg = false;
		//_menuSelPngFlg = false;			// ��������
		//_menu = MENU::NON;			// ��Ԃ�߂�
		//_useOrThrowAway = false;	// �g���Ǝ̂Ă�̕����`�����
		if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
		{
			lambdaBattle();
		}
	}

	if (_itemAction == ITEM::ENEMY_2)
	{
		// ���̃^�[���͓G�̍U���𖳌���
		_nonDamageFlg = true;
		_itemAction = ITEM::NON;

		if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
		{
			lambdaBattle();
		}
	}

	// �ꎞ�I�ɍU����up
	if (_itemAction == ITEM::KYOUKA_POW)
	{
		_powUpNum = 5;
		_itemAction = ITEM::NON;

		//// ���j���[��ʂ�����
		//_menuBackPngFlg = false;
		//_menuSelPngFlg = false;			// ��������
		//_menu = MENU::NON;			// ��Ԃ�߂�
		//_useOrThrowAway = false;	// �g���Ǝ̂Ă�̕����`�����
		if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
		{
			lambdaBattle();
		}
	}
}

void Menu::Draw(Player* player, Item* item, Monster* monster)
{
	//DrawGraph(0, 0, _menuPNG, true);

	if (monster->GetEnemyState() != ENEMY_STATE::EXIST)
	{
		DrawGraph(0, 0, _menuPNG, true);
	}
	else
	{
		// �A�C�e���̂ݎg�p�\
		DrawGraph(0, 0, _menuSelPNG[1], true);
	}

	// ���j���[��ʂ̕\��
	if (_menuBackPngFlg)
	{
		DrawGraph(200, 0, _menuBackPNG, true);
	}

	// ���j���[����
	if (_menuSelPngFlg)
	{
		for (int i = 0; i <= 2; i++)
		{
			DrawGraph(menu_pair[i].first.x, menu_pair[i].first.y, _menuSelPNG[i], true);
		}

		DrawGraph(menu_pair[3].first.x, menu_pair[3].first.y, _menuTitleBackPNG, true);	
		DrawGraph(menu_pair[4].first.x, menu_pair[4].first.y, _menuSavePNG, true);
	}

	// �A�C�e��
	if (_menu == MENU::ITEM)
	{
		// �g�Ɠ���A�C�e���̕`��
		for (int i = 0; i <= 11; i++)
		{
			DrawGraph(itemBox[i].pos.x, itemBox[i].pos.y, _itemBoxPNG, true);
			// ���肵�Ă���A�C�e���̕`��(���Ă��Ȃ������牽���`�悳��Ȃ��悤�ɂ���)
			DrawGraph(itemBox[i].pos.x, itemBox[i].pos.y, itemBox[i].png, true);
		}

		DrawGraph(_choicePos.x, _choicePos.y, _itemChoicePNG, true);

		// �������ꂽ�炻����E�̕������o��
		DrawFormatString(_equipSwordPos.x + 10, _equipSwordPos.y + 10, GetColor(255, 0, 0), "E");
		DrawFormatString(_equipShieldPos.x + 10, _equipShieldPos.y + 10, GetColor(255, 0, 0), "E");
	}

	// �X�e�[�^�X���
	if (_menu == MENU::STATUS)
	{
		DrawFormatString(350, 100, 0x000000, "���x��:%d", player->GetNowLevel());
		DrawFormatString(350, 130, 0x000000, "�̗�:%d / %d", player->GetHP(), player->GetMaxHP());
		DrawFormatString(350, 160, 0x000000, "�U����:%d(+ %d) = %d", player->GetAttackDamage(), _equipDamage, player->GetAttackDamage() + _equipDamage);
		DrawFormatString(350, 190, 0x000000, "�h���:%d(+ %d) = %d", player->GetDifense(), _equipGuard, player->GetDifense() + _equipGuard);
		DrawFormatString(350, 220, 0x000000, "���̃��x���܂�:�c��%d", player->GetNextLevel());
		DrawFormatString(350, 250, 0x000000, "������:%d�~", player->GetMoney());
	}

	// ���j���[�̃X�e�[�^�X��ʂƃA�C�e����ʂ̕`��
	if (_menu == MENU::ITEM || _menu == MENU::STATUS)
	{
		// �߂�
		DrawGraph(375, 470, _backPNG, true);

		if (_useOrThrowAway)
		{
			// �����̌��摜
			DrawGraph(0, 320, _setumeiPNG, true);

			// �A�C�e�����g�����ƂɌ��ʂ�����Ƃ�
			if (!_nonNeedFlg)
			{
				// �g��
				DrawGraph(50, 400, _usePNG, true);
			}
			else
			{
				// �A�C�e�����g�����ƂɌ��ʂ��Ȃ��Ƃ�

				// �`��u�����h���[�h�ύX
				SetDrawBlendMode(DX_BLENDMODE_ADD, 100);
				// �g��
				DrawGraph(50, 400, _usePNG, true);

				// �`��u�����h���[�h���m�[�u�����h�ɂ���
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				DrawFormatString(20, 300, 0xffffff, "���g���Ă����ʂ��Ȃ�");
			}
			// �̂Ă�
			DrawGraph(50, 500, _suteruPNG, true);
			// ���j���[�̃A�C�e���őI�������A�C�e���̐������o��
			if (_itemSetumei != ITEM::NON)
			{
				//_chooseNum = _menu->GetNumNum();
				DrawFormatString(25, 340, 0x000000, "%s\n", item->GetSetumei(static_cast<int>(_itemSetumei) - 1));
				//DrawFormatString(700, 450, 0xFFFFFF, "%s\n", item[static_cast<int>(_menu->Getitemsetumei()) -1].setumei);
			}
		}
	}
}

void Menu::MenuButton_NonEnemy(void)
{
	_mouse = GetMouseInput();					 //�}�E�X�̓��͏�Ԏ擾
	GetMousePoint(&_cursorPos.x, &_cursorPos.y);	 //�}�E�X�̍��W�擾

	if (_mouse & MOUSE_INPUT_LEFT) {				 //�}�E�X�̍��{�^����������Ă�����
		//X:0,Y:0
		// �����\�����ɏォ�獀�ڂ��\������Ȃ��悤�ɂ���
		if (!_menuBackPngFlg)
		{
			if (_cursorPos.x >= 0 && _cursorPos.x <= 0 + 150 && _cursorPos.y >= 0 && _cursorPos.y <= 0 + 75)
			{
				// �N���b�N��
				PlaySoundMem(_seClick, DX_PLAYTYPE_BACK, true);

				_menuBackPngFlg = true;
				_menuSelPngFlg = true;
			}
		}

		if (_menuSelPngFlg)
		{
			// ���j���[���ڃ{�^���Ƃ̓����蔻��
			for (int i = 0; i <= 4; i++)
			{
				if (_cursorPos.x >= menu_pair[i].first.x && _cursorPos.x <= menu_pair[i].first.x + buttonSize[i].x && _cursorPos.y >= menu_pair[i].first.y && _cursorPos.y <= menu_pair[i].first.y + buttonSize[i].y)
				{
					// �N���b�N��
					PlaySoundMem(_seClick, DX_PLAYTYPE_BACK, true);

					_menu = menu_pair[i].second;
					_menuSelPngFlg = false;	// ��������
				}
			}
		}

		// �߂�{�^������������Q�[���ĊJ
		if (_menu == MENU::ITEM || _menu == MENU::STATUS)
		{
			if (_cursorPos.x >= 375 && _cursorPos.x <= 375 + 150 && _cursorPos.y >= 470 && _cursorPos.y <= 470 + 60)
			{
				// �N���b�N��
				PlaySoundMem(_seClick, DX_PLAYTYPE_BACK, true);

				_menuBackPngFlg = false;
				_menuSelPngFlg = false;			// ��������
				_menu = MENU::NON;			// ��Ԃ�߂�
				_useOrThrowAway = false;	// �g���Ǝ̂Ă�̕����`�����
			}
		}
	}
}

void Menu::MenuButton_Enemy(void)
{
	// �A�C�e���{�^���̂ݕ\��
	_mouse = GetMouseInput();					 //�}�E�X�̓��͏�Ԏ擾
	GetMousePoint(&_cursorPos.x, &_cursorPos.y);	 //�}�E�X�̍��W�擾

	if (_mouse & MOUSE_INPUT_LEFT) {				 //�}�E�X�̍��{�^����������Ă�����
		if (_cursorPos.x >= 0 && _cursorPos.x <= 0 + 200 && _cursorPos.y >= 0 && _cursorPos.y <= 0 + 100)
		{
			// �N���b�N��
			PlaySoundMem(_seClick, DX_PLAYTYPE_BACK, true);

			_menu = MENU::ITEM;
			_menuBackPngFlg = true;
		}

		// �߂�{�^������������Q�[���ĊJ
		if (_menu == MENU::ITEM || _menu == MENU::STATUS)
		{
			if (_cursorPos.x >= 375 && _cursorPos.x <= 375 + 150 && _cursorPos.y >= 470 && _cursorPos.y <= 470 + 60)
			{
				// �N���b�N��
				PlaySoundMem(_seClick, DX_PLAYTYPE_BACK, true);

				_menuBackPngFlg = false;
				_menuSelPngFlg = false;			// ��������
				_menu = MENU::NON;			// ��Ԃ�߂�
				_useOrThrowAway = false;	// �g���Ǝ̂Ă�̕����`�����
			}
		}
	}
}

void Menu::Setitem(ITEM item, int png)
{
	//����12
	for (int i = 0; i < 12; i++)
	{
		// �������ꏊ��T���ē����
		if (itemBox[i]._item == ITEM::NON)
		{
			itemBox[i]._item = item;
			itemBox[i].png = png;
			return;						// ���ꂽ��for���𔲂���悤�ɂ���
		}
	}
	//itemBox[0]._item = item;
}

bool Menu::GetMenuFlg(void)
{
	return _menuSelPngFlg;
}

MENU Menu::GetMenu(void)
{
	return _menu;
}

int Menu::GetCanHaveItem(void)
{
	int canHave = 0;
	//����12
	for (int i = 0; i < 12; i++)
	{
		// �������ꏊ��T���ē����
		if (itemBox[i]._item == ITEM::NON)
		{
			canHave++;
		}
	}

	// �Ō�܂�0�Ȃ�A�������Ƃ��낪�Ȃ�
	return canHave;
}

int Menu::GetEquipDamage(void)
{
	return _equipDamage;
}

int Menu::GetEquipGuard(void)
{
	return _equipGuard;
}

int Menu::GetPowUp(void)
{
	return _powUpNum;
}

void Menu::SetPowUp(int num)
{
	// �U�����邲�Ƃ�-1����Ă���
	_powUpNum = num;
}

bool Menu::GetNonDamageFlg(void)
{
	return _nonDamageFlg;
}

void Menu::SetNonDamageFlg(bool flag)
{
	_nonDamageFlg = flag;
}