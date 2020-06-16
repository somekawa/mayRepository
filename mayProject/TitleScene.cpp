#include "DxLib.h"
#include "SceneMng.h"
#include "GameCtl.h"
#include "SelectScene.h"
#include "TitleScene.h"
#include "MouseCtl.h"

// static�ϐ��̎���<�^>�N���X��::�ϐ��� = ������;
int TitleScene::_titleBGM = 0;

TitleScene::TitleScene()
{
	Init();
}

TitleScene::~TitleScene()
{
	delete mouse;
}

bool TitleScene::Init(void)
{
	mouse = new MouseCtl();
	PngInit();
	_pngLight = 128;
	_lightFlg = false;
	_seFlg = false;
	_seClick = LoadSoundMem("sound/se/click.mp3");
	_titleBGM = LoadSoundMem("sound/bgm/title.mp3");
	PlaySoundMem(_titleBGM, DX_PLAYTYPE_LOOP, true);
	return true;
}

void TitleScene::PngInit(void)
{
	std::string title = "image/title.png";
	_titlePNG = LoadGraph(title.c_str());

	std::string start = "image/start.png";
	_startPNG = LoadGraph(start.c_str());
}

unique_Base TitleScene::Update(unique_Base own, const GameCtl& ctl)
{
	mouse->UpDate();
	if (mouse->GetClickTrg()) {	 //�}�E�X�̍��{�^����������Ă�����
		// �����蔻��
		if (mouse->GetPos().x >= 250 && mouse->GetPos().x <= 250 + 400 && mouse->GetPos().y >= 400 && mouse->GetPos().y <= 400+150)
		{
			if (CheckSoundMem(_seClick) == 0)
			{
				PlaySoundMem(_seClick, DX_PLAYTYPE_BACK, true);
				_seFlg = true;
			}
		}
	}

	Draw();

	if (_seFlg && CheckSoundMem(_seClick) == 0)
	{
		DeleteSoundMem(_seClick);
		return std::make_unique<SelectScene>();
	}

	// ���邳��������
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

void TitleScene::Draw(void)
{
	ClsDrawScreen();
	//���u�����h
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pngLight);
	DrawGraph(0, 0,_titlePNG, true);
	DrawGraph(250, 400, _startPNG, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	ScreenFlip();
}