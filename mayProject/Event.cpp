#include "Dxlib.h"
#include "Event.h"
#include "GameScene.h"
#include "Player.h"
#include "Cards.h"
#include "SelectScene.h"

Event::Event()
{
	Init();
}

Event::~Event()
{
	// ���֌W
	for (int i = 0; i < 4; i++)
	{
		DeleteSoundMem(_soundSE[i]);
	}
}

void Event::Init(void)
{
	_event = EVENT_STATE::NON;
	// �h���E���l
	_healYadoFlg = false;
	_nonMoneyFlg = false;
	_buyFlg = false;
	_chooseFlg = false;
	_chooseNum = -1;
	_itemNextPage = false;

	// �{�^���E�󔠁E���ݕ�
	_pushFlg = false;
	_fateNum = -1;

	// ���̑�
	_getFlg = false;
	_anounceFlg = false;
	_onceFlg = false;
	_kyouseiButtleFlg = false;

	// �󔠐ݒ�
	int chestHandle;
	// �{�^���ƈ��ݕ��̍��W�ݒ�
	int posHandle;

	if (SelectScene::modeTest == MODE::NORMAL)
	{
		chestHandle = FileRead_open("csv/chest1.csv");
		posHandle = FileRead_open("csv/buttonDrink1.csv");
		if (chestHandle == NULL || posHandle == NULL)
		{
			return; //�G���[���̏���
		}
	}
	else if (SelectScene::modeTest == MODE::HARD)
	{
		chestHandle = FileRead_open("csv/chest2.csv");
		posHandle = FileRead_open("csv/buttonDrink2.csv");
		if (chestHandle == NULL || posHandle == NULL)
		{
			return; //�G���[���̏���
		}
	}

	for (int i = 0; i < 4; i++)
	{
		FileRead_scanf(chestHandle, "%d,%d,%d,%d", &_chestOpen[i], &_chestBingo[i], &_chestPos[i].x, &_chestPos[i].y);
	}
	//�t�@�C�������
	FileRead_close(chestHandle);

	for (int i = 0; i < 4; i++)
	{
		FileRead_scanf(posHandle, "%d,%d", &_buttonDrink[i].x, &_buttonDrink[i].y);
	}

	//�t�@�C�������
	FileRead_close(posHandle);

	// �{�^��
	_buttonNum = -1;
	_buttonPush[0] = false;
	_buttonPush[1] = false;
	_buttonEventFlg = false;

	// ���ݕ�
	_drinkNum = -1;
	_drinking[0] = false;
	_drinking[1] = false;
	_drinkEventFlg = false;

	// �����g���b�v
	_trapFlg = false;
	_nowTrapFlg = false;

	// ����G
	_eventMonsFlg = false;
	_eventMonsEncountFlg = false;
	_eventMonsEndFlg = false;

	pngInit();

	// SE
	_soundSE[0] = LoadSoundMem("sound/se/click.mp3");
	_soundSE[1] = LoadSoundMem("sound/se/biribiri.mp3");
	_soundSE[2] = LoadSoundMem("sound/se/damage.mp3");
	_soundSE[3] = LoadSoundMem("sound/se/poison.mp3");
}

void Event::pngInit(void)
{
	eventImages.try_emplace("heal_human", LoadGraph("image/heal_human.png"));		// �h��
	eventImages.try_emplace("syounin", LoadGraph("image/syounin.png"));				// ���l
	eventImages.try_emplace("bin", LoadGraph("image/nazo_bin.png"));				// ���ݕ�
	eventImages.try_emplace("chestInItem", LoadGraph("image/tresure_1.png"));		// ��(�A�C�e���������Ă���)
	eventImages.try_emplace("chestKara", LoadGraph("image/chest_kara.png"));		// ���
	eventImages.try_emplace("zou", LoadGraph("image/daiza.png"));					// �����g���b�v
	eventImages.try_emplace("mons", LoadGraph("image/monster/event_monster.png"));	// ����G

	// ���b�Z�[�W
	std::string message = "image/message.png";
	_messagePNG = LoadGraph(message.c_str());
	// ���l�̎������̌��g
	std::string frame = "image/frame.png";
	_syouninWakuPNG = LoadGraph(frame.c_str());
	// �A�C�e���{�b�N�X�w�i
	std::string itembox = "image/itembox.png";
	_itemBoxPNG = LoadGraph(itembox.c_str());
	// �I�𒆂̃A�C�e��
	std::string itemChoice = "image/itemChoice.png";
	_itemChoicePNG = LoadGraph(itemChoice.c_str());
	// ����؂�̕���
	//std::string soldout = "image/soldout.png";
	//_soldOutPNG = LoadGraph(soldout.c_str());
	// ��
	std::string trasure_0 = "image/trasure_0.png";
	LoadDivGraph(trasure_0.c_str(), 2, 2, 1, 390 / 2, 431, _chestPNG);
	// �����摜�̕����ǂݍ���
	std::string sentakusi = "image/sentakusi/sentakusi.png";
	LoadDivGraph(sentakusi.c_str(),12, 12, 1, 150, 75, _sentakusiPNG);
	// ���i�y�[�W���ړ����邽�߂̖��̉摜
	std::string yajirusi = "image/yajirusi.png";
	_yajirusiPNG = LoadGraph(yajirusi.c_str());
}

void Event::UpDate(GameScene* game, Player* player, Menu* menu, Item* item, Monster* monster,Cards* cards)
{
	auto lambda = [&]() {
		monster->SetEnemyNum(6, 0);
		cards->SetTurn(3);
		_onceFlg = true;
		_kyouseiButtleFlg = true;
	};

	if (_event == EVENT_STATE::YADO)
	{
		if (_eventMonsFlg && !_onceFlg)
		{
			lambda();
		}
		else
		{
			Yado(game, player);
		}
	}

	if (_event == EVENT_STATE::SYOUNIN)
	{
		if (_eventMonsFlg && !_onceFlg)
		{
			lambda();
		}
		else
		{
			Syounin(game, player, menu, item);
		}
	}

	if (_event == EVENT_STATE::BUTTON)
	{
		if (_eventMonsFlg && !_onceFlg)
		{
			lambda();
		}
		else
		{
			Button(game, player);
		}
	}

	if (_event == EVENT_STATE::CHEST)
	{
		if (_eventMonsFlg && !_onceFlg)
		{
			lambda();
		}
		else
		{
			Chest(game, player, menu, item);
		}
	}

	if (_event == EVENT_STATE::DRINK)
	{
		if (_eventMonsFlg && !_onceFlg)
		{
			lambda();
		}
		else
		{
			Drink(game, player);
		}
	}

	if (_event == EVENT_STATE::TRAP)
	{
		Trap(game,player);
	}

	if (_event == EVENT_STATE::ENEMY)
	{
		Enemy(game, player, monster);
	}

	if (_event == EVENT_STATE::EVE_MONS)
	{
		eventMons(game, monster,cards);
	}
}

void Event::Draw(GameScene* game, Player* player, Menu* menu, Item* item)
{
	// �h��
	if (_event == EVENT_STATE::YADO && !_eventMonsFlg)
	{
		// �l�摜
		DrawGraph(0, 0, eventImages["heal_human"], true);
		// ����
		DrawGraph(600, 345, _sentakusiPNG[10], true);
		// ���b�Z�[�W�{�b�N�X
		DrawGraph(420, 50, _messagePNG, true);

		if (_nonMoneyFlg)
		{
			DrawFormatString(450, 70, 0x000000, "�������V�k:\n%���������悤����́B\n���邪�悢");
		}

		if (!_healYadoFlg)
		{
			// ����
			DrawGraph(600, 200, _sentakusiPNG[2], true);
			DrawFormatString(450, 70, 0x000000, "�������V�k:\n%d�~�ŉ񕜂��Ă�낤��? \n(������:%d�~)", player->GetNowLevel() * 100,player->GetMoney());
		}
		else
		{
			if (!_nonMoneyFlg)
			{
				DrawFormatString(450, 70, 0x000000, "�������V�k:\n˯˯˯˯...");
			}
		}
	}

	// ���l
	if (_event == EVENT_STATE::SYOUNIN && !_eventMonsFlg)
	{
		// �l�摜
		DrawGraph(100, 0, eventImages["syounin"], true);
		// ���b�Z�[�W�{�b�N�X
		DrawGraph(420, 50, _messagePNG, true);
		DrawFormatString(450, 70, 0x000000, "���l:\n����������?");

		if (_buyFlg)
		{
			DrawGraph(200, 0, _syouninWakuPNG, true);

			// 1�y�[�W��
			if (!_itemNextPage)
			{
				// �A�C�e���\��
				for (int i = 0; i <= 7; i++)
				{
					DrawGraph(item->GetPos(i).x, item->GetPos(i).y, _itemBoxPNG, true);
					DrawGraph(item->GetPos(i).x, item->GetPos(i).y, item->GetPair(i).first, true);
				}
			}
			else
			{
				// 2�y�[�W��
				// �A�C�e���\��
				for (int i = 0; i <= 7; i++)
				{
					DrawGraph(item->GetPos(i).x, item->GetPos(i).y, _itemBoxPNG, true);
					DrawGraph(item->GetPos(i).x, item->GetPos(i).y, item->GetPair(i+8).first, true);
				}
			}

			// �I�𒆂̃A�C�e���̘g
			if (_chooseNum >= 0)
			{
				DrawGraph(item->GetPos(_chooseNum).x, item->GetPos(_chooseNum).y, _itemChoicePNG, true);
			}

			// ���݂̏������̕\��
			DrawFormatString(470, 485, 0x000000, "������:%d�~", player->GetMoney());

			// ���i�̎��̃y�[�W�ւ̖��
			DrawGraph(490,300, _yajirusiPNG, true);
		}

		// ����
		DrawGraph(600, 345, _sentakusiPNG[10], true);

		if (!_buyFlg || _chooseFlg)
		{
			// �w��
			DrawGraph(600, 200, _sentakusiPNG[0], true);
			if (menu->GetCanHaveItem() == 0)
			{
				// ���������^����������ĂȂ���
				DrawFormatString(600, 160, 0xffffff, "�����i�������ς���");
			}
		}

		if (_buyFlg && _chooseFlg)
		{
			if (_nonMoneyFlg)
			{
				// ������������Ȃ���
				DrawFormatString(600, 180, 0xffffff, "������������Ȃ�");
			}
		}

		// �����\��(���i�I�𒆂̂�)
		if (_chooseNum != -1)
		{
			DrawFormatString(300, 450, 0x000000, "%s\n", item->GetSetumei(static_cast<int>(_itemInfo)-1));
			DrawFormatString(300, 470, 0x000000, "%d�~\n", item->GetCostMoney(static_cast<int>(_itemInfo) - 1));
		}
	}

	// �{�^���o����
	if (_event == EVENT_STATE::BUTTON && !_eventMonsFlg)
	{
		for (int i = 0; i < 4; i++)
		{
			if (_buttonDrink[i].x == game->plPosX && _buttonDrink[i].y == game->plPosY)
			{
				_buttonNum = i;
			}
		}

		if (!_buttonPush[_buttonNum])
		{
			// ���b�Z�[�W�{�b�N�X
			DrawGraph(420, 50, _messagePNG, true);

			if (_fateNum == -1)
			{
				// ����
				DrawGraph(600, 200, _sentakusiPNG[4], true);
				DrawFormatString(450, 70, 0x000000, "�ǂɃ{�^�������Ă���...\n");
			}

			if (_fateNum == 0)
			{
				DrawFormatString(450, 70, 0x000000, "�Ȃ��1000�~���łĂ���!\n");
			}

			if (_fateNum > 0)
			{
				DrawFormatString(450, 70, 0x000000, "�̒��ɓd�������ꂽ!!");
			}
		}

		// ����
		DrawGraph(600, 345, _sentakusiPNG[10], true);
	}

	// �󔠏o����
	if (_event == EVENT_STATE::CHEST && !_eventMonsFlg)
	{
		// ���b�Z�[�W�{�b�N�X
		DrawGraph(420, 50, _messagePNG, true);

		// �󔠃`�F�b�N
		int a = 0;
		for (int i = 0; i < 4; i++)
		{
			if (_chestPos[i].x == GameScene::plPosX && _chestPos[i].y == GameScene::plPosY)
			{
				a = i;
			}
		}
		if (_chestOpen[a] == 0)
		{		
			// �i��(�󔠖���)
			DrawGraph(600, 345, _sentakusiPNG[10], true);
		
			if (_fateNum == -1)
			{
				// �J����
				DrawGraph(600, 200, _sentakusiPNG[3], true);
				DrawGraph(350, 150, _chestPNG[0], true);
				DrawFormatString(450, 70, 0x000000, "�󔠂��u���Ă���");
			}
				
			// �Ӓ�A�C�e�����g�����Ƃ��̕`��
			if (menu->GetMeganeFlg())
			{
				if (_chestBingo[a] == 1)
				{
					DrawFormatString(450, 70, 0xff0000, "\n\n���ɂ��������Ƃ���͂Ȃ�");
				}
				if (_chestBingo[a] == 0)
				{
					DrawFormatString(450, 70, 0xff0000, "\n\n�S�[�X�g�����߂��Ă���");
				}
			}
		}
		else
		{
			if (_getFlg)
			{
				// ���
				DrawGraph(600, 200, _sentakusiPNG[8], true);
			}
		
			if (_anounceFlg)
			{
				// ���������^����������ĂȂ�
				DrawFormatString(600, 180, 0xffffff, "�����i�������ς���");
			}
		
			if (_fateNum == 1)
			{
				DrawGraph(350, 150, eventImages["chestInItem"], true);
				DrawFormatString(450, 70, 0x000000, "�A�C�e���������Ă���!");
			}
			else if (_fateNum == 0)
			{
				DrawGraph(350, 150, _chestPNG[1], true);
				DrawFormatString(450, 70, 0x000000, "�S�[�X�g������U�����Ă���!");
			}
			else
			{
				// �󔠂����łɊJ���Ă���
				DrawFormatString(450, 70, 0x000000, "�󔠂͊J���Ă���");
				DrawGraph(350, 150, eventImages["chestKara"], true);
			}
		
			// ����(�󔠖���)
			DrawGraph(600, 345, _sentakusiPNG[10], true);
		}
	}

	// �r�o����
	if (_event == EVENT_STATE::DRINK && !_eventMonsFlg)
	{
		for (int i = 0; i < 4; i++)
		{
			if (_buttonDrink[i].x == game->plPosX && _buttonDrink[i].y == game->plPosY)
			{
				_drinkNum = i;
			}
		}

		if (!_drinking[_drinkNum])
		{
			// ���b�Z�[�W�{�b�N�X
			DrawGraph(420, 50, _messagePNG, true);
			// �r�摜
			DrawGraph(350, 250, eventImages["bin"], true);
			if (_fateNum == -1)
			{
				// ����
				DrawGraph(600, 200, _sentakusiPNG[1], true);
				DrawFormatString(450, 70, 0x000000, "[Drink Me]\n�Ƃ����ꂽ�r������...");
			}

			if (_fateNum == 0)
			{
				DrawFormatString(450, 70, 0x000000, "�̂����ɂȂ���!\n�h��͂��オ����");
			}

			if (_fateNum > 0)
			{
				DrawFormatString(450, 70, 0x000000, "�łɂ������Ă��܂���...");
			}
		}
		// ����
		DrawGraph(600, 345, _sentakusiPNG[10], true);
	}

	// �����g���b�v�o����
	if (_event == EVENT_STATE::TRAP)
	{
		// ���b�Z�[�W�{�b�N�X
		DrawGraph(420, 50, _messagePNG, true);
		// ����
		DrawGraph(600, 345, _sentakusiPNG[10], true);
		// ���ׂ�
		DrawGraph(600, 200, _sentakusiPNG[9], true);
		DrawGraph(200, 75,eventImages["zou"], true);

		if (_nowTrapFlg)
		{
			if (_eventMonsEncountFlg && !_eventMonsEndFlg)
			{
				DrawFormatString(450, 70, 0x000000, "�w��ɓG�������Ă���c!");
			}
			else
			{
				DrawFormatString(450, 70, 0x000000, "�����g���b�v�̂悤��");
			}
		}
		else
		{
			DrawFormatString(450, 70, 0x000000, "�������ȑ�������");
			if (_eventMonsEncountFlg && !_eventMonsEndFlg)
			{
				DrawFormatString(450, 90, 0xff0000, "�w��ɓG�������Ă���c!");
			}
		}
	}

	// �C�x���g�G�Ƒ�����
	if (_event == EVENT_STATE::EVE_MONS || _event != EVENT_STATE::NON && _eventMonsFlg)
	{
		DrawGraph(100, 75, eventImages["mons"], true);

		// �퓬�ɓ���O
		if (!_eventMonsFlg)
		{
			// ���b�Z�[�W�{�b�N�X
			DrawGraph(420, 50, _messagePNG, true);
			// ����
			DrawGraph(600, 345, _sentakusiPNG[10], true);
			// ���ׂ�
			DrawGraph(600, 200, _sentakusiPNG[11], true);
			DrawFormatString(450, 70, 0x000000, "�G�������ӂ����ł���");
		}
	}

	// �C�x���g��NON�ł�eventmonFlg�����Ă���`�悵�Ă݂�(��ʂɒ���t��)
	if (_event == EVENT_STATE::NON && _eventMonsEncountFlg)
	{
		DrawGraph(100, 75, eventImages["mons"], true);
		// ���b�Z�[�W�{�b�N�X
		DrawGraph(420, 50, _messagePNG, true);
		DrawFormatString(450, 70, 0x000000, "�Ȃ�ƓG���ǂ������Ă���!");
	}
}

void Event::SetEvent(const EVENT_STATE& state)
{
	_event = state;
}

void Event::SetFateNum(const int& num)
{
	_fateNum = num;
}

void Event::SetReset(void)
{
	_onceFlg = false;

	// �{�^���̏�Ԃ����Z�b�g����
	_buttonPush[0] = false;
	_buttonPush[1] = false;

	// ���ݕ��̏�Ԃ����Z�b�g����
	_drinking[0] = false;
	_drinking[1] = false;

	// �󔠂̏�Ԃ����Z�b�g����
	for (int i = 0; i < 4; i++)
	{
		_chestOpen[i] = 0;
	}

	// �����g���b�v�������Ŏ��S���Ă�����V���ȓW�J��
	if (_trapFlg)
	{
		_nowTrapFlg = true;
	}
	_trapFlg = false;
}

bool Event::GetEventMonsFlg(void)const
{
	return _eventMonsFlg;
}

void Event::SetEventMonsFlg(const bool& flag)
{
	_eventMonsFlg = flag;
}

bool Event::GetEventMonsEndFlg(void)const
{
	return _eventMonsEndFlg;
}

void Event::SetEventMonsEncountFlg(const bool& flag)
{
	_eventMonsEncountFlg = flag;
}

bool Event::GetEventMonsEncountFlg(void)const
{
	return _eventMonsEncountFlg;
}

void Event::SetCautionFlg(const bool& flag)
{
	_kyouseiButtleFlg = flag;
}

bool Event::GetCautionFlg(void)const
{
	return _kyouseiButtleFlg;
}

void Event::Enemy(GameScene* game, Player* player, Monster* monster)
{
	game->eventState = EVENT_STATE::NON;
	_event = EVENT_STATE::NON;

	// �v���C���[next_level - �G�o���l
	// next_level <= 0�Ȃ�nowNum��+1���āA�X�e�[�^�X�X�V
	player->SetNextLevel(player->GetNextLevel() - monster->GetKeikenti());

	// �������𑝂₷
	player->SetMoney(player->GetMoney() + monster->GetMoney());
}

void Event::Yado(GameScene* game, Player* player)
{
	if (game->mouse & MOUSE_INPUT_LEFT) {			 //�}�E�X�̍��{�^����������Ă�����
		if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 345 && game->cursorPos.y <= 345 + 75)
		{
			// �N���b�N��
			PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
			// ����
			game->backFlg = true;
			game->eventState = EVENT_STATE::NON;
			_event = EVENT_STATE::NON;
			_healYadoFlg = false;
			_nonMoneyFlg = false;
		}

		// �񕜂𗊂�
		if (!_healYadoFlg)
		{
			if (player->GetMoney() < player->GetNowLevel() * 100)
			{
				// ������������Ȃ��ĉ񕜂ł��Ȃ�
				_healYadoFlg = true;
				_nonMoneyFlg = true;
			}

			if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 200 && game->cursorPos.y <= 200 + 75)
			{
				// �N���b�N��
				PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);

				// ���������邪HP�S��
				// ���邨���͂��̎��̎����̃��x���ɂ���ĈقȂ�
				player->SetMoney(player->GetMoney() - player->GetNowLevel() * 50);
				player->SetHP(player->GetMaxHP());
				_healYadoFlg = true;
			}
		}
	}
}

void Event::Syounin(GameScene* game, Player* player, Menu* menu, Item* item)
{
	if (game->mouse & MOUSE_INPUT_LEFT) {			 //�}�E�X�̍��{�^����������Ă�����
		if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 345 && game->cursorPos.y <= 345 + 75)
		{
			// �N���b�N��
			PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
			// ����
			game->backFlg = true;
			_chooseNum = -1;
			game->eventState = EVENT_STATE::NON;
			_event = EVENT_STATE::NON;
			_buyFlg = false;
			_chooseFlg = false;
			_nonMoneyFlg = false;
			// �A�C�e���̕�[����
			//for (int i = 0; i <= 10; i++)
			//{
				//if (item_pair[i].first == _soldOutPNG)
				//{
				//	item_pair[i].first = item[i].png;
				//	item_pair[i].second = static_cast<ITEM>(static_cast<int>(ITEM::POTION) + i);
				//	_itemPos[i] = { (((i % 3) + 2) * 100) + 90,((i / 3) + 1) * 100 };
				//}
				//
				// �A�C�e����[��3�ȏ�i��ł���̔��������ꕔ�ύX�����Ă���
				// soldout�̉摜�����ꂼ��ŗp�ӂ�������Ă�̂ŃA�h���X���Ⴄ����=�ɂȂ�Ȃ�
				//if (item->GetPair(i).second == ITEM::NON)
				//{
					//item->SetHojuPair(i,_nowEvent);
					//item->SetPos(i);
				//}
			//}
		}

		if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 200 && game->cursorPos.y <= 200 + 75)
		{
			if (!_buyFlg)
			{
				// �N���b�N��
				PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
				// �w������
				// �w��������v���C���[�̎������̒��ɒǉ�����
				_buyFlg = true;
			}
		}
	}

	// ���l�̎������Ƃ̓����蔻��
	if (_buyFlg)
	{
		if (game->mouse & MOUSE_INPUT_LEFT && !(game->mouseOld & MOUSE_INPUT_LEFT)) {			 //�}�E�X�̍��{�^����������Ă�����
			// ���̃y�[�W�ւ������ꂽ��
			if (game->cursorPos.x >= 490 && game->cursorPos.x <= 490 + 100 && game->cursorPos.y >= 300 && game->cursorPos.y <= 300 + 100)
			{
				_itemNextPage = !_itemNextPage;
			}

			for (int i = 0; i <= 7; i++)
			{
				if (game->cursorPos.x >= item->GetPos(i).x && game->cursorPos.x <= item->GetPos(i).x + 100 && game->cursorPos.y >= item->GetPos(i).y && game->cursorPos.y <= item->GetPos(i).y + 100)
				{
					if (item->GetPair(i).second != ITEM::NON)
					{
						// �w�����邩�̃{�^����\������
						_chooseFlg = true;
						_chooseNum = i;
						if (!_itemNextPage)
						{
							_itemInfo = item->GetPair(i).second;
						}
						else
						{
							_itemInfo = static_cast<ITEM>(static_cast<int>(item->GetPair(i).second) + 8);
						}
					}
				}
			}
		}
	}

	if (_chooseFlg)
	{
		// ������������Ȃ�
		if (player->GetMoney() < item->GetCostMoney(static_cast<int>(_itemInfo) - 1))
		{
			_nonMoneyFlg = true;
		}
		else
		{
			_nonMoneyFlg = false;
		}

		if (game->mouse & MOUSE_INPUT_LEFT) {			 //�}�E�X�̍��{�^����������Ă�����
			if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 200 && game->cursorPos.y <= 200 + 75)
			{
				// �N���b�N��
				PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);

				if (item->GetPair(_chooseNum).second != ITEM::NON)
				{
					// choosenum��static_cast<int>(_itemInfo) - 1�ɕύX
					if (player->GetMoney() < item->GetCostMoney(static_cast<int>(_itemInfo) - 1))
					{
						_chooseFlg = false;
					}
					else
					{
						// �����i�������ς��œ�����Ȃ�
						if (menu->GetCanHaveItem() == 0)
						{
							_chooseFlg = false;
						}
						else
						{
							// �v���C���[�̂���������
							// _chooseNum���ƁA�����ɑΉ��ł��Ȃ�
							player->SetMoney(player->GetMoney() - item->GetCostMoney(static_cast<int>(_itemInfo) - 1));
							if (!_itemNextPage)
							{
								menu->Setitem(item->GetPair(_chooseNum).second, item->GetPair(_chooseNum).first);
								//item->SetSoldOutPair(_chooseNum);
							}
							else
							{
								menu->Setitem(item->GetPair(_chooseNum+8).second, item->GetPair(_chooseNum+8).first);
								//item->SetSoldOutPair(_chooseNum+8);
							}
							_chooseFlg = false;
						}
					}
				}
			}
		}
	}
}

void Event::Button(GameScene* game, Player* player)
{
	if (game->mouse & MOUSE_INPUT_LEFT) {			 //�}�E�X�̍��{�^����������Ă�����
		if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 345 && game->cursorPos.y <= 345 + 75)
		{
			// �N���b�N��
			PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
			// ����
			game->backFlg = true;
			game->eventState = EVENT_STATE::NON;
			_event = EVENT_STATE::NON;
			_fateNum = -1;
			_buttonEventFlg = false;
			if (_pushFlg)
			{
				_buttonPush[_buttonNum] = true;
				_pushFlg = false;
			}
		}

		// ����
		for (int i = 0; i < 4; i++)
		{
			if (_buttonDrink[i].x == game->plPosX && _buttonDrink[i].y == game->plPosY)
			{
				_buttonNum = i;
			}
		}

		if (_fateNum == -1 && !_buttonPush[_buttonNum])
		{
			if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 200 && game->cursorPos.y <= 200 + 75)
			{
				// �N���b�N��
				PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
				_pushFlg = true;
				_fateNum = GetRand(2);	// 0 ~ 2
			}
		}
	}

	// �{�^�����������Ƃɂ����Ƃ�
	if (_pushFlg && !_buttonEventFlg)
	{
		if (_fateNum == 0)
		{
			// �������炦��
			player->SetMoney(player->GetMoney() + 1000);
			//_pushFlg = false;
			_buttonEventFlg = true;
		}
		else
		{
			// �d����
			PlaySoundMem(_soundSE[1], DX_PLAYTYPE_BACK, true);
			// �̗͂�1/4���
			player->SetHP(player->GetHP() - (float)player->GetMaxHP() * (1.0f / 4.0f));
			//_pushFlg = false;
			game->shakeFlg = true;
			_buttonEventFlg = true;
		}
	}
}

void Event::Chest(GameScene* game, Player* player, Menu* menu, Item* item)
{
	if (game->mouse & MOUSE_INPUT_LEFT) {			 //�}�E�X�̍��{�^����������Ă�����
		if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 345 && game->cursorPos.y <= 345 + 75)
		{
			// �N���b�N��
			PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
			// ����
			game->backFlg = true;
			game->eventState = EVENT_STATE::NON;
			_event = EVENT_STATE::NON;
			_pushFlg = false;
			_fateNum = -1;
			_anounceFlg = false;
			_getFlg = false;
			if (menu->GetMeganeFlg())
			{
				menu->SetMeganeFlg(false);
			}
		}

		// �󔠃`�F�b�N
		int a = 0;
		for (int i = 0; i < 4; i++)
		{
			if (_chestPos[i].x == GameScene::plPosX && _chestPos[i].y == GameScene::plPosY)
			{
				a = i;
			}
		}
		// �J����
		if (_fateNum == -1 && _chestOpen[a] == 0)
		{
			if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 200 && game->cursorPos.y <= 200 + 75)
			{
				_pushFlg = true;
				_chestOpen[a] = 1;
				_fateNum = _chestBingo[a];
				if (menu->GetMeganeFlg())
				{
					menu->SetMeganeFlg(false);
				}
			}
		}
	}

	// �󔠂��J���邱�Ƃɂ����Ƃ�
	if (_pushFlg)
	{
		if (_fateNum == 1)
		{
			_pushFlg = false;
			_getFlg = true;
		}
		else if(_fateNum == 0)
		{
			// �_���[�W��
			PlaySoundMem(_soundSE[2], DX_PLAYTYPE_BACK, true);
			// �̗͂�1/4���
			player->SetHP(player->GetHP() - (float)player->GetMaxHP() * (1.0f / 4.0f));
			_pushFlg = false;
			game->shakeFlg = true;
		}
	}

	if (_getFlg)
	{
		// �����������^�����ǂ������ׂ�
		if (menu->GetCanHaveItem() != 0)
		{
			_anounceFlg = false;
		}
		else
		{
			_anounceFlg = true;
		}

		if (game->mouse & MOUSE_INPUT_LEFT) {			 //�}�E�X�̍��{�^����������Ă�����
			if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 200 && game->cursorPos.y <= 200 + 75)
			{
				if (menu->GetCanHaveItem() != 0)
				{
					// �N���b�N��
					PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
					int randNum = GetRand(4);
					switch (randNum)	// 0~3
					{
					case 0:
						// �����������^������Ȃ���Ύ��Ă�
						menu->Setitem(item->GetPair(static_cast<int>(ITEM::POTION_BIG) - 1).second, item->GetPair(static_cast<int>(ITEM::POTION_BIG) - 1).first);
						break;
					case 1:
						// �����������^������Ȃ���Ύ��Ă�
						menu->Setitem(item->GetPair(static_cast<int>(ITEM::DETOX) - 1).second, item->GetPair(static_cast<int>(ITEM::DETOX) - 1).first);
						break;
					case 2:
						// �����������^������Ȃ���Ύ��Ă�
						menu->Setitem(item->GetPair(static_cast<int>(ITEM::KYOUKA_POW) - 1).second, item->GetPair(static_cast<int>(ITEM::KYOUKA_POW) - 1).first);
						break;
					case 3:
						// �����������^������Ȃ���Ύ��Ă�
						menu->Setitem(item->GetPair(static_cast<int>(ITEM::HEART) - 1).second, item->GetPair(static_cast<int>(ITEM::HEART) - 1).first);
						break;
					default:
						// �����������^������Ȃ���Ύ��Ă�
						menu->Setitem(item->GetPair(static_cast<int>(ITEM::POTION_BIG) - 1).second, item->GetPair(static_cast<int>(ITEM::POTION_BIG) - 1).first);
					}
					_getFlg = false;
					_anounceFlg = false;
				}
			}
		}
	}
}

void Event::Drink(GameScene* game, Player* player)
{
	if (game->mouse & MOUSE_INPUT_LEFT) {			 //�}�E�X�̍��{�^����������Ă�����
		if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 345 && game->cursorPos.y <= 345 + 75)
		{
			// �N���b�N��
			PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
			// ����
			game->backFlg = true;
			game->eventState = EVENT_STATE::NON;
			_event = EVENT_STATE::NON;
			_fateNum = -1;

			_drinkEventFlg = false;
			if (_pushFlg)
			{
				_drinking[_drinkNum] = true;
				_pushFlg = false;
			}
		}

		for (int i = 0; i < 4; i++)
		{
			if (_buttonDrink[i].x == game->plPosX && _buttonDrink[i].y == game->plPosY)
			{
				_drinkNum = i;
			}
		}

		// ����
		if (_fateNum == -1 && !_drinking[_drinkNum])
		{
			if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 200 && game->cursorPos.y <= 200 + 75)
			{
				// �N���b�N��
				PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
				_pushFlg = true;
				//_fateNum = GetRand(2);	// 0 ~ 2
				_fateNum = 1;
			}
		}
	}

	// ���ނ��Ƃɂ����Ƃ�
	if (_pushFlg && !_drinkEventFlg)
	{
		if (_fateNum == 0)
		{
			// ���b�L�[!��b�h��͂��オ��
			player->SetDifense(player->GetDifense() + 2);
			//_pushFlg = false;
			_drinkEventFlg = true;
		}
		else
		{
			// �ŉ�
			PlaySoundMem(_soundSE[3], DX_PLAYTYPE_BACK, true);
			// �łɂ�����
			player->SetCondition(CONDITION::POISON);
			//_pushFlg = false;
			game->shakeFlg = true;
			_drinkEventFlg = true;
		}
	}
}

void Event::Trap(GameScene* game, Player* player)
{
	if (game->mouse & MOUSE_INPUT_LEFT) {			 //�}�E�X�̍��{�^����������Ă�����
		if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 345 && game->cursorPos.y <= 345 + 75)
		{
			// �N���b�N��
			PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
			game->backFlg = true;
			game->eventState = EVENT_STATE::NON;
			_event = EVENT_STATE::NON;
			_fateNum = -1;
		}

		// ���ׂ�
		if (!_trapFlg)
		{
			if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 200 && game->cursorPos.y <= 200 + 75)
			{
				// �N���b�N��
				PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
				_trapFlg = true;
			}
		}
	}

	// ���ׂ邱�Ƃɂ����Ƃ�
	if (_trapFlg)
	{
		// �����g���b�v�̔���
		player->SetHP(player->GetHP() - player->GetMaxHP());

		// �Ƃ肠���������œ���G�C�x���g���I��������
		if (_eventMonsEncountFlg && !_eventMonsEndFlg)
		{
			_eventMonsEncountFlg = false;
			_eventMonsEndFlg = true;
		}
	}
}

void Event::eventMons(GameScene* game, Monster* monster, Cards* cards)
{
	_eventMonsEncountFlg = true;
	if (game->mouse & MOUSE_INPUT_LEFT) {			 //�}�E�X�̍��{�^����������Ă�����
		if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 345 && game->cursorPos.y <= 345 + 75)
		{
			// �N���b�N��
			PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
			// ����
			game->backFlg = true;
			game->eventState = EVENT_STATE::NON;
			_event = EVENT_STATE::NON;
			_fateNum = -1;
		}

		// �키
		if (!_eventMonsFlg)
		{
			if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 200 && game->cursorPos.y <= 200 + 75)
			{
				// �N���b�N��
				PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
				_eventMonsFlg = true;
				monster->SetEnemyNum(6, 0);
				cards->SetTurn(3);
			}
		}
	}
}