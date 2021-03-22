#include <DxLib.h>
#include "Event.h"
#include "../Scene/GameScene.h"
#include "../Common/MouseCtl.h"
#include "../Player.h"
#include "DeathTrapSt.h"

#define PI 3.141592653589793f

DeathTrapSt::DeathTrapSt()
{
	exr = 0.0f;
}

DeathTrapSt::~DeathTrapSt()
{
}

void DeathTrapSt::Update(Event& eve, GameScene& game, Player& player, MouseCtl& mouse)
{
	exr++;
	if (mouse.GetClickTrg())
	{
		if (mouse.GetPos().x >= 600 && mouse.GetPos().x <= 600 + 150 && mouse.GetPos().y >= 345 && mouse.GetPos().y <= 345 + 75)
		{
			exr = 0.0f;
			// クリック音
			PlaySoundMem(eve._soundSE[0], DX_PLAYTYPE_BACK, true);
			game.backFlg = true;
			game.eventState = EVENT_STATE::NON;
			eve._event = EVENT_STATE::NON;
			eve._fateNum = -1;
		}

		// 調べる
		if (!eve._trapFlg)
		{
			if (mouse.GetPos().x >= 600 && mouse.GetPos().x <= 600 + 150 && mouse.GetPos().y >= 200 && mouse.GetPos().y <= 200 + 75)
			{
				// クリック音
				PlaySoundMem(eve._soundSE[0], DX_PLAYTYPE_BACK, true);
				eve._trapFlg = true;
			}
		}
	}

	// 調べることにしたとき
	if (eve._trapFlg)
	{
		exr = 0.0f;
		// 即死トラップの発動
		player.SetHP(player.GetHP() - player.GetMaxHP());

		// ここで特定敵イベントを終了させる
		if (eve._eventMonsEncountFlg && !eve._eventMonsEndFlg)
		{
			eve._eventMonsEncountFlg = false;
			eve._eventMonsEndFlg = true;
		}
	}
}

void DeathTrapSt::Draw(Event& eve)
{
	// メッセージボックス
	DrawGraph(420, 50, eve._drawHandle["message"], true);
	// 去る
	DrawRotaGraph(600 + 150 / 2, 345 + 75 / 2, 0.9f + sinf(PI * 2.0f / 200.0f * exr) * 0.1, 0.0f, eve._choicesPNG[10], true);
	// 調べる
	DrawRotaGraph(600 + 150 / 2, 200 + 75 / 2, 0.9f + sinf(PI * 2.0f / 200.0f * exr) * 0.1, 0.0f, eve._choicesPNG[9], true);
	DrawGraph(200, 75, eve._eventImages["deathTrap"], true);

	if (eve._nowTrapFlg)
	{
		if (eve._eventMonsEncountFlg && !eve._eventMonsEndFlg)
		{
			DrawFormatString(450, 70, 0x000000, "背後に敵が迫っている…!");
		}
		else
		{
			DrawFormatString(450, 70, 0x000000, "即死トラップのようだ");
		}
	}
	else
	{
		DrawFormatString(450, 70, 0x000000, "怪しげな像がある");
		if (eve._eventMonsEncountFlg && !eve._eventMonsEndFlg)
		{
			DrawFormatString(450, 90, 0xff0000, "背後に敵が迫っている…!");
		}
	}
}