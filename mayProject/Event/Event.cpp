#include <Dxlib.h>
#include "Event.h"
#include "../Scene/GameScene.h"
#include "../Player.h"
#include "../Card/Cards.h"
#include "../Scene/SelectScene.h"
#include "../Common/MouseCtl.h"

#define PI 3.141592653589793f

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
	_healinnFlg = false;
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
	_forcedButtleFlg = false;

	_chestItemMap.try_emplace(0, ITEM::POTION_BIG);
	_chestItemMap.try_emplace(1, ITEM::DETOX);
	_chestItemMap.try_emplace(2, ITEM::POW_UP);
	_chestItemMap.try_emplace(3, ITEM::HEART);

	// �󔠐ݒ�
	int chestHandle;
	// �{�^���ƈ��ݕ��̍��W�ݒ�
	int posHandle;

	if (SelectScene::modeSelect == MODE::NORMAL)
	{
		chestHandle = FileRead_open("csv/chest1.csv");
		posHandle   = FileRead_open("csv/buttonDrink1.csv");
		if (chestHandle == NULL || posHandle == NULL)
		{
			return; // �G���[
		}
	}
	else if (SelectScene::modeSelect == MODE::HARD)
	{
		chestHandle = FileRead_open("csv/chest2.csv");
		posHandle   = FileRead_open("csv/buttonDrink2.csv");
		if (chestHandle == NULL || posHandle == NULL)
		{
			return; // �G���[
		}
	}
	else
	{
		return;		// �G���[
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
	_soundSE[1] = LoadSoundMem("sound/se/blitz.mp3");
	_soundSE[2] = LoadSoundMem("sound/se/damage.mp3");
	_soundSE[3] = LoadSoundMem("sound/se/poison.mp3");

	innSt	    = std::make_unique<INNSt>();
	merchantSt  = std::make_unique<MerchantSt>();
	buttonSt    = std::make_unique<ButtonSt>();
	chestSt     = std::make_unique<ChestSt>();
	drinkSt     = std::make_unique<DrinkSt>();
	deathTrapSt = std::make_unique<DeathTrapSt>();
	eventMonsSt = std::make_unique<EventMonsSt>();

	exr = 0.0f;
}

void Event::pngInit(void)
{
	_eventImages.try_emplace("inn", LoadGraph("image/event/inn.png"));						// �h��
	_eventImages.try_emplace("merchant", LoadGraph("image/event/merchant.png"));			// ���l
	_eventImages.try_emplace("bottle", LoadGraph("image/event/bottle.png"));				// ���ݕ�
	_eventImages.try_emplace("chestInItem", LoadGraph("image/event/tresure_1.png"));		// ��(�A�C�e���������Ă���)
	_eventImages.try_emplace("chestEmpty", LoadGraph("image/event/chest_empty.png"));		// ���
	_eventImages.try_emplace("deathTrap", LoadGraph("image/event/instant_death_trap.png"));	// �����g���b�v
	_eventImages.try_emplace("mons", LoadGraph("image/monster/event_monster.png"));			// ����G

	_drawHandle.try_emplace("message", LoadGraph("image/message.png"));
	_drawHandle.try_emplace("frame", LoadGraph("image/frame.png"));
	_drawHandle.try_emplace("itembox", LoadGraph("image/itembox.png"));
	_drawHandle.try_emplace("itemChoice", LoadGraph("image/itemChoice.png"));
	_drawHandle.try_emplace("arrow", LoadGraph("image/arrow.png"));

	// ��
	std::string trasure_0 = "image/event/trasure_0.png";
	LoadDivGraph(trasure_0.c_str(), 2, 2, 1, 390 / 2, 431, _chestPNG);
	// �����摜�̕����ǂݍ���
	std::string sentakusi = "image/choice/choice.png";
	LoadDivGraph(sentakusi.c_str(),12, 12, 1, 150, 75, _choicesPNG);
}

void Event::UpDate(GameScene* game, const std::shared_ptr<Player>& player, const std::shared_ptr<Menu>& menu, const std::shared_ptr<Item>& item, const std::shared_ptr<Monster>& monster, const std::shared_ptr<Cards>& cards, const std::shared_ptr<MouseCtl>& mouse)
{
	// �����퓬�̐ݒ�
	auto forcedButtleLambda = [&]() {
		if (_eventMonsFlg && !_onceFlg)
		{
			monster->SetEnemyNum(6, 0);
			cards->SetTurn(3);
			_onceFlg = true;
			_forcedButtleFlg = true;
		}
	};

	// _event�ƈ�v���Ă���C�x���g�𔭐�������
	switch (_event)
	{
	case EVENT_STATE::ENEMY:
		Enemy(game, player, monster);
		break;
	case EVENT_STATE::INN:
		forcedButtleLambda();
		innSt->Update(*this, *game, *player, *mouse);
		break;
	case EVENT_STATE::MERCHANT:
		forcedButtleLambda();
		merchantSt->Update(*this, *game, *player, *mouse, *item, *menu);
		break;
	case EVENT_STATE::BUTTON:
		forcedButtleLambda();
		buttonSt->Update(*this, *game, *player, *mouse);
		break;
	case EVENT_STATE::CHEST:
		forcedButtleLambda();
		chestSt->Update(*this, *game, *player, *menu, *item, *mouse);
		break;
	case EVENT_STATE::DRINK:
		forcedButtleLambda();
		drinkSt->Update(*this, *game, *player, *mouse);
		break;
	case EVENT_STATE::TRAP:
		deathTrapSt->Update(*this, *game, *player, *mouse);
		break;
	case EVENT_STATE::EVE_MONS:
		eventMonsSt->Update(*this, *game, *monster, *cards, *mouse);
		break;
	default:
		break;
	}
}

void Event::Draw(GameScene* game, const std::shared_ptr<Player>& player, const std::shared_ptr<Menu>& menu, const std::shared_ptr<Item>& item)
{
	// _event�ƈ�v���Ă���C�x���g��`�悷��
	switch (_event)
	{
	case EVENT_STATE::INN:
		if (!_eventMonsFlg)
		{
			innSt->Draw(*this, *player);
		}
		break;
	case EVENT_STATE::MERCHANT:
		if (!_eventMonsFlg)
		{
			merchantSt->Draw(*this, *player, *item, *menu);
		}
		break;
	case EVENT_STATE::BUTTON:
		if (!_eventMonsFlg)
		{
			buttonSt->Draw(*this, *game);
		}
		break;
	case EVENT_STATE::CHEST:
		if (!_eventMonsFlg)
		{
			chestSt->Draw(*this, *menu);
		}
		break;
	case EVENT_STATE::DRINK:
		if (!_eventMonsFlg)
		{
			drinkSt->Draw(*this, *game);
		}
		break;
	case EVENT_STATE::TRAP:
		deathTrapSt->Draw(*this);
		break;
	default:
		break;
	}

	// �C�x���g�G�Ƒ�����
	if (_event == EVENT_STATE::EVE_MONS || _event != EVENT_STATE::NON && _eventMonsFlg)
	{
		eventMonsSt->Draw(*this);
	}

	// �C�x���g��NON�ł�eventmonFlg�������Ă�����`��(��ʂɒ���t��)
	if (_event == EVENT_STATE::NON && _eventMonsEncountFlg)
	{
		DrawGraph(100, 75, _eventImages["mons"], true);
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
	_forcedButtleFlg = flag;
}

bool Event::GetCautionFlg(void)const
{
	return _forcedButtleFlg;
}

void Event::Enemy(GameScene* game, const std::shared_ptr<Player>& player, const std::shared_ptr<Monster>& monster)
{
	game->eventState = EVENT_STATE::NON;
	_event = EVENT_STATE::NON;

	player->SetNextLevel(player->GetNextLevel() - monster->GetEXP());

	// �������𑝂₷
	player->SetMoney(player->GetMoney() + monster->GetMoney());
}