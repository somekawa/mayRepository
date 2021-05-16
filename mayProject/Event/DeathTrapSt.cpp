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
	choicesPngSize = { 150,75 };
	offsetPos = { 600,345 };
}

DeathTrapSt::~DeathTrapSt()
{
}

void DeathTrapSt::Update(Event& eve, GameScene& game, Player& player, MouseCtl& mouse)
{
	exr++;
	if (mouse.GetClickTrg())
	{
		if (mouse.GetPos().x >= offsetPos.x && mouse.GetPos().x <= offsetPos.x + choicesPngSize.x &&
			mouse.GetPos().y >= offsetPos.y && mouse.GetPos().y <= offsetPos.y + choicesPngSize.y)
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
			if (mouse.GetPos().x >= offsetPos.x && mouse.GetPos().x <= offsetPos.x + choicesPngSize.x &&
				mouse.GetPos().y >= 200 && mouse.GetPos().y <= 200 + choicesPngSize.y)
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
	DrawRotaGraph(offsetPos.x + choicesPngSize.x / 2, offsetPos.y + choicesPngSize.y / 2, static_cast<double>(0.9f + (sinf(PI * 2.0f / 200.0f * exr) * 0.1f)), 0.0f, eve._choicesPNG[10], true);
	// ���ׂ�
	DrawRotaGraph(offsetPos.x + choicesPngSize.x / 2, 200 + choicesPngSize.y / 2, static_cast<double>(0.9f + (sinf(PI * 2.0f / 200.0f * exr) * 0.1f)), 0.0f, eve._choicesPNG[9], true);
	DrawGraph(200, 75, eve._eventImages["deathTrap"], true);

	const VECTOR2 tmpVec(450, 70);
	const unsigned int color(0x000000);

	if (eve._nowTrapFlg)
	{
		if (eve._eventMonsEncountFlg && !eve._eventMonsEndFlg)
		{
			DrawFormatString(tmpVec.x, tmpVec.y, color, "�w��ɓG�������Ă���c!");
		}
		else
		{
			DrawFormatString(tmpVec.x, tmpVec.y, color, "�����g���b�v�̂悤��");
		}
	}
	else
	{
		DrawFormatString(tmpVec.x, tmpVec.y, color, "�������ȑ�������");
		if (eve._eventMonsEncountFlg && !eve._eventMonsEndFlg)
		{
			DrawFormatString(450, 90, 0xff0000, "�w��ɓG�������Ă���c!");
		}
	}
}