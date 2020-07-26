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
			// �N���b�N��
			PlaySoundMem(eve._soundSE[0], DX_PLAYTYPE_BACK, true);
			// ����
			game.backFlg = true;
			game.eventState = EVENT_STATE::NON;
			eve._event = EVENT_STATE::NON;
			eve._healYadoFlg = false;
			eve._nonMoneyFlg = false;
		}

		// �񕜂𗊂�
		if (!eve._healYadoFlg)
		{
			if (player.GetMoney() < player.GetNowLevel() * 100)
			{
				// ������������Ȃ��ĉ񕜂ł��Ȃ�
				eve._healYadoFlg = true;
				eve._nonMoneyFlg = true;
			}

			if (mouse.GetPos().x >= 600 && mouse.GetPos().x <= 600 + 150 && mouse.GetPos().y >= 200 && mouse.GetPos().y <= 200 + 75)
			{
				// �N���b�N��
				PlaySoundMem(eve._soundSE[0], DX_PLAYTYPE_BACK, true);

				// ���������邪HP�S��
				// ���邨���͂��̎��̎����̃��x���ɂ���ĈقȂ�
				player.SetMoney(player.GetMoney() - player.GetNowLevel() * 50);
				player.SetHP(player.GetMaxHP());
				eve._healYadoFlg = true;
			}
		}
	}
}

void YadoSt::Draw(Event& eve, Player& player)
{
	// �l�摜
	DrawGraph(0, 0, eve.eventImages["heal_human"], true);
	// ����
	DrawGraph(600, 345, eve._sentakusiPNG[10], true);
	// ���b�Z�[�W�{�b�N�X
	DrawGraph(420, 50, eve._messagePNG, true);

	if (eve._nonMoneyFlg)
	{
		DrawFormatString(450, 70, 0x000000, "�������V�k:\n%���������悤����́B\n���邪�悢");
	}

	if (!eve._healYadoFlg)
	{
		// ����
		DrawGraph(600, 200, eve._sentakusiPNG[2], true);
		DrawFormatString(450, 70, 0x000000, "�������V�k:\n%d�~�ŉ񕜂��Ă�낤��? \n(������:%d�~)", player.GetNowLevel() * 50, player.GetMoney());
	}
	else
	{
		if (!eve._nonMoneyFlg)
		{
			DrawFormatString(450, 70, 0x000000, "�������V�k:\n˯˯˯˯...");
		}
	}
}
