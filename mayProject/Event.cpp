#include "Dxlib.h"
#include "Event.h"
#include "GameScene.h"
#include "Player.h"
#include "Cards.h"
#include "SelectScene.h"
#include "MouseCtl.h"

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

	_chestItemMap.try_emplace(0, ITEM::POTION_BIG);
	_chestItemMap.try_emplace(1, ITEM::DETOX);
	_chestItemMap.try_emplace(2, ITEM::KYOUKA_POW);
	_chestItemMap.try_emplace(3, ITEM::HEART);

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
	else
	{
		// �����ɂ�����G���[
		return;
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

	yadoSt = std::make_unique<YadoSt>();
	syouninSt = std::make_unique<SyouninSt>();
	buttonSt = std::make_unique<ButtonSt>();
	chestSt = std::make_unique<ChestSt>();
	drinkSt = std::unique_ptr<DrinkSt>();
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

	_drawHandle.try_emplace("message", LoadGraph("image/message.png"));
	_drawHandle.try_emplace("frame", LoadGraph("image/frame.png"));
	_drawHandle.try_emplace("itembox", LoadGraph("image/itembox.png"));
	_drawHandle.try_emplace("itemChoice", LoadGraph("image/itemChoice.png"));
	_drawHandle.try_emplace("yajirusi", LoadGraph("image/yajirusi.png"));

	//// ���b�Z�[�W
	//std::string message = "image/message.png";
	//_messagePNG = LoadGraph(message.c_str());
	//// ���l�̎������̌��g
	//std::string frame = "image/frame.png";
	//_syouninWakuPNG = LoadGraph(frame.c_str());
	//// �A�C�e���{�b�N�X�w�i
	//std::string itembox = "image/itembox.png";
	//_itemBoxPNG = LoadGraph(itembox.c_str());
	//// �I�𒆂̃A�C�e��
	//std::string itemChoice = "image/itemChoice.png";
	//_itemChoicePNG = LoadGraph(itemChoice.c_str());
	// ��
	std::string trasure_0 = "image/trasure_0.png";
	LoadDivGraph(trasure_0.c_str(), 2, 2, 1, 390 / 2, 431, _chestPNG);
	// �����摜�̕����ǂݍ���
	std::string sentakusi = "image/sentakusi/sentakusi.png";
	LoadDivGraph(sentakusi.c_str(),12, 12, 1, 150, 75, _sentakusiPNG);
	//// ���i�y�[�W���ړ����邽�߂̖��̉摜
	//std::string yajirusi = "image/yajirusi.png";
	//_yajirusiPNG = LoadGraph(yajirusi.c_str());
}

void Event::UpDate(GameScene* game, Player* player, Menu* menu, Item* item, Monster* monster,Cards* cards, MouseCtl* mouse)
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
			yadoSt->Update(*this, *game, *player, *mouse);

			//Yado(game, player,mouse);
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
			syouninSt->Update(*this, *game, *player, *mouse, *item, *menu);
			//Syounin(game, player, menu, item,mouse);
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
			buttonSt->Update(*this, *game, *player, *mouse);
			//Button(game, player,mouse);
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
			chestSt->Update(*this, *game, *player, *menu, *item, *mouse);
			//Chest(game, player, menu, item,mouse);
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
			drinkSt->Update(*this, *game, *player, *mouse);
			//Drink(game, player,mouse);
		}
	}

	if (_event == EVENT_STATE::TRAP)
	{
		Trap(game,player,mouse);
	}

	if (_event == EVENT_STATE::ENEMY)
	{
		Enemy(game, player, monster);
	}

	if (_event == EVENT_STATE::EVE_MONS)
	{
		eventMons(game, monster,cards,mouse);
	}
}

void Event::Draw(GameScene* game, Player* player, Menu* menu, Item* item)
{
	// �h��
	if (_event == EVENT_STATE::YADO && !_eventMonsFlg)
	{
		yadoSt->Draw(*this, *player);
	}

	// ���l
	if (_event == EVENT_STATE::SYOUNIN && !_eventMonsFlg)
	{
		syouninSt->Draw(*this, *player, *item, *menu);
	}

	// �{�^���o����
	if (_event == EVENT_STATE::BUTTON && !_eventMonsFlg)
	{
		buttonSt->Draw(*this, *game);
	}

	// �󔠏o����
	if (_event == EVENT_STATE::CHEST && !_eventMonsFlg)
	{
		chestSt->Draw(*this, *menu);
	}

	// �r�o����
	if (_event == EVENT_STATE::DRINK && !_eventMonsFlg)
	{
		drinkSt->Draw(*this, *game);
	}

	// �����g���b�v�o����
	if (_event == EVENT_STATE::TRAP)
	{
		// ���b�Z�[�W�{�b�N�X
		DrawGraph(420, 50, _drawHandle["message"], true);
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
			DrawGraph(420, 50, _drawHandle["message"], true);
			// ����
			DrawGraph(600, 345, _sentakusiPNG[10], true);
			// ���ׂ�
			DrawGraph(600, 200, _sentakusiPNG[11], true);
			DrawFormatString(450, 70, 0x000000, "�G�������ӂ����ł���\n�����ǂ����@�͂Ȃ����낤��..");
		}
	}

	// �C�x���g��NON�ł�eventmonFlg�����Ă���`�悵�Ă݂�(��ʂɒ���t��)
	if (_event == EVENT_STATE::NON && _eventMonsEncountFlg)
	{
		DrawGraph(100, 75, eventImages["mons"], true);
		// ���b�Z�[�W�{�b�N�X
		DrawGraph(420, 50, _drawHandle["message"], true);
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

void Event::Trap(GameScene* game, Player* player, MouseCtl* mouse)
{
	if (mouse->GetClickTrg())
	{			
		if (mouse->GetPos().x >= 600 && mouse->GetPos().x <= 600 + 150 && mouse->GetPos().y >= 345 && mouse->GetPos().y <= 345 + 75)
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
			if (mouse->GetPos().x >= 600 && mouse->GetPos().x <= 600 + 150 && mouse->GetPos().y >= 200 && mouse->GetPos().y <= 200 + 75)
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

void Event::eventMons(GameScene* game, Monster* monster, Cards* cards, MouseCtl* mouse)
{
	_eventMonsEncountFlg = true;
	if (mouse->GetClickTrg()) 
	{			 
		if (mouse->GetPos().x >= 600 && mouse->GetPos().x <= 600 + 150 && mouse->GetPos().y >= 345 && mouse->GetPos().y <= 345 + 75)
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
			if (mouse->GetPos().x >= 600 && mouse->GetPos().x <= 600 + 150 && mouse->GetPos().y >= 200 && mouse->GetPos().y <= 200 + 75)
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