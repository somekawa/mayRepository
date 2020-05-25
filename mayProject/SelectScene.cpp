#include "DxLib.h"
#include "Player.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "Menu.h"
#include "SelectScene.h"

// static�ϐ��̎���<�^>�N���X��::�ϐ��� = ������;
MODE SelectScene::modeTest = MODE::NON;

SelectScene::SelectScene()
{
	Init();
}

SelectScene::~SelectScene()
{
}

unique_Base SelectScene::Update(unique_Base own, const GameCtl& ctl)
{
	int x = 0;
	int y = 0;
	auto Mouse = GetMouseInput();                //�}�E�X�̓��͏�Ԏ擾
	GetMousePoint(&x, &y);					     //�}�E�X�̍��W�擾
	if (Mouse & MOUSE_INPUT_LEFT && !(_oldMouse & MOUSE_INPUT_LEFT)) {	 //�}�E�X�̍��{�^����������Ă�����
		// �����蔻��(NORMAL�I����)
		if (x >= 250 && x <= 250 + 400 && y >= 100 && y <= 100 + 150)
		{
			DeleteSoundMem(TitleScene::_titleBGM);
			if (CheckSoundMem(_seClick) == 0)
			{
				PlaySoundMem(_seClick, DX_PLAYTYPE_BACK, true);
				toGameFlg = true;
			}
			modeTest = MODE::NORMAL;
			//Player::LoadTest();
			Menu::LoadTest();
			//return std::make_unique<GameScene>();
		}
		// �����蔻��(HARD�I����)
		if (x >= 250 && x <= 250 + 400 && y >= 300 && y <= 300 + 150)
		{
			DeleteSoundMem(TitleScene::_titleBGM);
			if (CheckSoundMem(_seClick) == 0)
			{
				PlaySoundMem(_seClick, DX_PLAYTYPE_BACK, true);
				toGameFlg = true;
			}
			modeTest = MODE::HARD;
			//return std::make_unique<GameScene>();
		}

		// �����蔻��(�^�C�g���֖߂�)
		if (x >= 650 && x <= 650 + 200 && y >= 450 && y <= 450 + 100)
		{
			if (CheckSoundMem(_seClick) == 0)
			{
				PlaySoundMem(_seClick, DX_PLAYTYPE_BACK, true);
				toTitleFlg = true;
			}
			//return std::make_unique<TitleScene>();
		}
	}

	_oldMouse = Mouse;

	if (toGameFlg)
	{
		if (CheckSoundMem(_seClick) == 0)
		{
			DeleteSoundMem(_seClick);
			return std::make_unique<GameScene>();
		}
	}
	else if (toTitleFlg)
	{
		if (CheckSoundMem(_seClick) == 0)
		{
			DeleteSoundMem(_seClick);
			return std::make_unique<TitleScene>();
		}
	}

	Draw();

	// ���邭������Â����鏈��
	if (!_lightFlg)
	{
		if (_pngLight <= 255)
		{
			_pngLight++;
			if (_pngLight == 255)
			{
				_lightFlg = true;
			}
		}
	}

	if (_lightFlg)
	{
		_pngLight--;
		if (_pngLight <= 128)
		{
			_lightFlg = false;
		}
	}

	// ������Scene�̃��j�[�N�|�C���^��Ԃ� ���L�����Y�ꂸ��!
	return std::move(own);
}

bool SelectScene::Init(void)
{
	std::string normal = "image/normal.png";
	_normalPNG = LoadGraph(normal.c_str());

	std::string hard = "image/hard.png";
	_hardPNG = LoadGraph(hard.c_str());

	std::string renga = "image/renga.png";
	_backPNG = LoadGraph(renga.c_str());

	std::string titleBackButton = "image/titleBackButton.png";
	_titleBackPNG = LoadGraph(titleBackButton.c_str());

	_pngLight = 128;
	_lightFlg = false;
	// SE�e�X�g
	_seClick = LoadSoundMem("sound/se/click.mp3");
	return true;
}

void SelectScene::Draw(void)
{
	ClsDrawScreen();
	//���u�����h
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pngLight);
	DrawGraph(0, 0, _backPNG, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawGraph(650, 450, _titleBackPNG, true);
	DrawGraph(250, 100, _normalPNG, true);
	DrawGraph(250, 300, _hardPNG, true);
	ScreenFlip();
}