#include <DxLib.h>
#include "Event.h"
#include "ChestSt.h"
#include "../Scene/GameScene.h"
#include "../Common/MouseCtl.h"
#include "../Player.h"
#include "../Menu.h"
#include "../Item/Item.h"

#define PI 3.141592653589793f

ChestSt::ChestSt()
{
	exr = 0.0f;
	choicesPngSize = { 150,75 };
	offsetPos = { 600,345 };
}

ChestSt::~ChestSt()
{
}

void ChestSt::Update(Event& eve,GameScene& game, Player& player, Menu& menu, Item& item, MouseCtl& mouse)
{
	exr++;
	if (mouse.GetClickTrg())
	{
		if (mouse.GetPos().x >= offsetPos.x && mouse.GetPos().x <= offsetPos.x + choicesPngSize.x &&
			mouse.GetPos().y >= offsetPos.y && mouse.GetPos().y <= offsetPos.y + choicesPngSize.y)
		{
			exr = 0.0f;
			// �N���b�N��
			PlaySoundMem(eve._soundSE[0], DX_PLAYTYPE_BACK, true);
			// ����
			game.backFlg = true;
			game.eventState = EVENT_STATE::NON;
			eve._event = EVENT_STATE::NON;
			eve._pushFlg = false;
			eve._fateNum = -1;
			eve._anounceFlg = false;
			eve._getFlg = false;
			if (menu.GetAppraisalFlg())
			{
				menu.SetAppraisalFlg(false);
			}
		}

		// �󔠃`�F�b�N
		int boxNum = 0;
		for (int i = 0; i < 4; i++)
		{
			if (eve._chestPos[i].x == GameScene::plPosX && eve._chestPos[i].y == GameScene::plPosY)
			{
				boxNum = i;
			}
		}

		// �J����
		if (eve._fateNum == -1 && eve._chestOpen[boxNum] == 0)
		{
			if (mouse.GetPos().x >= offsetPos.x && mouse.GetPos().x <= offsetPos.x + choicesPngSize.x &&
				mouse.GetPos().y >= 200 && mouse.GetPos().y <= 200 + choicesPngSize.y)
			{
				eve._pushFlg = true;
				eve._chestOpen[boxNum] = 1;
				eve._fateNum = eve._chestBingo[boxNum];
				if (menu.GetAppraisalFlg())
				{
					menu.SetAppraisalFlg(false);
				}
			}
		}
	}

	// �󔠂��J���邱�Ƃɂ����Ƃ�
	if (eve._pushFlg)
	{
		if (eve._fateNum == 1)
		{
			eve._pushFlg = false;
			eve._getFlg  = true;
		}
		else if (eve._fateNum == 0)
		{
			// �_���[�W��
			PlaySoundMem(eve._soundSE[2], DX_PLAYTYPE_BACK, true);
			// �̗͂�1/4���
			player.SetHP(player.GetHP() - static_cast<int>(static_cast<float>(player.GetMaxHP()) * (1.0f / 4.0f)));
			eve._pushFlg  = false;
			game.shakeFlg = true;
		}
		else
		{
			// �����������s��Ȃ�
		}
	}

	if (eve._getFlg)
	{
		// �����������^�����ǂ������ׂ�
		if (menu.GetCanHaveItem() != 0)
		{
			eve._anounceFlg = false;
		}
		else
		{
			eve._anounceFlg = true;
		}

		if (!mouse.GetClickTrg())
		{
			return;		// �N���b�N��ԂŖ������return����
		}

		if (mouse.GetPos().x >= offsetPos.x && mouse.GetPos().x <= offsetPos.x + choicesPngSize.x && mouse.GetPos().y >= 200 && mouse.GetPos().y <= 200 + choicesPngSize.y)
		{
			if (menu.GetCanHaveItem() != 0)
			{
				// �N���b�N��
				PlaySoundMem(eve._soundSE[0], DX_PLAYTYPE_BACK, true);
				const int randNum = GetRand(4);
				// �����������^������Ȃ���Ύ��Ă�
				menu.SetItem(item.GetPair(static_cast<int>(eve._chestItemMap[randNum]) - 1).second, item.GetPair(static_cast<int>(eve._chestItemMap[randNum]) - 1).first);
				eve._getFlg = false;
				eve._anounceFlg = false;
			}
		}
	}
}

void ChestSt::Draw(Event& eve, Menu& menu)
{
	// ���b�Z�[�W�{�b�N�X
	DrawGraph(420, 50, eve._drawHandle["message"], true);

	// �󔠃`�F�b�N
	int tmpNum = 0;
	for (int i = 0; i < 4; i++)
	{
		if (eve._chestPos[i].x == GameScene::plPosX && eve._chestPos[i].y == GameScene::plPosY)
		{
			tmpNum = i;
		}
	}

	const VECTOR2 tmpVec(450, 70);
	const unsigned int color(0x000000);

	if (eve._chestOpen[tmpNum] == 0)
	{
		// �i��(�󔠖���)
		DrawRotaGraph(offsetPos.x + choicesPngSize.x / 2, offsetPos.y + choicesPngSize.y / 2, static_cast<double>(0.9f + (sinf(PI * 2.0f / 200.0f * exr) * 0.1f)), 0.0f, eve._choicesPNG[10], true);

		if (eve._fateNum == -1)
		{
			// �J����
			DrawRotaGraph(offsetPos.x + choicesPngSize.x / 2, 200 + choicesPngSize.y / 2, static_cast<double>(0.9f + (sinf(PI * 2.0f / 200.0f * exr) * 0.1f)), 0.0f, eve._choicesPNG[3], true);
			DrawGraph(350, choicesPngSize.x, eve._chestPNG[0], true);
			DrawFormatString(tmpVec.x, tmpVec.y, color, "�󔠂��u���Ă���");
		}

		// �Ӓ�A�C�e�����g�����Ƃ��̕`��
		if (menu.GetAppraisalFlg())
		{
			if (eve._chestBingo[tmpNum] == 1)
			{
				DrawFormatString(tmpVec.x, tmpVec.y, color, "\n\n���ɂ��������Ƃ���͂Ȃ�");
			}
			if (eve._chestBingo[tmpNum] == 0)
			{
				DrawFormatString(tmpVec.x, tmpVec.y, color, "\n\n�S�[�X�g�����߂��Ă���");
			}
		}
	}
	else
	{
		if (eve._getFlg)
		{
			// ���
			DrawRotaGraph(offsetPos.x + choicesPngSize.x / 2, 200 + choicesPngSize.y / 2, static_cast<double>(0.9f + (sinf(PI * 2.0f / 200.0f * exr) * 0.1f)), 0.0f, eve._choicesPNG[8], true);
		}

		if (eve._anounceFlg)
		{
			// ���������^����������ĂȂ�
			DrawFormatString(offsetPos.x, 180, 0xffffff, "�����i�������ς���");
		}

		const VECTOR2 itemVec(350, 150);
		if (eve._fateNum == 1)
		{
			DrawGraph(itemVec.x, itemVec.y, eve._eventImages["chestInItem"], true);
			DrawFormatString(tmpVec.x, tmpVec.y, color, "�A�C�e���������Ă���!");
		}
		else if (eve._fateNum == 0)
		{
			DrawGraph(itemVec.x, itemVec.y, eve._chestPNG[1], true);
			DrawFormatString(tmpVec.x, tmpVec.y, color, "�S�[�X�g������U�����Ă���!");
		}
		else
		{
			// �󔠂����łɊJ���Ă���
			DrawGraph(itemVec.x, itemVec.y, eve._eventImages["chestEmpty"], true);
			DrawFormatString(tmpVec.x, tmpVec.y, color, "�󔠂͊J���Ă���");
		}

		// ����(�󔠖���)
		DrawRotaGraph(offsetPos.x + choicesPngSize.x / 2, offsetPos.y + choicesPngSize.y / 2, 0.9f + sinf(PI * 2.0f / 200.0f * exr) * 0.1, 0.0f, eve._choicesPNG[10], true);
	}
}