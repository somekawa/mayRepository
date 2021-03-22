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
			// �N���b�N��
			PlaySoundMem(eve._soundSE[0], DX_PLAYTYPE_BACK, true);
			game.backFlg = true;
			game.eventState = EVENT_STATE::NON;
			eve._event = EVENT_STATE::NON;
			eve._fateNum = -1;
		}

		// ���ׂ�
		if (!eve._trapFlg)
		{
			if (mouse.GetPos().x >= 600 && mouse.GetPos().x <= 600 + 150 && mouse.GetPos().y >= 200 && mouse.GetPos().y <= 200 + 75)
			{
				// �N���b�N��
				PlaySoundMem(eve._soundSE[0], DX_PLAYTYPE_BACK, true);
				eve._trapFlg = true;
			}
		}
	}

	// ���ׂ邱�Ƃɂ����Ƃ�
	if (eve._trapFlg)
	{
		exr = 0.0f;
		// �����g���b�v�̔���
		player.SetHP(player.GetHP() - player.GetMaxHP());

		// �����œ���G�C�x���g���I��������
		if (eve._eventMonsEncountFlg && !eve._eventMonsEndFlg)
		{
			eve._eventMonsEncountFlg = false;
			eve._eventMonsEndFlg = true;
		}
	}
}

void DeathTrapSt::Draw(Event& eve)
{
	// ���b�Z�[�W�{�b�N�X
	DrawGraph(420, 50, eve._drawHandle["message"], true);
	// ����
	DrawRotaGraph(600 + 150 / 2, 345 + 75 / 2, 0.9f + sinf(PI * 2.0f / 200.0f * exr) * 0.1, 0.0f, eve._choicesPNG[10], true);
	// ���ׂ�
	DrawRotaGraph(600 + 150 / 2, 200 + 75 / 2, 0.9f + sinf(PI * 2.0f / 200.0f * exr) * 0.1, 0.0f, eve._choicesPNG[9], true);
	DrawGraph(200, 75, eve._eventImages["deathTrap"], true);

	if (eve._nowTrapFlg)
	{
		if (eve._eventMonsEncountFlg && !eve._eventMonsEndFlg)
		{
			DrawFormatString(450, 70, 0x000000, "�w��ɓG�������Ă���c!");
		}
		else
		{
			DrawFormatString(450, 70, 0x000000, "�����g���b�v�̂悤��");
		}
	}
	else
	{
		DrawFormatString(450, 70, 0x000000, "�������ȑ�������");
		if (eve._eventMonsEncountFlg && !eve._eventMonsEndFlg)
		{
			DrawFormatString(450, 90, 0xff0000, "�w��ɓG�������Ă���c!");
		}
	}
}