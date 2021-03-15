#include <DxLib.h>
#include "Event.h"
#include "ButtonSt.h"
#include "GameScene.h"
#include "MouseCtl.h"
#include "Player.h"

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
			eve._buttonEventFlg = false;
			if (eve._pushFlg)
			{
				eve._buttonPush[eve._buttonNum] = true;
				eve._pushFlg = false;
			}
		}

		// 押す
		for (int i = 0; i < 4; i++)
		{
			if (eve._buttonDrink[i].x == game.plPosX && eve._buttonDrink[i].y == game.plPosY)
			{
				eve._buttonNum = i;
			}
		}

		if (eve._fateNum == -1 && !eve._buttonPush[eve._buttonNum])
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

	// ボタンを押すことにしたとき
	if (eve._pushFlg && !eve._buttonEventFlg)
	{
		if (eve._fateNum == 0)
		{
			// お金が手に入る
			player.SetMoney(player.GetMoney() + getMoney);
			eve._buttonEventFlg = true;
		}
		else
		{
			// 電流音
			PlaySoundMem(eve._soundSE[1], DX_PLAYTYPE_BACK, true);
			// 体力の1/4を削る
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
		// メッセージボックス
		DrawGraph(420, 50, eve._drawHandle["message"], true);

		if (eve._fateNum == -1)
		{
			// 押す
			DrawRotaGraph(offsetPos.x + choicesPngSize.x / 2, 200 + choicesPngSize.y / 2, 0.9f + sinf(PI * 2.0f / 200.0f * exr) * 0.1, 0.0f, eve._choicesPNG[4], true);
			DrawFormatString(450, 70, 0x000000, "壁にボタンがついている...\n");
		}

		if (eve._fateNum == 0)
		{
			DrawFormatString(450, 70, 0x000000, "なんと1000円がでてきた!\n");
		}

		if (eve._fateNum > 0)
		{
			DrawFormatString(450, 70, 0x000000, "体中に電流が流れた!!");
		}
	}

	// 去る
	DrawRotaGraph(offsetPos.x + choicesPngSize.x / 2, offsetPos.y + choicesPngSize.y / 2, 0.9f + sinf(PI * 2.0f / 200.0f * exr) * 0.1, 0.0f, eve._choicesPNG[10], true);
}
