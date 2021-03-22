#include <DxLib.h>
#include "Event.h"
#include "../Scene/GameScene.h"
#include "../Enemy/Monster.h"
#include "../Card/Cards.h"
#include "../Common/MouseCtl.h"
#include "EventMonsSt.h"

#define PI 3.141592653589793f

EventMonsSt::EventMonsSt()
{
	exr = 0.0f;
}

EventMonsSt::~EventMonsSt()
{
}

void EventMonsSt::Update(Event& eve, GameScene& game, Monster& monster, Cards& cards, MouseCtl& mouse)
{
	exr++;
	eve._eventMonsEncountFlg = true;
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
			eve._fateNum = -1;
		}

		// 戦う
		if (!eve._eventMonsFlg)
		{
			if (mouse.GetPos().x >= 600 && mouse.GetPos().x <= 600 + 150 && mouse.GetPos().y >= 200 && mouse.GetPos().y <= 200 + 75)
			{
				exr = 0.0f;
				// クリック音
				PlaySoundMem(eve._soundSE[0], DX_PLAYTYPE_BACK, true);
				eve._eventMonsFlg = true;
				monster.SetEnemyNum(6, 0);
				cards.SetTurn(3);
			}
		}
	}
}

void EventMonsSt::Draw(Event& eve)
{
	DrawGraph(100, 75, eve._eventImages["mons"], true);

	// 戦闘に入る前
	if (!eve._eventMonsFlg)
	{
		// メッセージボックス
		DrawGraph(420, 50, eve._drawHandle["message"], true);
		// 去る
		DrawRotaGraph(600 + 150 / 2, 345 + 75 / 2, 0.9f + sinf(PI * 2.0f / 200.0f * exr) * 0.1, 0.0f, eve._choicesPNG[10], true);
		// 調べる
		DrawRotaGraph(600 + 150 / 2, 200 + 75 / 2, 0.9f + sinf(PI * 2.0f / 200.0f * exr) * 0.1, 0.0f, eve._choicesPNG[11], true);
		DrawFormatString(450, 70, 0x000000, "敵が道をふさいでいる\n何か良い方法はないだろうか..");
	}
}