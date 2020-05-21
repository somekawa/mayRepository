#include "Dxlib.h"
#include "Event.h"
#include "GameScene.h"
#include "Player.h"
#include "Cards.h"

Event::Event()
{
	Init();
}

Event::~Event()
{
	// ���֌W
	for (int i = 0; i < 6; i++)
	{
		DeleteSoundMem(_soundSE[i]);
	}
}

void Event::Init(void)
{
	_event = EVENT_STATE::NON;
	_nowEvent = 0;
	_chooseNum = -1;
	_buyFlg = false;
	_chooseFlg = false;
	_healYadoFlg = false;	
	_nonMoneyFlg = false;
	_pushFlg = false;
	_fateNum = -1;
	_getFlg = false;
	_anounceFlg = false;
	_soundWalk = false;

	pngInit();

	// SE
	_soundSE[0] = LoadSoundMem("sound/se/click.mp3");
	_soundSE[1] = LoadSoundMem("sound/se/biribiri.mp3");
	_soundSE[2] = LoadSoundMem("sound/se/walk_short.mp3");
	_soundSE[3] = LoadSoundMem("sound/se/damage.mp3");
	_soundSE[4] = LoadSoundMem("sound/se/poison.mp3");
	_soundSE[5] = LoadSoundMem("sound/se/door.mp3");

	// �󔠐ݒ�
	auto testFileHandle = FileRead_open("csv/chest.csv");
	if (testFileHandle == NULL)
	{
		return; //�G���[���̏���
	}

	for (int i = 0; i < 2; i++)
	{
		FileRead_scanf(testFileHandle, "%d,%d,%d,%d", &chestOpen[i], &chestBingo[i], &chestPos[i].x, &chestPos[i].y);
	}

	//�t�@�C�������
	FileRead_close(testFileHandle);
}

void Event::pngInit(void)
{
	// �h��
	std::string heal_human = "image/heal_human.png";
	_healHumanPNG = LoadGraph(heal_human.c_str());
	// ���l
	std::string syounin = "image/syounin.png";
	_syouninPNG = LoadGraph(syounin.c_str());
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
	std::string soldout = "image/soldout.png";
	_soldOutPNG = LoadGraph(soldout.c_str());
	// ��
	std::string trasure_0 = "image/trasure_0.png";
	LoadDivGraph(trasure_0.c_str(), 2, 2, 1, 390 / 2, 431, _chestPNG);
	// ��(�A�C�e����)
	std::string tresure_1 = "image/tresure_1.png";
	_chsetItemPNG = LoadGraph(tresure_1.c_str());
	// ��̕r
	std::string nazo_bin = "image/nazo_bin.png";
	_drinkPNG = LoadGraph(nazo_bin.c_str());
	// �����摜�̕����ǂݍ���
	std::string sentakusi = "image/sentakusi/sentakusi.png";
	LoadDivGraph(sentakusi.c_str(),12, 12, 1, 150, 75, _sentakusiPNG);
	// ���
	std::string yajirusi = "image/yajirusi.png";
	yajirusiPNG = LoadGraph(yajirusi.c_str());
	// ��̕�
	std::string chest_kara = "image/chest_kara.png";
	karaPNG = LoadGraph(chest_kara.c_str());
	// �����g���b�v�̑�
	std::string zou = "image/daiza.png";
	zouPNG = LoadGraph(zou.c_str());
	// �C�x���g�G
	std::string eveMons = "image/monster/event_monster.png";
	eventMonsPNG = LoadGraph(eveMons.c_str());
}

void Event::UpDate(GameScene* game, Player* player, Menu* menu, Item* item, Monster* monster,Cards* cards)
{
	if (_event == EVENT_STATE::YADO)
	{
		Yado(game,player);
	}

	if (_event == EVENT_STATE::SYOUNIN)
	{
		Syounin(game, player,menu,item);
	}

	if (_event == EVENT_STATE::BUTTON)
	{
		Button(game, player);
	}

	if (_event == EVENT_STATE::CHEST)
	{
		Chest(game, player, menu, item);
	}

	if (_event == EVENT_STATE::DRINK)
	{
		Drink(game, player);
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
	// ���s������h�A���܂ł̂Ȃ��p
	// 1:�Đ��� 0:�Đ����Ă��Ȃ�
	if (CheckSoundMem(_soundSE[2]) == 1 && _soundWalk)
	{
		_soundWalk = true;
		_seCnt = 20;
	}
	else if (CheckSoundMem(_soundSE[2]) == 0 && _soundWalk)
	{
		if (_seCnt >= 0)
		{
			_seCnt--;
		}
		else
		{
			// �h�A��
			PlaySoundMem(_soundSE[5], DX_PLAYTYPE_BACK, true);
			_soundWalk = false;
		}
	}
}

void Event::Draw(GameScene* game, Player* player, Menu* menu, Item* item)
{
	// �h��
	if (_event == EVENT_STATE::YADO)
	{
		// �l�摜
		DrawGraph(0, 0, _healHumanPNG, true);
		// �i��
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
	if (_event == EVENT_STATE::SYOUNIN)
	{
		// �l�摜
		DrawGraph(100, 0, _syouninPNG, true);
		// ���b�Z�[�W�{�b�N�X
		DrawGraph(420, 50, _messagePNG, true);
		DrawFormatString(450, 70, 0x000000, "���l:\n����������?");

		if (_buyFlg)
		{
			DrawGraph(200, 0, _syouninWakuPNG, true);

			// �g�̕`��
			//for (int y = 100; y <= 400; y += 100)
			//{
			//	for (int x = 290; x <= 500; x += 100)
			//	{
			//		DrawGraph(x, y, _itemBoxPNG, true);
			//	}
			//}

			// 1�y�[�W��
			if (!_itemNextPage)
			{
				// �A�C�e���\��
				for (int i = 0; i <= 7; i++)
				{
					DrawGraph(item->GetPos(i).x, item->GetPos(i).y, _itemBoxPNG, true);
					DrawGraph(item->GetPos(i).x, item->GetPos(i).y, item->GetPair(i).first, true);

					//DrawGraph(_itemPos[i].x, _itemPos[i].y, _itemBoxPNG, true);
					//DrawGraph(_itemPos[i].x, _itemPos[i].y, item_pair[i].first, true);
				}
			}
			else
			{
				// 2�y�[�W��
				// �A�C�e���\��
				for (int i = 0; i <= 3; i++)
				{
					DrawGraph(item->GetPos(i).x, item->GetPos(i).y, _itemBoxPNG, true);
					DrawGraph(item->GetPos(i).x, item->GetPos(i).y, item->GetPair(i+8).first, true);
				}
			}
			//DrawGraph(_itemPos[0].x, _itemPos[0].y, item_pair[0].first, true);

			// �I�𒆂̃A�C�e���̘g
			if (_chooseNum >= 0)
			{
				DrawGraph(item->GetPos(_chooseNum).x, item->GetPos(_chooseNum).y, _itemChoicePNG, true);
				//DrawGraph(_itemPos[_chooseNum].x, _itemPos[_chooseNum].y, _itemChoicePNG, true);
			}

			// ���݂̏������̕\��
			DrawFormatString(470, 485, 0x000000, "������:%d�~", player->GetMoney());

			// ���i�̎��̃y�[�W�ւ̖��
			DrawGraph(490,300, yajirusiPNG, true);
		}

		// �i��
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
			//DrawFormatString(300, 450, 0xFFFFFF, "%s\n", item->GetSetumei(_chooseNum));
			//DrawFormatString(300, 470, 0xFFFFFF, "%d�~\n", item->GetCostMoney(_chooseNum));
			DrawFormatString(300, 450, 0x000000, "%s\n", item->GetSetumei(static_cast<int>(_itemInfo)-1));
			DrawFormatString(300, 470, 0x000000, "%d�~\n", item->GetCostMoney(static_cast<int>(_itemInfo) - 1));

			//DrawFormatString(300, 450, 0xFFFFFF, "%s\n", item[_chooseNum].setumei);
			//DrawFormatString(300, 470, 0xFFFFFF, "%d�~\n", item[_chooseNum].itemCostMoney);
			//DrawFormatString(150, 130, 0xFFFFFF, "%s\n", item[1].setumei);
			//DrawFormatString(150, 160, 0xFFFFFF, "%s\n", item[2].setumei);
		}
	}

	// �{�^���o����
	if (_event == EVENT_STATE::BUTTON)
	{
		// ���b�Z�[�W�{�b�N�X
		DrawGraph(420, 50, _messagePNG, true);

		// �i��(�{�^������)
		DrawGraph(600, 345, _sentakusiPNG[10], true);

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

	// �󔠏o����
	if (_event == EVENT_STATE::CHEST)
	{
		// ���b�Z�[�W�{�b�N�X
		DrawGraph(420, 50, _messagePNG, true);

		// �󔠃`�F�b�N
		int a = 0;
		for (int i = 0; i < 2; i++)
		{
			if (chestPos[i].x == GameScene::testx && chestPos[i].y == GameScene::testy)
			{
				a = i;
			}
		}
		//if (chestPos[0].x == GameScene::testx && chestPos[0].y == GameScene::testy)
		//{
			if (chestOpen[a] == 0)
			{
				//if (_getFlg)
				//{
				//	// ���
				//	DrawGraph(600, 200, _sentakusiPNG[8], true);
				//}
				//
				//if (_anounceFlg)
				//{
				//	// ���������^����������ĂȂ�
				//	DrawFormatString(600, 180, 0xffffff, "�����i�������ς���");
				//}
		
				// �i��(�󔠖���)
				DrawGraph(600, 345, _sentakusiPNG[10], true);
		
				if (_fateNum == -1)
				{
					// �J����
					DrawGraph(600, 200, _sentakusiPNG[3], true);
					DrawGraph(350, 150, _chestPNG[0], true);
					DrawFormatString(450, 70, 0x000000, "�󔠂��u���Ă���");
				}
		
				//if (_fateNum == 1)
				//{
				//	DrawGraph(350, 150, _chsetItemPNG, true);
				//	DrawFormatString(450, 70, 0x000000, "�A�C�e���������Ă���!");
				//}
				//
				//if (_fateNum == 0)
				//{
				//	DrawGraph(350, 150, _chestPNG[1], true);
				//	DrawFormatString(450, 70, 0x000000, "������S�[�X�g������\n���Ȃ��ɍU�����Ă���!");
				//}
		
				// �Ӓ�A�C�e�����g�����Ƃ��̕`��
				if (menu->GetMeganeFlg())
				{
					//if (game->chestFate == 0)
					//{
					//	DrawFormatString(450, 70, 0xff0000, "\n\n���ɂ��������Ƃ���͂Ȃ�");
					//}
					//
					//if (game->chestFate > 0)
					//{
					//	DrawFormatString(450, 70, 0xff0000, "\n\n�S�[�X�g��������");
					//}
		
					if (chestBingo[a] == 1)
					{
						DrawFormatString(450, 70, 0xff0000, "\n\n���ɂ��������Ƃ���͂Ȃ�");
					}
		
					if (chestBingo[a] == 0)
					{
						DrawFormatString(450, 70, 0xff0000, "\n\n�S�[�X�g��������");
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
					DrawGraph(350, 150, _chsetItemPNG, true);
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
					DrawGraph(350, 150, karaPNG, true);
				}
		
				// �i��(�󔠖���)
				DrawGraph(600, 345, _sentakusiPNG[10], true);
			}
		//}
	}

	// �r�o����
	if (_event == EVENT_STATE::DRINK)
	{
		// ���b�Z�[�W�{�b�N�X
		DrawGraph(420, 50, _messagePNG, true);
		// �r�摜
		DrawGraph(350, 250, _drinkPNG, true);
		// �i��(�r����)
		DrawGraph(600, 345, _sentakusiPNG[10], true);

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

	// �����g���b�v�o����
	if (_event == EVENT_STATE::TRAP)
	{
		// ���b�Z�[�W�{�b�N�X
		DrawGraph(420, 50, _messagePNG, true);
		// �i��
		DrawGraph(600, 345, _sentakusiPNG[10], true);
		// ���ׂ�
		DrawGraph(600, 200, _sentakusiPNG[9], true);
		DrawGraph(200, 75,zouPNG, true);

		if (nowTrapFlg)
		{
			if (eventmonsencountFlg && !eventmonsEndFlg)
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
			if (eventmonsencountFlg && !eventmonsEndFlg)
			{
				DrawFormatString(450, 90, 0xff0000, "�w��ɓG�������Ă���c!");
			}
		}
	}

	// �C�x���g�G�Ƒ�����
	if (_event == EVENT_STATE::EVE_MONS)
	{
		DrawGraph(100, 75, eventMonsPNG, true);

		// �퓬�ɓ���O
		if (!eventmonsFlg)
		{
			// ���b�Z�[�W�{�b�N�X
			DrawGraph(420, 50, _messagePNG, true);
			// ����
			DrawGraph(600, 345, _sentakusiPNG[10], true);
			// ���ׂ�
			DrawGraph(600, 200, _sentakusiPNG[11], true);
			DrawFormatString(450, 70, 0x000000, "�������ȓG������");
		}
	}

	// �C�x���g��NON�ł�eventmonFlg�����Ă���`�悵�Ă݂�(��ʂɒ���t��)
	if (_event == EVENT_STATE::NON && eventmonsencountFlg)
	{
		DrawGraph(100, 75, eventMonsPNG, true);
		// ���b�Z�[�W�{�b�N�X
		DrawGraph(420, 50, _messagePNG, true);
		DrawFormatString(450, 70, 0x000000, "�Ȃ�ƓG���ǂ������Ă���!");
	}

}

void Event::SetEvent(EVENT_STATE state)
{
	_event = state;
}

int Event::GetNowEvent(void)
{
	return _nowEvent;
}

void Event::SetNowEvent(int num)
{
	_nowEvent = num;
}

void Event::SetFateNum(int num)
{
	_fateNum = num;
}

void Event::SetReset(void)
{
	// �󔠂̏�Ԃ����Z�b�g����
	for (int i = 0; i < 2; i++)
	{
		chestOpen[i] = 0;
	}

	// �����g���b�v�������Ŏ��S���Ă�����V���ȓW�J��
	if (trapFlg)
	{
		nowTrapFlg = true;
	}
	trapFlg = false;
}

bool Event::GetEventMonsFlg(void)
{
	return eventmonsFlg;
}

void Event::SetEventMonsFlg(bool flag)
{
	eventmonsFlg = flag;
}

bool Event::GetEventMonsEndFlg(void)
{
	return eventmonsEndFlg;
}

void Event::SetEventMonsEncountFlg(bool flag)
{
	eventmonsencountFlg = flag;
}

bool Event::GetEventMonsEncountFlg(void)
{
	return eventmonsencountFlg;
}

void Event::Enemy(GameScene* game, Player* player, Monster* monster)
{
	_nowEvent++;

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
		// �񕜂���Ȃ�
		if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 345 && game->cursorPos.y <= 345 + 75)
		{
			// �N���b�N��
			PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
			// ���s��
			PlaySoundMem(_soundSE[2], DX_PLAYTYPE_BACK, true);

			game->_backFlg = true;

			// ��ɐi��
			game->walkCnt++;
			//game->moveFlg = true;
			game->eventState = EVENT_STATE::NON;
			_event = EVENT_STATE::NON;
			_nowEvent++;
			_healYadoFlg = false;
			_nonMoneyFlg = false;
			_soundWalk = true;
		}

		// �񕜂��Ăق���
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

				// ���������邪HP���^��!
				// ���邨���͂��̎��̎����̃��x���ɂ���ĈقȂ�
				player->SetMoney(player->GetMoney() - player->GetNowLevel() * 100);
				player->SetHP(player->GetMaxHP());
				_healYadoFlg = true;
			}
		}
	}
}

void Event::Syounin(GameScene* game, Player* player, Menu* menu, Item* item)
{
	if (game->mouse & MOUSE_INPUT_LEFT) {			 //�}�E�X�̍��{�^����������Ă�����
		// ��������Ȃ�
		if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 345 && game->cursorPos.y <= 345 + 75)
		{
			// �N���b�N��
			PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
			// ���s��
			PlaySoundMem(_soundSE[2], DX_PLAYTYPE_BACK, true);

			game->_backFlg = true;

			// ��ɐi��
			game->walkCnt++;
			_chooseNum = -1;		// -1�ɂ��Ȃ��ƁA�I��������ԂŐi�ނ��������玟�̏��l��soldout�ɂȂ����Ⴄ
			game->eventState = EVENT_STATE::NON;
			_event = EVENT_STATE::NON;
			_nowEvent++;
			_buyFlg = false;
			_chooseFlg = false;
			_nonMoneyFlg = false;
			//game->moveFlg = true;
			_soundWalk = true;

			// �A�C�e���̕�[����
			for (int i = 0; i <= 10; i++)
			{
				//if (item_pair[i].first == _soldOutPNG)
				//{
				//	item_pair[i].first = item[i].png;
				//	item_pair[i].second = static_cast<ITEM>(static_cast<int>(ITEM::POTION) + i);
				//	_itemPos[i] = { (((i % 3) + 2) * 100) + 90,((i / 3) + 1) * 100 };
				//}

				// �A�C�e����[��3�ȏ�i��ł���̔��������ꕔ�ύX�����Ă���
				// soldout�̉摜�����ꂼ��ŗp�ӂ�������Ă�̂ŃA�h���X���Ⴄ����=�ɂȂ�Ȃ�
				//if (item->GetPair(i).second == ITEM::NON)
				//{
					item->SetHojuPair(i,_nowEvent);
					item->SetPos(i);
				//}
			}

		}

		if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 200 && game->cursorPos.y <= 200 + 75)
		{
			if (!_buyFlg)
			{
				// �N���b�N��
				PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);

				// �w������
				// �w�i�g���o���Ă��̒��ɏ��i�����j���[�Ɠ������ɕ`��
				// �w��������v���C���[�̎������̒��ɒǉ�����
				_buyFlg = true;
			}
		}
	}

	// ���l�̎������Ƃ̓����蔻��
	if (_buyFlg)
	{
		if (game->mouse & MOUSE_INPUT_LEFT && !(game->mouseOld & MOUSE_INPUT_LEFT)) {			 //�}�E�X�̍��{�^����������Ă�����

			//if (cursorPos.x >= _itemPos[0].x && cursorPos.x <= _itemPos[0].x + 100 && cursorPos.y >= _itemPos[0].y && cursorPos.y <= _itemPos[0].y + 100)
			//{
			//	if (item_pair[0].second != ITEM::NON)
			//	{
			//		// �{���͂����ŕK�v�Ȃ����ƌ��ʂ�\�����āA�w�����邩��߂邩�̃{�^����\������
			//		// �{�^���́A�w���Ɛi�ނ�ύX����ʒu�ɕ`�悷��
			//
			//		// �v���C���[�̃��j���[�ɕ\�������悤�ɂ���
			//		//_menu->SetPNG(item_pair[0].first);
			//		//_menu->Setitem(item_pair[0].second);
			//
			//		// �������Ƃ���ɂ�soldout�ł������Ƃ��āAitem��non�ɂ���
			//		//item_pair[0].first = _soldOutPNG;
			//		//item_pair[0].second = ITEM::NON;
			//	}
			//}

			// ���̃y�[�W�ւ������ꂽ��
			if (game->cursorPos.x >= 490 && game->cursorPos.x <= 490 + 100 && game->cursorPos.y >= 300 && game->cursorPos.y <= 300 + 100)
			{
				_itemNextPage = !_itemNextPage;
			}

			for (int i = 0; i <= 7; i++)
			{
				//if (cursorPos.x >= _itemPos[i].x && cursorPos.x <= _itemPos[i].x + 100 && cursorPos.y >= _itemPos[i].y && cursorPos.y <= _itemPos[i].y + 100)
				//{
				//	if (item_pair[i].second != ITEM::NON)
				//	{
				//		// �w�����邩�̃{�^����\������
				//		// �{�^���́A�w���Ɛi�ނ�ύX����ʒu�ɕ`�悷��
				//		_chooseFlg = true;
				//		_chooseNum = i;
				//	}
				//}
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

				//if (item_pair[_chooseNum].second != ITEM::NON)
				//{
				//	// ����������Ȃ��������Ȃ�
				//	//if (_player->GetMoney() < item[_chooseNum].itemCostMoney)
				//	//{
				//		//_chooseFlg = false;
				//	//}
				//	//else
				//	{
				//		// �v���C���[�̂���������
				//		//_player->SetMoney(_player->GetMoney() - item[_chooseNum].itemCostMoney);
				//
				//		// �v���C���[�̃��j���[�ɕ\�������悤�ɂ���
				//		_menu->Setitem(item_pair[_chooseNum].second,item_pair[_chooseNum].first);
				//
				//		// �������Ƃ���ɂ�soldout�ł������Ƃ��āAitem��non�ɂ���
				//		item_pair[_chooseNum].first = _soldOutPNG;
				//		item_pair[_chooseNum].second = ITEM::NON;
				//		_chooseFlg = false;
				//	}
				//}

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
							//player->SetMoney(player->GetMoney() - item->GetCostMoney(_chooseNum));
							player->SetMoney(player->GetMoney() - item->GetCostMoney(static_cast<int>(_itemInfo) - 1));

							// �v���C���[�̃��j���[�ɕ\�������悤�ɂ���
							//menu->Setitem(item->GetPair(_chooseNum).second, item->GetPair(_chooseNum).first);

							// �������Ƃ���ɂ�soldout�ɂ���item��non�ɂ���
							if (!_itemNextPage)
							{
								menu->Setitem(item->GetPair(_chooseNum).second, item->GetPair(_chooseNum).first);
								item->SetSoldOutPair(_chooseNum);
							}
							else
							{
								menu->Setitem(item->GetPair(_chooseNum+8).second, item->GetPair(_chooseNum+8).first);
								item->SetSoldOutPair(_chooseNum+8);
							}
							//item->SetSoldOutPair(_chooseNum);
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
			// ���s��
			PlaySoundMem(_soundSE[2], DX_PLAYTYPE_BACK, true);

			game->_backFlg = true;

			// ��ɐi��
			game->walkCnt++;
			//game->moveFlg = true;
			game->eventState = EVENT_STATE::NON;
			_event = EVENT_STATE::NON;
			_nowEvent++;
			_pushFlg = false;
			_fateNum = -1;
			_soundWalk = true;
		}

		// ����
		if (_fateNum == -1)
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
	if (_pushFlg)
	{
		if (_fateNum == 0)
		{
			// ���b�L�[!�������炦��
			player->SetMoney(player->GetMoney() + 1000);
			_pushFlg = false;
		}
		else
		{
			// �d����
			PlaySoundMem(_soundSE[1], DX_PLAYTYPE_BACK, true);

			// �A�����b�L�[!�т�т�̌Y�ő̗̓}�C�i�X
			// �̗͂�1/4���炢��낤����
			player->SetHP(player->GetHP() - (float)player->GetMaxHP() * (1.0f / 4.0f));
			_pushFlg = false;
			game->shakeFlg = true;
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
			// ���s��
			PlaySoundMem(_soundSE[2], DX_PLAYTYPE_BACK, true);

			game->_backFlg = true;

			// ��ɐi��
			game->walkCnt++;
			//game->moveFlg = true;
			game->eventState = EVENT_STATE::NON;
			_event = EVENT_STATE::NON;
			_nowEvent++;
			_pushFlg = false;
			_fateNum = -1;
			_anounceFlg = false;
			_getFlg = false;
			_soundWalk = true;
			if (menu->GetMeganeFlg())
			{
				menu->SetMeganeFlg(false);
			}
		}

		// �󔠃`�F�b�N
		int a = 0;
		for (int i = 0; i < 2; i++)
		{
			if (chestPos[i].x == GameScene::testx && chestPos[i].y == GameScene::testy)
			{
				a = i;
			}
		}
		// �J����
		if (_fateNum == -1 && chestOpen[a] == 0)
		{
			if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 200 && game->cursorPos.y <= 200 + 75)
			{
				_pushFlg = true;
				//_fateNum = GetRand(2);	// 0 ~ 2
				//_fateNum = 0;
				//_fateNum = game->chestFate;
				//if (chestPos[i].x == GameScene::testx && chestPos[i].y == GameScene::testy)
				//{
					// �����肩�͂��ꂩ�������
					// �J�������Ƃɂ���
					chestOpen[a] = 1;
					_fateNum = chestBingo[a];
				//}
				if (menu->GetMeganeFlg())
				{
					menu->SetMeganeFlg(false);
				}
			}
		}
		//// �J����
		//if (_fateNum == -1 && !chestOpen[0])
		//{
		//	if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 200 && game->cursorPos.y <= 200 + 75)
		//	{
		//		_pushFlg = true;
		//		//_fateNum = GetRand(2);	// 0 ~ 2
		//		//_fateNum = 0;
		//		//_fateNum = game->chestFate;
		//		if (chestPos[0].x == GameScene::testx && chestPos[0].y == GameScene::testy)
		//		{
		//			// �����肩�͂��ꂩ�������
		//			// �J�������Ƃɂ���
		//			chestOpen[0] = true;
		//			_fateNum = chestBingo[0];
		//		}
		//		if (menu->GetMeganeFlg())
		//		{
		//			menu->SetMeganeFlg(false);
		//		}
		//	}
		//}
	}

	// �󔠂��J���邱�Ƃɂ����Ƃ�
	if (_pushFlg)
	{
		if (_fateNum == 1)
		{
			// ���b�L�[!�A�C�e�������炦��
			// �v���C���[�̃��j���[�ɕ\�������悤�ɂ���
			//_menu->Setitem(item_pair[0].second, item_pair[0].first);
			//
			//menu->Setitem(item->GetPair(0).second, item->GetPair(0).first);
			//if (menu->GetCanHaveItem() != 0)
			//{
			//	// �����������^������Ȃ���Ύ��Ă�
			//	menu->Setitem(item->GetPair(0).second, item->GetPair(0).first);
			//}
			_pushFlg = false;
			_getFlg = true;
		}
		else if(_fateNum == 0)
		{
			// �_���[�W��
			PlaySoundMem(_soundSE[3], DX_PLAYTYPE_BACK, true);

			// �A�����b�L�[!�т����蔠������
			// �̗͂�1/4���炢��낤����
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
					// �����������^������Ȃ���Ύ��Ă�
					//menu->Setitem(item->GetPair(static_cast<int>(ITEM::POTION_BIG) - 1).second, item->GetPair(static_cast<int>(ITEM::POTION_BIG)-1).first);
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
			// ���s��
			PlaySoundMem(_soundSE[2], DX_PLAYTYPE_BACK, true);

			game->_backFlg = true;

			// ��ɐi��
			game->walkCnt++;
			//game->moveFlg = true;
			game->eventState = EVENT_STATE::NON;
			_event = EVENT_STATE::NON;
			_nowEvent++;
			_pushFlg = false;
			_fateNum = -1;
			_soundWalk = true;
		}

		// ����
		if (_fateNum == -1)
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

	// ���ނ��Ƃɂ����Ƃ�
	if (_pushFlg)
	{
		if (_fateNum == 0)
		{
			// ���b�L�[!��b�h��͂��オ��
			player->SetDifense(player->GetDifense() + 2);
			_pushFlg = false;
		}
		else
		{
			// �ŉ�
			PlaySoundMem(_soundSE[4], DX_PLAYTYPE_BACK, true);

			// �A�����b�L�[!�łɂ�����
			player->SetCondition(CONDITION::POISON);
			_pushFlg = false;
			game->shakeFlg = true;
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
			// ���s��
			PlaySoundMem(_soundSE[2], DX_PLAYTYPE_BACK, true);

			game->_backFlg = true;

			// ��ɐi��
			game->walkCnt++;
			//game->moveFlg = true;
			game->eventState = EVENT_STATE::NON;
			_event = EVENT_STATE::NON;
			_nowEvent++;
			_fateNum = -1;
			_soundWalk = true;
		}

		// ���ׂ�
		if (!trapFlg)
		{
			if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 200 && game->cursorPos.y <= 200 + 75)
			{
				// �N���b�N��
				PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
				trapFlg = true;
			}
		}
	}

	// ���ׂ邱�Ƃɂ����Ƃ�
	if (trapFlg)
	{
		// �����g���b�v�̔���
		player->SetHP(player->GetHP() - player->GetMaxHP());

		// �Ƃ肠���������œ���G�C�x���g���I��������
		if (eventmonsencountFlg && !eventmonsEndFlg)
		{
			eventmonsencountFlg = false;
			eventmonsEndFlg = true;
		}
	}
}

void Event::eventMons(GameScene* game, Monster* monster, Cards* cards)
{
	eventmonsencountFlg = true;
	if (game->mouse & MOUSE_INPUT_LEFT) {			 //�}�E�X�̍��{�^����������Ă�����
		if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 345 && game->cursorPos.y <= 345 + 75)
		{
			// �N���b�N��
			PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
			// ���s��
			PlaySoundMem(_soundSE[2], DX_PLAYTYPE_BACK, true);

			game->_backFlg = true;

			// ��ɐi��
			game->walkCnt++;
			//game->moveFlg = true;
			game->eventState = EVENT_STATE::NON;
			_event = EVENT_STATE::NON;
			_nowEvent++;
			_fateNum = -1;
			_soundWalk = true;
		}

		// �키
		if (!eventmonsFlg)
		{
			if (game->cursorPos.x >= 600 && game->cursorPos.x <= 600 + 150 && game->cursorPos.y >= 200 && game->cursorPos.y <= 200 + 75)
			{
				// �N���b�N��
				PlaySoundMem(_soundSE[0], DX_PLAYTYPE_BACK, true);
				eventmonsFlg = true;
				auto ene = 6;
				monster->SetEnemyNum(ene, 0);
				cards->SetTurn(3);
			}
		}
		//if(eventmonsFlg)
		//{
		//	auto ene = 6;
		//	monster->SetEnemyNum(ene, 0);
		//	cards->SetTurn(3);
		//}
	}
}