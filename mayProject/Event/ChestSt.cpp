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
			if (menu.GetAppraisalFlg())
			{
				menu.SetAppraisalFlg(false);
			}
		}

		// 宝箱チェック
		int boxNum = 0;
		for (int i = 0; i < 4; i++)
		{
			if (eve._chestPos[i].x == GameScene::plPosX && eve._chestPos[i].y == GameScene::plPosY)
			{
				boxNum = i;
			}
		}

		// 開ける
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

	// 宝箱を開けることにしたとき
	if (eve._pushFlg)
	{
		if (eve._fateNum == 1)
		{
			eve._pushFlg = false;
			eve._getFlg  = true;
		}
		else if (eve._fateNum == 0)
		{
			// ダメージ音
			PlaySoundMem(eve._soundSE[2], DX_PLAYTYPE_BACK, true);
			// 体力の1/4削る
			player.SetHP(player.GetHP() - static_cast<int>(static_cast<float>(player.GetMaxHP()) * (1.0f / 4.0f)));
			eve._pushFlg  = false;
			game.shakeFlg = true;
		}
		else
		{
			// 何も処理を行わない
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

		if (!mouse.GetClickTrg())
		{
			return;		// クリック状態で無ければreturnする
		}

		if (mouse.GetPos().x >= offsetPos.x && mouse.GetPos().x <= offsetPos.x + choicesPngSize.x && mouse.GetPos().y >= 200 && mouse.GetPos().y <= 200 + choicesPngSize.y)
		{
			if (menu.GetCanHaveItem() != 0)
			{
				// クリック音
				PlaySoundMem(eve._soundSE[0], DX_PLAYTYPE_BACK, true);
				const int randNum = GetRand(4);
				// 持ち物が満タンじゃなければ持てる
				menu.SetItem(item.GetPair(static_cast<int>(eve._chestItemMap[randNum]) - 1).second, item.GetPair(static_cast<int>(eve._chestItemMap[randNum]) - 1).first);
				eve._getFlg = false;
				eve._anounceFlg = false;
			}
		}
	}
}

void ChestSt::Draw(Event& eve, Menu& menu)
{
	// メッセージボックス
	DrawGraph(420, 50, eve._drawHandle["message"], true);

	// 宝箱チェック
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
		// 進む(宝箱無視)
		DrawRotaGraph(offsetPos.x + choicesPngSize.x / 2, offsetPos.y + choicesPngSize.y / 2, static_cast<double>(0.9f + (sinf(PI * 2.0f / 200.0f * exr) * 0.1f)), 0.0f, eve._choicesPNG[10], true);

		if (eve._fateNum == -1)
		{
			// 開ける
			DrawRotaGraph(offsetPos.x + choicesPngSize.x / 2, 200 + choicesPngSize.y / 2, static_cast<double>(0.9f + (sinf(PI * 2.0f / 200.0f * exr) * 0.1f)), 0.0f, eve._choicesPNG[3], true);
			DrawGraph(350, choicesPngSize.x, eve._chestPNG[0], true);
			DrawFormatString(tmpVec.x, tmpVec.y, color, "宝箱が置いてある");
		}

		// 鑑定アイテムを使ったときの描画
		if (menu.GetAppraisalFlg())
		{
			if (eve._chestBingo[tmpNum] == 1)
			{
				DrawFormatString(tmpVec.x, tmpVec.y, color, "\n\n特におかしいところはない");
			}
			if (eve._chestBingo[tmpNum] == 0)
			{
				DrawFormatString(tmpVec.x, tmpVec.y, color, "\n\nゴーストが取り憑いている");
			}
		}
	}
	else
	{
		if (eve._getFlg)
		{
			// 取る
			DrawRotaGraph(offsetPos.x + choicesPngSize.x / 2, 200 + choicesPngSize.y / 2, static_cast<double>(0.9f + (sinf(PI * 2.0f / 200.0f * exr) * 0.1f)), 0.0f, eve._choicesPNG[8], true);
		}

		if (eve._anounceFlg)
		{
			// 持ち物満タンだからもてない
			DrawFormatString(offsetPos.x, 180, 0xffffff, "所持品がいっぱいだ");
		}

		const VECTOR2 itemVec(350, 150);
		if (eve._fateNum == 1)
		{
			DrawGraph(itemVec.x, itemVec.y, eve._eventImages["chestInItem"], true);
			DrawFormatString(tmpVec.x, tmpVec.y, color, "アイテムが入っていた!");
		}
		else if (eve._fateNum == 0)
		{
			DrawGraph(itemVec.x, itemVec.y, eve._chestPNG[1], true);
			DrawFormatString(tmpVec.x, tmpVec.y, color, "ゴーストが現れ攻撃してきた!");
		}
		else
		{
			// 宝箱をすでに開けている
			DrawGraph(itemVec.x, itemVec.y, eve._eventImages["chestEmpty"], true);
			DrawFormatString(tmpVec.x, tmpVec.y, color, "宝箱は開いている");
		}

		// 去る(宝箱無視)
		DrawRotaGraph(offsetPos.x + choicesPngSize.x / 2, offsetPos.y + choicesPngSize.y / 2, 0.9f + sinf(PI * 2.0f / 200.0f * exr) * 0.1, 0.0f, eve._choicesPNG[10], true);
	}
}