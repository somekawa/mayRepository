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
	choicesPngSize = { 150,75 };
	offsetPos = { 600,345 };
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
		if (mouse.GetPos().x >= offsetPos.x && mouse.GetPos().x <= offsetPos.x + choicesPngSize.x &&
			mouse.GetPos().y >= offsetPos.y && mouse.GetPos().y <= offsetPos.y + choicesPngSize.y)
		{
			exr = 0.0f;
			// �N���b�N��
			PlaySoundMem(eve._soundSE[0], DX_PLAYTYPE_BACK, true);
			// ����
			game.backFlg = true;
			game.eventState = EVENT_STATE::NON;
			eve._event = EVENT_STATE::NON;
			eve._fateNum = -1;
		}

		// �키
		if (!eve._eventMonsFlg)
		{
			if (mouse.GetPos().x >= offsetPos.x && mouse.GetPos().x <= offsetPos.x + choicesPngSize.x &&
				mouse.GetPos().y >= 200 && mouse.GetPos().y <= 200 + choicesPngSize.y)
			{
				exr = 0.0f;
				// �N���b�N��
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

	// �퓬�ɓ���O
	if (!eve._eventMonsFlg)
	{
		// ���b�Z�[�W�{�b�N�X
		DrawGraph(420, 50, eve._drawHandle["message"], true);
		// ����
		DrawRotaGraph(offsetPos.x + choicesPngSize.x / 2, offsetPos.y + choicesPngSize.y / 2, static_cast<double>(0.9f + (sinf(PI * 2.0f / 200.0f * exr) * 0.1f)), 0.0f, eve._choicesPNG[10], true);
		// ���ׂ�
		DrawRotaGraph(offsetPos.x + choicesPngSize.x / 2, 200 + choicesPngSize.y / 2, static_cast<double>(0.9f + (sinf(PI * 2.0f / 200.0f * exr) * 0.1f)), 0.0f, eve._choicesPNG[11], true);
		DrawFormatString(450, 70, 0x000000, "�G�������ӂ����ł���\n�����ǂ����@�͂Ȃ����낤��..");
	}
}