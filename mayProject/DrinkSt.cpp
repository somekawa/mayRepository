#include <DxLib.h>
#include "Event.h"
#include "DrinkSt.h"
#include "GameScene.h"
#include "MouseCtl.h"
#include "Player.h"

#define PI 3.141592653589793f

DrinkSt::DrinkSt()
{
	exr = 0.0f;
}

DrinkSt::~DrinkSt()
{
}

void DrinkSt::Update(Event& eve, GameScene& game, Player& player, MouseCtl& mouse)
{
	exr++;
	if (mouse.GetClickTrg())
	{
		if (mouse.GetPos().x >= 600 && mouse.GetPos().x <= 600 + 150 && mouse.GetPos().y >= 345 && mouse.GetPos().y <= 345 + 75)
		{
			exr = 0.0f;
			// �N���b�N��
			PlaySoundMem(eve._soundSE[0], DX_PLAYTYPE_BACK, true);
			// ����
			game.backFlg = true;
			game.eventState = EVENT_STATE::NON;
			eve._event = EVENT_STATE::NON;
			eve._fateNum = -1;

			eve._drinkEventFlg = false;
			if (eve._pushFlg)
			{
				eve._drinking[eve._drinkNum] = true;
				eve._pushFlg = false;
			}
		}

		for (int i = 0; i < 4; i++)
		{
			if (eve._buttonDrink[i].x == game.plPosX && eve._buttonDrink[i].y == game.plPosY)
			{
				eve._drinkNum = i;
			}
		}

		// ����
		if (eve._fateNum == -1 && !eve._drinking[eve._drinkNum])
		{
			if (mouse.GetPos().x >= 600 && mouse.GetPos().x <= 600 + 150 && mouse.GetPos().y >= 200 && mouse.GetPos().y <= 200 + 75)
			{
				// �N���b�N��
				PlaySoundMem(eve._soundSE[0], DX_PLAYTYPE_BACK, true);
				eve._pushFlg = true;
				eve._fateNum = GetRand(2);	// 0 ~ 2
			}
		}
	}

	// ���ނ��Ƃɂ����Ƃ�
	if (eve._pushFlg && !eve._drinkEventFlg)
	{
		if (eve._fateNum == 0)
		{
			// ���b�L�[!��b�h��͂��オ��
			player.SetDifense(player.GetDifense() + 2);
			//_pushFlg = false;
			eve._drinkEventFlg = true;
		}
		else
		{
			// �ŉ�
			PlaySoundMem(eve._soundSE[3], DX_PLAYTYPE_BACK, true);
			// �łɂ�����
			player.SetCondition(CONDITION::POISON);
			//_pushFlg = false;
			game.shakeFlg = true;
			eve._drinkEventFlg = true;
		}
	}
}

void DrinkSt::Draw(Event& eve,GameScene& game)
{
	for (int i = 0; i < 4; i++)
	{
		if (eve._buttonDrink[i].x == game.plPosX && eve._buttonDrink[i].y == game.plPosY)
		{
			eve._drinkNum = i;
		}
	}

	if (!eve._drinking[eve._drinkNum])
	{
		// ���b�Z�[�W�{�b�N�X
		DrawGraph(420, 50, eve._drawHandle["message"], true);
		// �r�摜
		DrawGraph(350, 250, eve.eventImages["bin"], true);
		if (eve._fateNum == -1)
		{
			// ����
			//DrawGraph(600, 200, eve._sentakusiPNG[1], true);
			DrawRotaGraph(600 + 150 / 2, 200 + 75 / 2, 0.9f + sinf(PI * 2.0f / 200.0f * exr) * 0.1, 0.0f, eve._sentakusiPNG[1], true);
			DrawFormatString(450, 70, 0x000000, "[Drink Me]\n�Ƃ����ꂽ�r������...");
		}

		if (eve._fateNum == 0)
		{
			DrawFormatString(450, 70, 0x000000, "�̂����ɂȂ���!\n�h��͂��オ����");
		}

		if (eve._fateNum > 0)
		{
			DrawFormatString(450, 70, 0x000000, "�łɂ������Ă��܂���...");
		}
	}
	// ����
	//DrawGraph(600, 345, eve._sentakusiPNG[10], true);
	DrawRotaGraph(600 + 150 / 2, 345 + 75 / 2, 0.9f + sinf(PI * 2.0f / 200.0f * exr) * 0.1, 0.0f, eve._sentakusiPNG[10], true);
}
