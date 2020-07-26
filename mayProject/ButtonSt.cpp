#include <DxLib.h>
#include "Event.h"
#include "ButtonSt.h"
#include "GameScene.h"
#include "MouseCtl.h"
#include "Player.h"

ButtonSt::ButtonSt()
{
}

ButtonSt::~ButtonSt()
{
}

void ButtonSt::Update(Event& eve, GameScene& game, Player& player, MouseCtl& mouse)
{
	if (mouse.GetClickTrg())
	{
		if (mouse.GetPos().x >= 600 && mouse.GetPos().x <= 600 + 150 && mouse.GetPos().y >= 345 && mouse.GetPos().y <= 345 + 75)
		{
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
			if (mouse.GetPos().x >= 600 && mouse.GetPos().x <= 600 + 150 && mouse.GetPos().y >= 200 && mouse.GetPos().y <= 200 + 75)
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
			// お金もらえる
			player.SetMoney(player.GetMoney() + 1000);
			eve._buttonEventFlg = true;
		}
		else
		{
			// 電流音
			PlaySoundMem(eve._soundSE[1], DX_PLAYTYPE_BACK, true);
			// 体力の1/4削る
			player.SetHP(player.GetHP() - static_cast<int>(static_cast<float>(player.GetMaxHP()) * (1.0f / 4.0f)));
			//_pushFlg = false;
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
		DrawGraph(420, 50, eve._messagePNG, true);

		if (eve._fateNum == -1)
		{
			// 押す
			DrawGraph(600, 200, eve._sentakusiPNG[4], true);
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
	DrawGraph(600, 345, eve._sentakusiPNG[10], true);
}
