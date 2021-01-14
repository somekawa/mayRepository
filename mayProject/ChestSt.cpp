#include <DxLib.h>
#include "Event.h"
#include "ChestSt.h"
#include "GameScene.h"
#include "MouseCtl.h"
#include "Player.h"
#include "Menu.h"
#include "Item.h"

#define PI 3.141592653589793f

ChestSt::ChestSt()
{
	exr = 0.0f;
}

ChestSt::~ChestSt()
{
}

void ChestSt::Update(Event& eve,GameScene& game, Player& player, Menu& menu, Item& item, MouseCtl& mouse)
{
	exr++;
	if (mouse.GetClickTrg())
	{
		if (mouse.GetPos().x >= 600 && mouse.GetPos().x <= 600 + 150 && mouse.GetPos().y >= 345 && mouse.GetPos().y <= 345 + 75)
		{
			exr = 0.0f;
			// クリック音
			PlaySoundMem(eve._soundSE[0], DX_PLAYTYPE_BACK, true);
			// 去る
			game.backFlg = true;
			game.eventState = EVENT_STATE::NON;
			eve._event = EVENT_STATE::NON;
			eve._pushFlg = false;
			eve._fateNum = -1;
			eve._anounceFlg = false;
			eve._getFlg = false;
			if (menu.GetMeganeFlg())
			{
				menu.SetMeganeFlg(false);
			}
		}

		// 宝箱チェック
		int a = 0;
		for (int i = 0; i < 4; i++)
		{
			if (eve._chestPos[i].x == GameScene::plPosX && eve._chestPos[i].y == GameScene::plPosY)
			{
				a = i;
			}
		}
		// 開ける
		if (eve._fateNum == -1 && eve._chestOpen[a] == 0)
		{
			if (mouse.GetPos().x >= 600 && mouse.GetPos().x <= 600 + 150 && mouse.GetPos().y >= 200 && mouse.GetPos().y <= 200 + 75)
			{
				eve._pushFlg = true;
				eve._chestOpen[a] = 1;
				eve._fateNum = eve._chestBingo[a];
				if (menu.GetMeganeFlg())
				{
					menu.SetMeganeFlg(false);
				}
			}
		}
	}

	// 宝箱を開けることにしたとき
	if (eve._pushFlg)
	{
		if (eve._fateNum == 1)
		{
			eve._pushFlg = false;
			eve._getFlg = true;
		}
		else if (eve._fateNum == 0)
		{
			// ダメージ音
			PlaySoundMem(eve._soundSE[2], DX_PLAYTYPE_BACK, true);
			// 体力の1/4削る
			player.SetHP(player.GetHP() - static_cast<int>(static_cast<float>(player.GetMaxHP()) * (1.0f / 4.0f)));
			eve._pushFlg = false;
			game.shakeFlg = true;
		}
	}

	if (eve._getFlg)
	{
		// 持ち物が満タンかどうか調べる
		if (menu.GetCanHaveItem() != 0)
		{
			eve._anounceFlg = false;
		}
		else
		{
			eve._anounceFlg = true;
		}

		if (mouse.GetClickTrg())
		{
			if (mouse.GetPos().x >= 600 && mouse.GetPos().x <= 600 + 150 && mouse.GetPos().y >= 200 && mouse.GetPos().y <= 200 + 75)
			{
				if (menu.GetCanHaveItem() != 0)
				{
					// クリック音
					PlaySoundMem(eve._soundSE[0], DX_PLAYTYPE_BACK, true);
					int randNum = GetRand(4);
					// 持ち物が満タンじゃなければ持てる
					menu.Setitem(item.GetPair(static_cast<int>(eve._chestItemMap[randNum]) - 1).second, item.GetPair(static_cast<int>(eve._chestItemMap[randNum]) - 1).first);
					eve._getFlg = false;
					eve._anounceFlg = false;
				}
			}
		}
	}
}

void ChestSt::Draw(Event& eve, Menu& menu)
{
	// メッセージボックス
	DrawGraph(420, 50, eve._drawHandle["message"], true);

	// 宝箱チェック
	int a = 0;
	for (int i = 0; i < 4; i++)
	{
		if (eve._chestPos[i].x == GameScene::plPosX && eve._chestPos[i].y == GameScene::plPosY)
		{
			a = i;
		}
	}
	if (eve._chestOpen[a] == 0)
	{
		// 進む(宝箱無視)
		DrawRotaGraph(600 + 150 / 2, 345 + 75 / 2, 0.9f + sinf(PI * 2.0f / 200.0f * exr) * 0.1, 0.0f, eve._sentakusiPNG[10], true);

		if (eve._fateNum == -1)
		{
			// 開ける
			DrawRotaGraph(600 + 150 / 2, 200 + 75 / 2, 0.9f + sinf(PI * 2.0f / 200.0f * exr) * 0.1, 0.0f, eve._sentakusiPNG[3], true);
			DrawGraph(350, 150, eve._chestPNG[0], true);
			DrawFormatString(450, 70, 0x000000, "宝箱が置いてある");
		}

		// 鑑定アイテムを使ったときの描画
		if (menu.GetMeganeFlg())
		{
			if (eve._chestBingo[a] == 1)
			{
				DrawFormatString(450, 70, 0xff0000, "\n\n特におかしいところはない");
			}
			if (eve._chestBingo[a] == 0)
			{
				DrawFormatString(450, 70, 0xff0000, "\n\nゴーストが取り憑いている");
			}
		}
	}
	else
	{
		if (eve._getFlg)
		{
			// 取る
			DrawRotaGraph(600 + 150 / 2, 200 + 75 / 2, 0.9f + sinf(PI * 2.0f / 200.0f * exr) * 0.1, 0.0f, eve._sentakusiPNG[8], true);
		}

		if (eve._anounceFlg)
		{
			// 持ち物満タンだからもてない
			DrawFormatString(600, 180, 0xffffff, "所持品がいっぱいだ");
		}

		if (eve._fateNum == 1)
		{
			DrawGraph(350, 150, eve.eventImages["chestInItem"], true);
			DrawFormatString(450, 70, 0x000000, "アイテムが入っていた!");
		}
		else if (eve._fateNum == 0)
		{
			DrawGraph(350, 150, eve._chestPNG[1], true);
			DrawFormatString(450, 70, 0x000000, "ゴーストが現れ攻撃してきた!");
		}
		else
		{
			// 宝箱をすでに開けている
			DrawFormatString(450, 70, 0x000000, "宝箱は開いている");
			DrawGraph(350, 150, eve.eventImages["chestKara"], true);
		}

		// 去る(宝箱無視)
		DrawRotaGraph(600 + 150 / 2, 345 + 75 / 2, 0.9f + sinf(PI * 2.0f / 200.0f * exr) * 0.1, 0.0f, eve._sentakusiPNG[10], true);
	}
}
