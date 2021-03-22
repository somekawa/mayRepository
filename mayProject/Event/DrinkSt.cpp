#include <DxLib.h>
#include "Event.h"
#include "DrinkSt.h"
#include "../Scene/GameScene.h"
#include "../Common/MouseCtl.h"
#include "../Player.h"

#define PI 3.141592653589793f

DrinkSt::DrinkSt()
{
	exr = 0.0f;
	choicesPngSize = { 150,75 };
	offsetPos = { 600,345 };
}

DrinkSt::~DrinkSt()
{
}

void DrinkSt::Update(Event& eve, GameScene& game, Player& player, MouseCtl& mouse)
{
	exr++;
	if (mouse.GetClickTrg())
	{
		if (mouse.GetPos().x >= offsetPos.x && mouse.GetPos().x <= offsetPos.x + choicesPngSize.x && mouse.GetPos().y >= offsetPos.y && mouse.GetPos().y <= offsetPos.y + choicesPngSize.y)
		{
			exr = 0.0f;
			// クリック音
			PlaySoundMem(eve._soundSE[0], DX_PLAYTYPE_BACK, true);
			// 去る
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

		// 飲む
		if (eve._fateNum == -1 && !eve._drinking[eve._drinkNum])
		{
			if (mouse.GetPos().x >= offsetPos.x && mouse.GetPos().x <= offsetPos.x + choicesPngSize.x && mouse.GetPos().y >= 200 && mouse.GetPos().y <= 200 + choicesPngSize.y)
			{
				// クリック音
				PlaySoundMem(eve._soundSE[0], DX_PLAYTYPE_BACK, true);
				eve._pushFlg = true;
				eve._fateNum = GetRand(2);	// 0 ~ 2
			}
		}
	}

	// 飲むことにしたとき
	if (eve._pushFlg && !eve._drinkEventFlg)
	{
		if (eve._fateNum == 0)
		{
			// 基礎防御力が上がる
			player.SetDifense(player.GetDifense() + 2);
			eve._drinkEventFlg = true;
		}
		else
		{
			// 毒音
			PlaySoundMem(eve._soundSE[3], DX_PLAYTYPE_BACK, true);
			// 毒にかかる
			player.SetCondition(CONDITION::POISON);
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
		// メッセージボックス
		DrawGraph(420, 50, eve._drawHandle["message"], true);
		// 瓶画像
		DrawGraph(350, 250, eve._eventImages["bottle"], true);
		if (eve._fateNum == -1)
		{
			// 飲む
			DrawRotaGraph(offsetPos.x + choicesPngSize.x / 2, 200 + choicesPngSize.y / 2, 0.9f + sinf(PI * 2.0f / 200.0f * exr) * 0.1, 0.0f, eve._choicesPNG[1], true);
			DrawFormatString(450, 70, 0x000000, "[Drink Me]\nとかかれた瓶がある...");
		}

		if (eve._fateNum == 0)
		{
			DrawFormatString(450, 70, 0x000000, "体が頑丈になった!\n防御力が上がった");
		}

		if (eve._fateNum > 0)
		{
			DrawFormatString(450, 70, 0x000000, "毒にかかってしまった...");
		}
	}
	// 去る
	DrawRotaGraph(offsetPos.x + choicesPngSize.x / 2, offsetPos.y + choicesPngSize.y / 2, 0.9f + sinf(PI * 2.0f / 200.0f * exr) * 0.1, 0.0f, eve._choicesPNG[10], true);
}