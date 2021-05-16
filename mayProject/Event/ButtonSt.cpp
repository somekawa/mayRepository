#include <DxLib.h>
#include "Event.h"
#include "ButtonSt.h"
#include "../Scene/GameScene.h"
#include "../Common/MouseCtl.h"
#include "../Player.h"

#define PI 3.141592653589793f

ButtonSt::ButtonSt()
{
	exr = 0.0f;
	getMoney = 1000;
	choicesPngSize = { 150,75 };
	offsetPos = { 600,345 };
}

ButtonSt::~ButtonSt()
{
}

void ButtonSt::Update(Event& eve, GameScene& game, Player& player, MouseCtl& mouse)
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
			eve._fateNum = -1;
			eve._buttonEventFlg = false;
			if (eve._pushFlg)
			{
				eve._buttonPush[eve._buttonNum] = true;
				eve._pushFlg = false;
			}
		}

		// ����
		for (int i = 0; i < 4; i++)
		{
			if (eve._buttonDrink[i].x == game.plPosX && eve._buttonDrink[i].y == game.plPosY)
			{
				eve._buttonNum = i;
			}
		}

		if (eve._fateNum == -1 && !(eve._buttonPush[eve._buttonNum]))
		{
			if (mouse.GetPos().x >= offsetPos.x && mouse.GetPos().x <= offsetPos.x + choicesPngSize.x &&
				mouse.GetPos().y >= 200 && mouse.GetPos().y <= 200 + choicesPngSize.y)
			{
				// �N���b�N��
				PlaySoundMem(eve._soundSE[0], DX_PLAYTYPE_BACK, true);
				eve._pushFlg = true;
				eve._fateNum = GetRand(2);	// 0 ~ 2
			}
		}
	}

	// �{�^�����������Ƃɂ����Ƃ�
	if (eve._pushFlg && !eve._buttonEventFlg)
	{
		if (eve._fateNum == 0)
		{
			// ��������ɓ���
			player.SetMoney(player.GetMoney() + getMoney);
			eve._buttonEventFlg = true;
		}
		else
		{
			// �d����
			PlaySoundMem(eve._soundSE[1], DX_PLAYTYPE_BACK, true);
			// �̗͂�1/4�����
			player.SetHP(player.GetHP() - static_cast<int>(static_cast<float>(player.GetMaxHP()) * (1.0f / 4.0f)));
			game.shakeFlg = true;
			eve._buttonEventFlg = true;
		}
	}
}

void ButtonSt::Draw(Event& eve, GameScene& game)
{
	for (int i = 0; i < 4; i++)
	{
		if (eve._buttonDrink[i].x == game.plPosX && eve._buttonDrink[i].y == game.plPosY)
		{
			eve._buttonNum = i;
		}
	}

	if (!eve._buttonPush[eve._buttonNum])
	{
		// ���b�Z�[�W�{�b�N�X
		DrawGraph(420, 50, eve._drawHandle["message"], true);

		const VECTOR2 tmpVec(450, 70);
		const unsigned int color(0x000000);
		if (eve._fateNum == -1)
		{
			// ����
			DrawRotaGraph(offsetPos.x + choicesPngSize.x / 2, 200 + choicesPngSize.y / 2, static_cast<double>(0.9f + (sinf(PI * 2.0f / 200.0f * exr) * 0.1f)), 0.0f, eve._choicesPNG[4], true);
			DrawFormatString(tmpVec.x, tmpVec.y, color, "�ǂɃ{�^�������Ă���...\n");
		}
		else if (eve._fateNum == 0)
		{
			DrawFormatString(tmpVec.x, tmpVec.y, color, "�Ȃ��1000�~���łĂ���!\n");
		}
		else if (eve._fateNum > 0)
		{
			DrawFormatString(tmpVec.x, tmpVec.y, color, "�̒��ɓd�������ꂽ!!");
		}
		else
		{
			// �����������s��Ȃ�
		}
	}

	// ����
	DrawRotaGraph(offsetPos.x + choicesPngSize.x / 2, offsetPos.y + choicesPngSize.y / 2, static_cast<double>(0.9f + (sinf(PI * 2.0f / 200.0f * exr) * 0.1f)), 0.0f, eve._choicesPNG[10], true);
}