#include <DxLib.h>
#include "Event.h"
#include "YadoSt.h"
#include "GameScene.h"
#include "MouseCtl.h"
#include "Player.h"

YadoSt::YadoSt()
{
}

YadoSt::~YadoSt()
{
}

void YadoSt::Update(Event& eve, GameScene& game, Player& player, MouseCtl& mouse)
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
			eve._healYadoFlg = false;
			eve._nonMoneyFlg = false;
		}

		// 回復を頼む
		if (!eve._healYadoFlg)
		{
			if (player.GetMoney() < player.GetNowLevel() * 100)
			{
				// 所持金が足りなくて回復できない
				eve._healYadoFlg = true;
				eve._nonMoneyFlg = true;
			}

			if (mouse.GetPos().x >= 600 && mouse.GetPos().x <= 600 + 150 && mouse.GetPos().y >= 200 && mouse.GetPos().y <= 200 + 75)
			{
				// クリック音
				PlaySoundMem(eve._soundSE[0], DX_PLAYTYPE_BACK, true);

				// お金が減るがHP全回復
				// 減るお金はその時の自分のレベルによって異なる
				player.SetMoney(player.GetMoney() - player.GetNowLevel() * 50);
				player.SetHP(player.GetMaxHP());
				eve._healYadoFlg = true;
			}
		}
	}
}

void YadoSt::Draw(Event& eve, Player& player)
{
	// 人画像
	DrawGraph(0, 0, eve.eventImages["heal_human"], true);
	// 去る
	DrawGraph(600, 345, eve._sentakusiPNG[10], true);
	// メッセージボックス
	DrawGraph(420, 50, eve._messagePNG, true);

	if (eve._nonMoneyFlg)
	{
		DrawFormatString(450, 70, 0x000000, "怪しい老婆:\n%金が足らんようじゃの。\n去るがよい");
	}

	if (!eve._healYadoFlg)
	{
		// 頼む
		DrawGraph(600, 200, eve._sentakusiPNG[2], true);
		DrawFormatString(450, 70, 0x000000, "怪しい老婆:\n%d円で回復してやろうか? \n(所持金:%d円)", player.GetNowLevel() * 50, player.GetMoney());
	}
	else
	{
		if (!eve._nonMoneyFlg)
		{
			DrawFormatString(450, 70, 0x000000, "怪しい老婆:\nﾋｯﾋｯﾋｯﾋｯ...");
		}
	}
}
