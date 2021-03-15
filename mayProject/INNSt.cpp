#include <DxLib.h>
#include "Event.h"
#include "INNSt.h"
#include "GameScene.h"
#include "MouseCtl.h"
#include "Player.h"

#define PI 3.141592653589793f

INNSt::INNSt()
{
	exr = 0.0f;
	choicesPngSize = { 150,75 };
	offsetPos = { 600,345 };
	needMoney = 50;
}

INNSt::~INNSt()
{
}

void INNSt::Update(Event& eve, GameScene& game, Player& player, MouseCtl& mouse)
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
			eve._healinnFlg = false;
			eve._nonMoneyFlg = false;
		}

		// 回復を頼む
		if (!eve._healinnFlg)
		{
			if (player.GetMoney() < player.GetNowLevel() * needMoney)
			{
				// 所持金が足りなくて回復できない
				eve._healinnFlg = true;
				eve._nonMoneyFlg = true;
			}

			if (mouse.GetPos().x >= offsetPos.x && mouse.GetPos().x <= offsetPos.x + choicesPngSize.x && mouse.GetPos().y >= 200 && mouse.GetPos().y <= 200 + choicesPngSize.y)
			{
				// クリック音
				PlaySoundMem(eve._soundSE[0], DX_PLAYTYPE_BACK, true);

				// お金が減るがHP全回復
				// 減るお金はその時の自分のレベルによって異なる
				player.SetMoney(player.GetMoney() - player.GetNowLevel() * needMoney);
				player.SetHP(player.GetMaxHP());
				eve._healinnFlg = true;
			}
		}
	}
}

void INNSt::Draw(Event& eve, Player& player)
{
	// 人画像
	DrawGraph(0, 0, eve.eventImages["heal_human"], true);
	// 去る
	DrawRotaGraph(offsetPos.x + choicesPngSize.x / 2, offsetPos.y + choicesPngSize.y / 2, 0.9f + sinf(PI * 2.0f / 200.0f * exr) * 0.1, 0.0f, eve._choicesPNG[10], true);
	// メッセージボックス
	DrawGraph(420, 50, eve._drawHandle["message"], true);

	if (eve._nonMoneyFlg)
	{
		DrawFormatString(450, 70, 0x000000, "怪しい老婆:\n%金が足らんようじゃの。\n去るがよい");
	}

	if (!eve._healinnFlg)
	{
		// 頼む
		DrawRotaGraph(offsetPos.x + choicesPngSize.x / 2, 200 + choicesPngSize.y / 2, 0.9f + sinf(PI * 2.0f / 200.0f * exr) * 0.1, 0.0f, eve._choicesPNG[2], true);
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
