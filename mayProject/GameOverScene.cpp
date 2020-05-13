#include "Dxlib.h"
#include <string>
#include "GameOverScene.h"
#include "TitleScene.h"

GameOverScene::GameOverScene()
{
	Init();
}

GameOverScene::~GameOverScene()
{
}

unique_Base GameOverScene::Update(unique_Base own, const GameCtl& ctl)
{
	// �Đ����łȂ���΍Đ����s��(0:�Đ����Ă��Ȃ�)
	// ���݂�Init�Ń��[�v�ݒ肵�Ă���B
	//if (CheckSoundMem(_overBGM) == 0)
	//{
	//	PlaySoundMem(_overBGM, DX_PLAYTYPE_LOOP, true);
	//}

	int x = 0;
	int y = 0;
	auto Mouse = GetMouseInput();                //�}�E�X�̓��͏�Ԏ擾
	GetMousePoint(&x, &y);					     //�}�E�X�̍��W�擾
	if (Mouse & MOUSE_INPUT_LEFT) {	 //�}�E�X�̍��{�^����������Ă�����
		// �����蔻��
		if (x >= 650 && x <= 650 + 200 && y >= 450 && y <= 450 + 100)
		{
			PlaySoundMem(_seClick, DX_PLAYTYPE_BACK, true);
			DeleteSoundMem(_overBGM);
			return std::make_unique<TitleScene>();
		}
	}


	Draw();

	// ���񂾂񖾂邭
	if (_pngBlend < 255)
	{
		_pngBlend++;
	}

	// ������Scene�̃��j�[�N�|�C���^��Ԃ� ���L�����Y�ꂸ��!
	return std::move(own);
}

bool GameOverScene::Init(void)
{
	std::string blood = "image/blood.png";
	_bloodPNG = LoadGraph(blood.c_str());

	std::string renga = "image/renga.png";
	_backPNG = LoadGraph(renga.c_str());

	std::string over = "image/over.png";
	_gameOverPNG = LoadGraph(over.c_str());

	std::string titleBackButton = "image/titleBackButton.png";
	_titleBackPNG = LoadGraph(titleBackButton.c_str());

	_pngBlend = 0;

	_seClick = LoadSoundMem("sound/se/click.mp3");

	// BGM�e�X�g
	_overBGM = LoadSoundMem("sound/bgm/over.mp3");
	PlaySoundMem(_overBGM, DX_PLAYTYPE_LOOP, true);
	return true;
}

void GameOverScene::Draw(void)
{
	ClsDrawScreen();
	// ���u�����h
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawGraph(0, 0, _backPNG, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// ���u�����h
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pngBlend);
	DrawGraph(0, 0, _bloodPNG, true);
	DrawGraph(200, 250, _gameOverPNG, true);
	DrawGraph(650, 450, _titleBackPNG, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	ScreenFlip();
}
