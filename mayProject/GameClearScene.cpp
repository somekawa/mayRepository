#include "Dxlib.h"
#include <string>
#include "TitleScene.h"
#include "GameClearScene.h"

GameClearScene::GameClearScene()
{
	Init();
}

GameClearScene::~GameClearScene()
{
}

unique_Base GameClearScene::Update(unique_Base own, const GameCtl& ctl)
{
	// �Đ����łȂ���΍Đ����s��(0:�Đ����Ă��Ȃ�)
	// ���݂�Init�Ń��[�v�ݒ肵�Ă���B
	//if (CheckSoundMem(clearBGM) == 0)
	//{
	//	PlaySoundMem(clearBGM, DX_PLAYTYPE_LOOP, true);
	//}

	int x = 0;
	int y = 0;
	auto Mouse = GetMouseInput();                //�}�E�X�̓��͏�Ԏ擾
	GetMousePoint(&x, &y);					     //�}�E�X�̍��W�擾
	if (Mouse & MOUSE_INPUT_LEFT/* && !(oldMouse & MOUSE_INPUT_LEFT)*/) {	 //�}�E�X�̍��{�^����������Ă�����
		// �����蔻��(�Ƃ肠��������̃{�b�N�X��)
		if (x >= 650 && x <= 650 + 200 && y >= 500 && y <= 500 + 100)
		{
			PlaySoundMem(_seClick, DX_PLAYTYPE_BACK, true);
			DeleteSoundMem(_clearBGM);
			return std::make_unique<TitleScene>();
		}
	}

	//oldMouse = mouse;

	// ���񂾂�Â�����
	if (_pngBlend >= 128)
	{
		_pngBlend--;
	}

	Draw();
	// ������Scene�̃��j�[�N�|�C���^��Ԃ� ���L�����Y�ꂸ��!
	return std::move(own);
}

bool GameClearScene::Init(void)
{
	std::string gameclear = "image/gameclear.png";
	_gameClearPNG = LoadGraph(gameclear.c_str());

	std::string clear = "image/clear.png";
	_backPNG = LoadGraph(clear.c_str());

	std::string titleBackButton = "image/titleBackButton.png";
	_backTitleButtonPNG = LoadGraph(titleBackButton.c_str());

	_pngBlend = 256;

	_seClick = LoadSoundMem("sound/se/click.mp3");

	// BGM�e�X�g(�f�[�^�����\�傫��)
	_clearBGM = LoadSoundMem("sound/bgm/clear.mp3");
	PlaySoundMem(_clearBGM, DX_PLAYTYPE_LOOP, true);
	return true;
}

void GameClearScene::Draw(void)
{
	ClsDrawScreen();
	// ���Z�u�����h
	SetDrawBlendMode(DX_BLENDMODE_ADD, _pngBlend);
	DrawGraph(0, 0, _backPNG, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawGraph(0, 0, _gameClearPNG, true);
	DrawGraph(650, 500, _backTitleButtonPNG, true);
	ScreenFlip();
}