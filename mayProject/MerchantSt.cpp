#include <DxLib.h>
#include "Event.h"
#include "MerchantSt.h"
#include "GameScene.h"
#include "MouseCtl.h"
#include "Player.h"
#include "Item.h"
#include "Menu.h"

#define PI 3.141592653589793f

MerchantSt::MerchantSt()
{
	exr = 0.0f;
	choicesPngSize = { 150,75 };
	offsetPos = { 600,345 };
}

MerchantSt::~MerchantSt()
{
}

void MerchantSt::Update(Event& eve, GameScene& game, Player& player, MouseCtl& mouse, Item& item, Menu& menu)
{
	exr++;
	if (mouse.GetClickTrg())
	{
		if (mouse.GetPos().x >= offsetPos.x && mouse.GetPos().x <= offsetPos.x + choicesPngSize.x && mouse.GetPos().y >= offsetPos.y && mouse.GetPos().y <= offsetPos.y + choicesPngSize.y)
		{
			// クリック音
			PlaySoundMem(eve._soundSE[0], DX_PLAYTYPE_BACK, true);
			// 去る
			exr = 0.0f;
			game.backFlg = true;
			eve._chooseNum = -1;
			game.eventState = EVENT_STATE::NON;
			eve._event = EVENT_STATE::NON;
			eve._buyFlg = false;
			eve._chooseFlg = false;
			eve._nonMoneyFlg = false;
		}

		if (mouse.GetPos().x >= offsetPos.x && mouse.GetPos().x <= offsetPos.x + choicesPngSize.x && mouse.GetPos().y >= 200 && mouse.GetPos().y <= 200 + choicesPngSize.y)
		{
			if (!eve._buyFlg)
			{
				// クリック音
				PlaySoundMem(eve._soundSE[0], DX_PLAYTYPE_BACK, true);
				// 購入する
				// 購入したらプレイヤーの持ち物の中に追加する
				eve._buyFlg = true;
			}
		}
	}

	// 商人の持ち物との当たり判定
	if (eve._buyFlg)
	{
		if (mouse.GetClickTrg())
		{
			// 次のページへが押された時
			if (mouse.GetPos().x >= 490 && mouse.GetPos().x <= 490 + 100 && mouse.GetPos().y >= 300 && mouse.GetPos().y <= 300 + 100)
			{
				eve._itemNextPage = !eve._itemNextPage;
			}

			for (int i = 0; i <= 7; i++)
			{
				if (mouse.GetPos().x >= item.GetPos(i).x && mouse.GetPos().x <= item.GetPos(i).x + 100 && mouse.GetPos().y >= item.GetPos(i).y && mouse.GetPos().y <= item.GetPos(i).y + 100)
				{
					if (item.GetPair(i).second != ITEM::NON)
					{
						// 購入するかのボタンを表示する
						eve._chooseFlg = true;
						eve._chooseNum = i;
						if (!eve._itemNextPage)
						{
							eve._itemInfo = item.GetPair(i).second;
						}
						else
						{
							eve._itemInfo = static_cast<ITEM>(static_cast<int>(item.GetPair(i).second) + 8);
						}
					}
				}
			}
		}
	}

	if (eve._chooseFlg)
	{
		// 所持金が足りない
		if (player.GetMoney() < item.GetCostMoney(static_cast<int>(eve._itemInfo) - 1))
		{
			eve._nonMoneyFlg = true;
		}
		else
		{
			eve._nonMoneyFlg = false;
		}

		if (mouse.GetClickTrg())
		{
			if (mouse.GetPos().x >= offsetPos.x && mouse.GetPos().x <= offsetPos.x + choicesPngSize.x && mouse.GetPos().y >= 200 && mouse.GetPos().y <= 200 + choicesPngSize.y)
			{
				// クリック音
				PlaySoundMem(eve._soundSE[0], DX_PLAYTYPE_BACK, true);

				if (item.GetPair(eve._chooseNum).second != ITEM::NON)
				{
					// choosenum→static_cast<int>(_itemInfo) - 1に変更
					if (player.GetMoney() < item.GetCostMoney(static_cast<int>(eve._itemInfo) - 1))
					{
						eve._chooseFlg = false;
					}
					else
					{
						// 所持品がいっぱいで入れられない
						if (menu.GetCanHaveItem() == 0)
						{
							eve._chooseFlg = false;
						}
						else
						{
							// プレイヤーのお金が減る
							// _chooseNumだと、装備に対応できない
							player.SetMoney(player.GetMoney() - item.GetCostMoney(static_cast<int>(eve._itemInfo) - 1));
							if (!eve._itemNextPage)
							{
								menu.Setitem(item.GetPair(eve._chooseNum).second, item.GetPair(eve._chooseNum).first);
							}
							else
							{
								menu.Setitem(item.GetPair(eve._chooseNum + 8).second, item.GetPair(eve._chooseNum + 8).first);
							}
							eve._chooseFlg = false;
						}
					}
				}
			}
		}
	}
}

void MerchantSt::Draw(Event& eve, Player& player,Item& item, Menu& menu)
{
	// 人画像
	DrawGraph(100, 0, eve.eventImages["syounin"], true);
	// メッセージボックス
	DrawGraph(420, 50, eve._drawHandle["message"], true);
	DrawFormatString(450, 70, 0x000000, "商人:\n何か買うか?");

	if (eve._buyFlg)
	{
		DrawGraph(200, 0, eve._drawHandle["frame"], true);

		// 1ページ目
		if (!eve._itemNextPage)
		{
			// アイテム表示
			for (int i = 0; i <= 7; i++)
			{
				DrawGraph(item.GetPos(i).x, item.GetPos(i).y, eve._drawHandle["itembox"], true);
				DrawGraph(item.GetPos(i).x, item.GetPos(i).y, item.GetPair(i).first, true);
			}
		}
		else
		{
			// 2ページ目
			// アイテム表示
			for (int i = 0; i <= 7; i++)
			{
				DrawGraph(item.GetPos(i).x, item.GetPos(i).y, eve._drawHandle["itembox"], true);
				DrawGraph(item.GetPos(i).x, item.GetPos(i).y, item.GetPair(i + 8).first, true);
			}
		}

		// 選択中のアイテムの枠
		if (eve._chooseNum >= 0)
		{
			DrawGraph(item.GetPos(eve._chooseNum).x, item.GetPos(eve._chooseNum).y, eve._drawHandle["itemChoice"], true);
		}

		// 現在の所持金の表示
		DrawFormatString(470, 485, 0x000000, "所持金:%d円", player.GetMoney());

		// 商品の次のページへの矢印
		DrawRotaGraph(490 + 50, 300 + 50, 0.7f + sinf(PI * 2.0f / 200.0f * exr) * 0.1,0.0f,eve._drawHandle["yajirusi"], true);
	}

	// 去る
	DrawRotaGraph(offsetPos.x + choicesPngSize.x / 2, offsetPos.y + choicesPngSize.y / 2, 0.9f + sinf(PI * 2.0f / 200.0f * exr) * 0.1, 0.0f, eve._choicesPNG[10], true);


	if (!eve._buyFlg || eve._chooseFlg)
	{
		// 購入
		DrawRotaGraph(offsetPos.x + choicesPngSize.x / 2, 200 + choicesPngSize.y / 2, 0.9f + sinf(PI * 2.0f / 200.0f * exr) * 0.1, 0.0f,eve._choicesPNG[0], true);
		if (menu.GetCanHaveItem() == 0)
		{
			// 持ち物満タンだからもてないよ
			DrawFormatString(offsetPos.x, 160, 0xffffff, "所持品がいっぱいだ");
		}
	}

	if (eve._buyFlg && eve._chooseFlg)
	{
		if (eve._nonMoneyFlg)
		{
			// 所持金が足りないよ
			DrawFormatString(offsetPos.x, 180, 0xffffff, "所持金が足りない");
		}
	}

	// 文字表示(商品選択中のみ)
	if (eve._chooseNum != -1)
	{
		DrawFormatString(300, 450, 0x000000, "%s\n", item.GetExplanation(static_cast<int>(eve._itemInfo) - 1));
		DrawFormatString(300, 470, 0x000000, "%d円\n", item.GetCostMoney(static_cast<int>(eve._itemInfo) - 1));
	}
}
