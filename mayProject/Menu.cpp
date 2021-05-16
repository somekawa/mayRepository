#include <DxLib.h>
#include <string>
#include "Scene/SelectScene.h"
#include "Scene/GameScene.h"
#include "Menu.h"
#include "Player.h"
#include "Item/Item.h"
#include "Enemy/Monster.h"
#include "Card/Cards.h"
#include "Common/MouseCtl.h"

// static�ϐ��̎���<�^>�N���X��::�ϐ��� = ������;
bool Menu::_loadFlg = false;

struct item
{
	VECTOR2 pos;		// �{�b�N�X�̈ʒu
	ITEM _item;			// �A�C�e�����
	int png;			// �摜
}itemBox[12];

Menu::Menu()
{
	Init();
}

Menu::~Menu()
{
	// ���֌W
	for (int i = 0; i < 5; i++)
	{
		DeleteSoundMem(_soundSE[i]);
	}
}

bool Menu::Init(void)
{
	_menu = MENU::NON;
	_itemAction = ITEM::NON;
	_itemExplanation = ITEM::NON;

	// �����Ȃ��Ƃ���ɏo���Ă���
	_equipShieldPos = { -100,-1 };
	_equipSwordPos  = { -100,-1 };
	_choicePos      = { -100,-100 };

	/*���j���[�֌W*/
	_chooseNum = -1;
	_menuBackPngFlg = false;
	_menuSelPngFlg  = false;

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
	_equipGuard  = 0;
	_useOrThrowAway = false;
	_nonNeedFlg   = false;
	_nonDamageFlg = false;
	_escapeFlg    = false;
	_AppraisalFlg = false;

	std::string itemName[static_cast<int>(ITEM::MAX)-1];
	std::pair<std::string, ITEM> pair[static_cast<int>(ITEM::MAX) - 1];

	// �A�C�e���֌W
	// �t�@�C����ǂݍ���
	const auto FileHandle_item = FileRead_open("csv/itemName.csv");
	if (FileHandle_item == NULL)
	{
		return false; //�G���[
	}

	const std::string image = "image/";
	const std::string png   = ".png";
	for (int i = 0; i <= static_cast<int>(ITEM::MAX) - 2; i++)
	{
		char name[256];
		FileRead_scanf(FileHandle_item, "%s",name);
		name[255] = '\0';
		pair[i].first  = image + name + png;
		pair[i].second = static_cast<ITEM>(i+1);
	}
	//�t�@�C�������
	FileRead_close(FileHandle_item);

	// �A�C�e���{�b�N�X�̈ʒu�Ə����A�C�e���̏��
	for (int i = 0; i <= 11; i++)
	{
		itemBox[i].pos = { ((i % 3) + 3) * 100,((i / 3) + 1) * 100 - 30 };
		if (!_loadFlg)
		{
			itemBox[i]._item = ITEM::NON;
		}
		else
		{
			// ���[�h���̃A�C�e������
			if (static_cast<int>(itemBox[i]._item) > 0)
			{
				itemBox[i].png = LoadGraph(pair[static_cast<int>(itemBox[i]._item)-1].first.c_str());
			}
			else
			{
				itemBox[i]._item = ITEM::NON;
			}
		}
	}

	PngInit();

	// SE
	_soundSE[0] = LoadSoundMem("sound/se/click.mp3");
	_soundSE[1] = LoadSoundMem("sound/se/healCard.mp3");
	_soundSE[2] = LoadSoundMem("sound/se/poison_care.mp3");
	_soundSE[3] = LoadSoundMem("sound/se/charge.mp3");	
	_soundSE[4] = LoadSoundMem("sound/se/dropItem.mp3");
	return true;
}

void Menu::PngInit(void)
{
	// ���j���[�̃{�^��3��
	std::string menuButton = "image/menu/menuSel.png";
	LoadDivGraph(menuButton.c_str(), 3, 3, 1, 200, 100, _menuSelPNG);

	_menuImages.try_emplace("titleBack", LoadGraph("image/menuTitleBackButton.png"));	// �^�C�g���֖߂�{�^��
	_menuImages.try_emplace("save", LoadGraph("image/menuSave.png"));					// �Z�[�u�{�^��
	_menuImages.try_emplace("menuButton", LoadGraph("image/menuButton.png"));			// ���j���[��ʔw�i
	_menuImages.try_emplace("menuWindow", LoadGraph("image/menu_window.png"));			// �A�C�e���{�b�N�X�w�i
	_menuImages.try_emplace("use", LoadGraph("image/choice/use.png"));					// �g�p�̕���
	_menuImages.try_emplace("throw_away", LoadGraph("image/choice/throw_away.png"));	// �̂Ă�̕���
	_menuImages.try_emplace("back", LoadGraph("image/choice/back.png"));				// �߂�̕���
	_menuImages.try_emplace("description", LoadGraph("image/description.png"));			// �����̌��摜
	_menuImages.try_emplace("itembox", LoadGraph("image/itembox.png"));					// �A�C�e���{�b�N�X�摜
	_menuImages.try_emplace("itemChoice", LoadGraph("image/itemChoice.png"));			// �I�𒆃A�C�e��
}

void Menu::UseItem(void)
{
	// �N���b�N��
	PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);

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
		if (item >= ITEM::POTION && item <= ITEM::GLASSES)
		{
			if (itemBox[_chooseNum]._item == item)
			{
				if (!_nonNeedFlg)
				{
					_itemAction = item;
					DeleteItem();
				}
				else
				{
					_useOrThrowAway = false;
					_nonNeedFlg = false;
				}
			}
		}
	}

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
		if (item >= ITEM::SHIELD && item <= ITEM::SHIELD_LV3)
		{
			if (itemBox[_chooseNum]._item == item)
			{
				lambdaShield(item);
			}
		}
	}

	// �h���b�v�A�C�e���Ɋւ���
	for (auto item : ITEM())
	{
		if (item >= ITEM::ENEMY_1 && item <= ITEM::POTION_BIG)
		{
			if (itemBox[_chooseNum]._item == item)
			{
				if (!_nonNeedFlg)
				{
					_itemAction = item;
					DeleteItem();
				}
				else
				{
					_useOrThrowAway = false;
					_nonNeedFlg = false;
				}
			}
		}
	}
}

void Menu::ThrowAwayItem(void)
{
	// �N���b�N��
	PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);

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
		if (item >= ITEM::SHIELD && item <= ITEM::SHIELD_LV3)
		{
			if (itemBox[_chooseNum]._item == item)
			{
				_equipShieldPos = { -100,-1 };
				_equipGuard = 0;
			}
		}
	}

	DeleteItem();
}

void Menu::DeleteItem(void)
{
	_itemExplanation = ITEM::NON;
	itemBox[_chooseNum]._item = ITEM::NON;
	itemBox[_chooseNum].png   = -1;
	_chooseNum = -1;
	_useOrThrowAway = false;
	_choicePos = { -100,-100 };
}

void Menu::GameContinue(const std::shared_ptr<MouseCtl>& mouse)
{
	// �߂�{�^������������Q�[���ĊJ
	if (_menu == MENU::ITEM || _menu == MENU::STATUS)
	{
		if (mouse->GetPos().x >= 375 && mouse->GetPos().x <= 375 + 150 &&
			mouse->GetPos().y >= 470 && mouse->GetPos().y <= 470 + 60)
		{
			// �N���b�N��
			PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);

			_menuBackPngFlg = false;
			_menuSelPngFlg = false;	// ����������
			_menu = MENU::NON;			// ��Ԃ�߂�
			_useOrThrowAway = false;	// �g���Ǝ̂Ă�̕���������
		}
	}
}

void Menu::Update(GameScene* game,const std::shared_ptr<Player>& player, const std::shared_ptr<Monster>& monster, const std::shared_ptr<Cards>& cards, const std::shared_ptr<MouseCtl>& mouse)
{
	if (_menu == MENU::SAVE)
	{
		// �Z�[�u����
		Save(player);
	}

	// �Q�[����ʖ߂�
	if (_menu == MENU::TO_GAME || _menu == MENU::SAVE)
	{
		_menuBackPngFlg = false;
		_menuSelPngFlg = false;
		_menu = MENU::NON;
	}

	// �A�C�e����ʏ���(�N���b�N���ꂽ����ITEM��ʂł���Ώ������s��)
	if (mouse->GetClickTrg() && (_menu == MENU::ITEM))
	{
		for (int i = 0; i <= 11; i++)
		{
			if (mouse->GetPos().x >= itemBox[i].pos.x && mouse->GetPos().x <= itemBox[i].pos.x + 100 &&
				mouse->GetPos().y >= itemBox[i].pos.y && mouse->GetPos().y <= itemBox[i].pos.y + 100)
			{
				if (itemBox[i]._item != ITEM::NON)
				{
					// �A�C�e����I��
					_chooseNum = i;
					_choicePos = itemBox[i].pos;
					_itemExplanation = itemBox[i]._item;
					_useOrThrowAway  = true;
					_nonNeedFlg = false;
				}
			}
		}

		// �I�𒆂̃A�C�e�����ǂ����邩
		if (_useOrThrowAway)
		{
			// �A�C�e�����g�p���Ă����ʂ��Ȃ��ƒm�点��t���O��true�ɂ���
			switch (itemBox[_chooseNum]._item)
			{
			case ITEM::POTION:
				if (player->GetHP() == player->GetMaxHP())
				{
					_nonNeedFlg = true;
				}
				break;
			case ITEM::POTION_BIG:
				if (player->GetHP() == player->GetMaxHP())
				{
					_nonNeedFlg = true;
				}
				break;
			case ITEM::DETOX:
				if (player->GetCondition() == CONDITION::FINE)
				{
					_nonNeedFlg = true;
				}
				break;
			case ITEM::GLASSES:
				if (_AppraisalFlg || game->eventState != EVENT_STATE::CHEST)
				{
					_nonNeedFlg = true;
				}
				break;
			case ITEM::ENEMY_1:
				if (monster->GetEnemyState() != ENEMY_STATE::EXIST)
				{
					_nonNeedFlg = true;
				}
				break;
			case ITEM::ENEMY_2:
				if (monster->GetEnemyState() != ENEMY_STATE::EXIST || _nonDamageFlg)
				{
					_nonNeedFlg = true;
				}
				break;
			case ITEM::ENEMY_3:
				if (monster->GetEnemyState() != ENEMY_STATE::EXIST || cards->GetTurn() == monster->GetMaxTurn())
				{
					_nonNeedFlg = true;
				}
				break;
			case ITEM::ENEMY_4:
				if (monster->GetEnemyState() != ENEMY_STATE::EXIST || monster->GetEnemyNum() == 5 || monster->GetEnemyNum() == 6)
				{
					_nonNeedFlg = true;
				}
				break;
			case ITEM::POW_UP:
				if (_powUpNum >= 5)
				{
					_nonNeedFlg = true;
				}
				break;
			case ITEM::SKILL_FAST:
				if (player->GetSkillCharge() == 0)
				{
					_nonNeedFlg = true;
				}
				break;
			default:
				break;
			}

			// �g��or����
			if (mouse->GetPos().x >= 50 &&  mouse->GetPos().x <= 50 + 150 &&
				mouse->GetPos().y >= 400 && mouse->GetPos().y <= 400 + 75)
			{
				UseItem();
			}

			// �̂Ă�
			if (mouse->GetPos().x >= 50 && mouse->GetPos().x <= 50 + 150 &&
				mouse->GetPos().y >= 500 && mouse->GetPos().y <= 500 + 75)
			{
				ThrowAwayItem();
			}
		}
	}

	// �퓬���̓A�C�e�����g�����тɉ�ʂ�߂�
	// �퓬���́A�A�C�e�����g�����ƂɓG�̑ҋ@�^�[��-1�ɂ���
	auto BattleLambda = [&]() {
		// ���j���[��ʂ�����
		_menuBackPngFlg = false;
		_menuSelPngFlg  = false;	// ��������
		_menu = MENU::NON;			// ��Ԃ�߂�
		_useOrThrowAway = false;	// �g���Ǝ̂Ă�̕����`�����
		cards->SetTurn(cards->GetTurn() - 1);
	};

	// �v���C���[�̉񕜖�ɂ���
	if (_itemAction == ITEM::POTION)
	{
		PlaySoundMem(_soundSE[1], DX_PLAYTYPE_BACK, true);
		player->SetHP(player->GetHP() + 30);
		_itemAction = ITEM::NON;
		if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
		{
			BattleLambda();
		}
	}

	// �v���C���[�̗̑͑����܂ɂ�鑝��
	if (_itemAction == ITEM::HEART)
	{
		PlaySoundMem(_soundSE[1], DX_PLAYTYPE_BACK, true);
		player->SetMaxHP(player->GetMaxHP() + 10);
		_itemAction = ITEM::NON;
		if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
		{
			BattleLambda();
		}
	}

	// �v���C���[�̉񕜖�ɂ���
	if (_itemAction == ITEM::POTION_BIG)
	{
		PlaySoundMem(_soundSE[1], DX_PLAYTYPE_BACK, true);
		player->SetHP(player->GetHP() + 50);
		_itemAction = ITEM::NON;
		if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
		{
			BattleLambda();
		}
	}

	// ��Ŗ�ɂ���Ԉُ��
	if (_itemAction == ITEM::DETOX)
	{
		PlaySoundMem(_soundSE[2], DX_PLAYTYPE_BACK, true);
		player->SetCondition(CONDITION::FINE);
		player->SetConditionTurn(-1);
		_itemAction = ITEM::NON;
		if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
		{
			BattleLambda();
		}
	}

	// �v���C���[�X�L������5�^�[���Z�k
	if (_itemAction == ITEM::SKILL_FAST)
	{
		PlaySoundMem(_soundSE[3], DX_PLAYTYPE_BACK, true);
		player->SetSkillCharge(player->GetSkillCharge() - 5);
		if (player->GetSkillCharge() <= 0)
		{
			// 0�������Ȃ��悤�ɂ���
			player->SetSkillCharge(0);
		}
		_itemAction = ITEM::NON;
		if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
		{
			BattleLambda();
		}
	}

	// �󔠂̒��g���Ӓ肷��
	if (_itemAction == ITEM::GLASSES)
	{
		PlaySoundMem(_soundSE[3], DX_PLAYTYPE_BACK, true);
		_AppraisalFlg = true;
		_itemAction = ITEM::NON;
		// ���j���[��ʂ�����
		_menuBackPngFlg = false;
		_menuSelPngFlg = false;		// ��������
		_menu = MENU::NON;			// ��Ԃ�߂�
		_useOrThrowAway = false;	// �g���Ǝ̂Ă�̕����`�����
	}

	// �v���C���[�̑�������
	bool tmpFlg = false;
	for (auto item : ITEM())
	{
		if (item >= ITEM::SWORD && item <= ITEM::SWORD_LV3)			// ���Ɋւ���
		{
			if (itemBox[_chooseNum]._item != item)
			{
				continue;
			}
			_equipDamage = (static_cast<int>(item) - (static_cast<int>(ITEM::SWORD)-1)) * 5;
			tmpFlg = true;
		}
		else if (item >= ITEM::SHIELD && item <= ITEM::SHIELD_LV3)	// ���Ɋւ���
		{
			if (itemBox[_chooseNum]._item != item)
			{
				continue;
			}
			_equipGuard = (static_cast<int>(item) - (static_cast<int>(ITEM::SHIELD) - 1)) * 4;
			tmpFlg = true;
		}
		else
		{
			// �����������s��Ȃ�
		}

		// �퓬���̃A�C�e�������Ȃ̂����m���߂�
		if (tmpFlg)	
		{
			_itemAction = ITEM::NON;
			if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
			{
				BattleLambda();
			}
			break;
		}
	}

	//// ���Ɋւ���
	//for (auto item : ITEM())
	//{
	//	if (item >= ITEM::SHIELD && item <= ITEM::SHIELD_LV3)
	//	{
	//		if (itemBox[_chooseNum]._item != item)
	//		{
	//			continue;
	//		}
	//		_equipGuard = (static_cast<int>(item) - (static_cast<int>(ITEM::SHIELD) - 1)) * 4;
	//		_itemAction = ITEM::NON;
	//		if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
	//		{
	//			BattleLambda();
	//		}
	//	}
	//}

	bool tmpUseItem = false;
	if (_itemAction == ITEM::ENEMY_1)
	{
		PlaySoundMem(_soundSE[4], DX_PLAYTYPE_BACK, true);
		monster->Damage(20,cards);	// �Œ�_���[�W20
		tmpUseItem = true;
	}

	if (_itemAction == ITEM::ENEMY_2)
	{
		PlaySoundMem(_soundSE[4], DX_PLAYTYPE_BACK, true);
		_nonDamageFlg = true; // ���̃^�[���͓G�̍U���𖳌���
		tmpUseItem = true;
	}

	if (_itemAction == ITEM::ENEMY_3)
	{
		PlaySoundMem(_soundSE[4], DX_PLAYTYPE_BACK, true);
		cards->SetTurn(monster->GetMaxTurn()+1);  // �v���C���[�̍s���\�^�[���𑝂₷
		tmpUseItem = true;
	}

	if (_itemAction == ITEM::ENEMY_4)
	{
		PlaySoundMem(_soundSE[4], DX_PLAYTYPE_BACK, true);
		_escapeFlg = true;	 // �퓬���瓦������
		tmpUseItem = true;
	}

	if (_itemAction == ITEM::POW_UP)
	{
		PlaySoundMem(_soundSE[3], DX_PLAYTYPE_BACK, true);
		_powUpNum = 5;		 // �ꎞ�I�ɍU����up
		tmpUseItem = true;
	}

	// �A�C�e���g�p���̏���
	if (tmpUseItem)
	{
		_itemAction = ITEM::NON;
		if (monster->GetEnemyState() == ENEMY_STATE::EXIST)
		{
			BattleLambda();
		}
	}
}

void Menu::Draw(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item, const std::shared_ptr<Monster>& monster)
{
	if (monster->GetEnemyState() != ENEMY_STATE::EXIST)
	{
		DrawGraph(0, 0, _menuImages["menuButton"], true);
	}
	else
	{
		// �A�C�e���̂ݎg�p�\
		DrawGraph(0, 0, _menuSelPNG[1], true);
	}

	// ���j���[��ʂ̕\��
	if (_menuBackPngFlg)
	{
		DrawGraph(200, 0, _menuImages["menuWindow"], true);
	}

	// ���j���[����
	if (_menuSelPngFlg)
	{
		for (int i = 0; i <= 2; i++)
		{
			DrawGraph(menu_pair[i].first.x, menu_pair[i].first.y, _menuSelPNG[i], true);
		}

		DrawGraph(menu_pair[3].first.x, menu_pair[3].first.y, _menuImages["titleBack"], true);	
		DrawGraph(menu_pair[4].first.x, menu_pair[4].first.y, _menuImages["save"], true);
	}

	// �A�C�e��
	if (_menu == MENU::ITEM)
	{
		// �g�Ɠ���A�C�e���̕`��
		for (int i = 0; i <= 11; i++)
		{
			DrawGraph(itemBox[i].pos.x, itemBox[i].pos.y, _menuImages["itembox"], true);
			// ���肵�Ă���A�C�e���̕`��(���Ă��Ȃ������牽���`�悳��Ȃ��悤�ɂ���)
			DrawGraph(itemBox[i].pos.x, itemBox[i].pos.y, itemBox[i].png, true);
		}

		DrawGraph(_choicePos.x, _choicePos.y, _menuImages["itemChoice"], true);

		// �������ꂽ�炻����E�̕������o��
		DrawFormatString(_equipSwordPos.x + 10, _equipSwordPos.y + 10, GetColor(255, 0, 0), "E");
		DrawFormatString(_equipShieldPos.x + 10, _equipShieldPos.y + 10, GetColor(255, 0, 0), "E");
	}

	// �X�e�[�^�X���
	if (_menu == MENU::STATUS)
	{
		int x = 350;
		int y = 100;
		int offset = 30;
		DrawFormatString(x, y + offset * 0, 0x000000, "���x��:%d"                  , player->GetNowLevel());
		DrawFormatString(x, y + offset * 1, 0x000000, "�̗�:%d / %d"               , player->GetHP(), player->GetMaxHP());
		DrawFormatString(x, y + offset * 2, 0x000000, "�U����:%d(+ %d) = %d"       , player->GetAttackDamage(), _equipDamage, player->GetAttackDamage() + _equipDamage);
		DrawFormatString(x, y + offset * 3, 0x000000, "�h���:%d(+ %d) = %d"       , player->GetDifense(), _equipGuard, player->GetDifense() + _equipGuard);
		DrawFormatString(x, y + offset * 4, 0x000000, "���̃��x���܂�:�c��%d"      , player->GetNextLevel());
		DrawFormatString(x, y + offset * 5, 0x000000, "������:%d�~"                , player->GetMoney());
		DrawFormatString(x, y + offset * 6, 0x000000, "�X�L���`���[�W�����܂�:%d��", player->GetSkillCharge());

		// �f�o�b�O���[�h�p�ɍU���͂������ݒ�ł���悤�ɂ��Ă���
		if (CheckHitKey(KEY_INPUT_F10) == 1)
		{
			player->SetAttackDamage(999);
		}
	}

	// ���j���[�̃X�e�[�^�X��ʂƃA�C�e����ʂ̕`��
	if (_menu == MENU::ITEM || _menu == MENU::STATUS)
	{
		// �߂�
		DrawGraph(375, 470, _menuImages["back"], true);

		if (!_useOrThrowAway)
		{
			return;
		}

		// �����̌��摜
		DrawGraph(0, 320, _menuImages["description"], true);

		// �A�C�e�����g�����ƂɌ��ʂ�����Ƃ�
		if (!_nonNeedFlg)
		{
			// �g��
			DrawGraph(50, 400, _menuImages["use"], true);
		}
		else
		{
			// �A�C�e�����g�����ƂɌ��ʂ��Ȃ��Ƃ�
			// �`��u�����h���[�h�ύX
			SetDrawBlendMode(DX_BLENDMODE_ADD, 100);
			// �g��
			DrawGraph(50, 400, _menuImages["use"], true);

			// �`��u�����h���[�h���m�[�u�����h�ɂ���
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			DrawFormatString(20, 300, 0xffffff, "���g���Ă����ʂ��Ȃ�");
		}

		// �̂Ă�
		DrawGraph(50, 500, _menuImages["throw_away"], true);
		// ���j���[�̃A�C�e���őI�������A�C�e���̐������o��
		if (_itemExplanation != ITEM::NON)
		{
			DrawFormatString(25, 340, 0x000000, "%s\n", item->GetExplanation(static_cast<int>(_itemExplanation) - 1));
		}
	}
}

void Menu::MenuButton_NonEnemy(const std::shared_ptr<MouseCtl>& mouse)
{
	// �N���b�N���Ă��Ȃ��Ƃ���return����
	if (!mouse->GetClickTrg())
	{
		return;
	}

	// �����\�����ɏォ�獀�ڂ��\������Ȃ��悤�ɂ���
	if (!_menuBackPngFlg)
	{
		if (mouse->GetPos().x >= 0 && mouse->GetPos().x <= 0 + 150 &&
			mouse->GetPos().y >= 0 && mouse->GetPos().y <= 0 + 75)
		{
			// �N���b�N��
			PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);

			_menuBackPngFlg = true;
			_menuSelPngFlg = true;
		}
	}

	if (_menuSelPngFlg)
	{
		// ���j���[���ڃ{�^���Ƃ̓����蔻��
		for (int i = 0; i <= 4; i++)
		{
			if (mouse->GetPos().x >= menu_pair[i].first.x && mouse->GetPos().x <= menu_pair[i].first.x + buttonSize[i].x &&
				mouse->GetPos().y >= menu_pair[i].first.y && mouse->GetPos().y <= menu_pair[i].first.y + buttonSize[i].y)
			{
				// �N���b�N��
				PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);

				_menu = menu_pair[i].second;
				_menuSelPngFlg = false;	// ��������
			}
		}
	}

	GameContinue(mouse);
}

void Menu::MenuButton_Enemy(const std::shared_ptr<MouseCtl>& mouse)
{
	// �N���b�N���Ă��Ȃ��Ƃ���return����
	if (!mouse->GetClickTrg())
	{
		return;
	}

	if (mouse->GetPos().x >= 0 && mouse->GetPos().x <= 0 + 200 &&
		mouse->GetPos().y >= 0 && mouse->GetPos().y <= 0 + 100)
	{
		// �N���b�N��
		PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);

		_menu = MENU::ITEM;
		_menuBackPngFlg = true;
	}

	GameContinue(mouse);
}

void Menu::SetItem(const ITEM& item, const int& png)
{
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
}

bool Menu::GetMenuFlg(void)const
{
	return _menuSelPngFlg;
}

MENU Menu::GetMenu(void)const
{
	return _menu;
}

int Menu::GetCanHaveItem(void)const
{
	int canHave = 0;
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

int Menu::GetEquipDamage(void)const
{
	return _equipDamage;
}

int Menu::GetEquipGuard(void)const
{
	return _equipGuard;
}

int Menu::GetPowUp(void)const
{
	return _powUpNum;
}

void Menu::SetPowUp(const int& num)
{
	// �U�����邲�Ƃ�-1����Ă���
	_powUpNum = num;
}

bool Menu::GetNonDamageFlg(void)const
{
	return _nonDamageFlg;
}

void Menu::SetNonDamageFlg(const bool& flag)
{
	_nonDamageFlg = flag;
}

bool Menu::GetMenuBackPngFlg(void)const
{
	return _menuBackPngFlg;
}

bool Menu::GetEscapeFlg(void)const
{
	return _escapeFlg;
}

void Menu::SetEscapeFlg(const bool& flag)
{
	_escapeFlg = flag;
}

bool Menu::GetAppraisalFlg(void)const
{
	return _AppraisalFlg;
}

void Menu::SetAppraisalFlg(const bool& flag)
{
	_AppraisalFlg = flag;
}

void Menu::Save(const std::shared_ptr<Player>& player)
{
	if (MessageBox(			// ���b�Z�[�W
		NULL,
		"���݂̏�Ԃ��Z�[�u���܂���?",
		"�m�F�_�C�A���O",
		MB_OKCANCEL
	) == IDOK)
	{
		// �Z�[�u������
		FILE* file;
		fopen_s(&file, "data/save1.csv", "wb");
		if (file == NULL)
		{
			return; // �G���[
		}
		fprintf(file, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n", player->GetNowLevel(), player->GetMaxHP(), player->GetHP(), player->GetAttackDamage(), player->GetDifense(), player->GetNextLevel(), player->GetMoney(), player->GetConditionTurn(), player->GetCondition());
		fprintf(file, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", itemBox[0]._item, itemBox[1]._item, itemBox[2]._item, itemBox[3]._item, itemBox[4]._item, itemBox[5]._item, itemBox[6]._item, itemBox[7]._item, itemBox[8]._item, itemBox[9]._item, itemBox[10]._item, itemBox[11]._item);
		fclose(file);
	}
}

void Menu::Load()
{
	if (MessageBox(			// ���b�Z�[�W
		NULL,
		"���[�h���܂���?",
		"�m�F�_�C�A���O",
		MB_OKCANCEL
	) == IDOK)
	{
		// ���[�h������
		// �t�@�C����ǂݍ���
		int FileHandle;
		FileHandle = FileRead_open("data/save1.csv");
		if (FileHandle == NULL)
		{
			return;	// �G���[
		}

		FileRead_scanf(FileHandle, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n", &Player::saveData[0], &Player::saveData[1], &Player::saveData[2], &Player::saveData[3], &Player::saveData[4], &Player::saveData[5], &Player::saveData[6], &Player::saveData[7], &Player::saveData[8]);
		FileRead_scanf(FileHandle, "%d,%d,%d,%d,%d,%d,%d,%d,%d", &itemBox[0]._item, &itemBox[1]._item, &itemBox[2]._item, &itemBox[3]._item, &itemBox[4]._item, &itemBox[5]._item, &itemBox[6]._item, &itemBox[7]._item, &itemBox[8]._item, &itemBox[9]._item, &itemBox[10]._item, &itemBox[11]._item);

		//�t�@�C�������
		FileRead_close(FileHandle);

		Player::loadFlg = true;
		_loadFlg = true;
		SelectScene::pushFlg = true;
	}
	else if (MessageBox(			// ���b�Z�[�W
		NULL,
		"�͂��߂��炵�܂���?",
		"�m�F�_�C�A���O",
		MB_OKCANCEL
	) == IDOK)
	{
		SelectScene::pushFlg = true;
	}
	else
	{
		SelectScene::pushFlg = false;
	}
}