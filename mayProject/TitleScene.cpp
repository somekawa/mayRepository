#include "DxLib.h"
#include "SceneMng.h"
#include "GameCtl.h"
#include "GameClearScene.h"
#include "SelectScene.h"
#include "TitleScene.h"

// static�ϐ��̎���<�^>�N���X��::�ϐ��� = ������;
int TitleScene::_titleBGM = 0;

TitleScene::TitleScene()
{
	Init();
}

TitleScene::~TitleScene()
{
}

bool TitleScene::Init(void)
{
	pngInit();
	_pngLight = 128;
	_lightFlg = false;
	_seFlg = false;
	_seClick = LoadSoundMem("sound/se/click.mp3");
	_titleBGM = LoadSoundMem("sound/bgm/title.mp3");
	PlaySoundMem(_titleBGM, DX_PLAYTYPE_LOOP, true);
	return true;
}

void TitleScene::pngInit(void)
{
	std::string title = "image/title.png";
	_titlePNG = LoadGraph(title.c_str());

	std::string start = "image/start.png";
	_startPNG = LoadGraph(start.c_str());
}

unique_Base TitleScene::Update(unique_Base own, const GameCtl& ctl)
{
	//if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_F1]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_F1]))
	//{
	//	return std::make_unique<GameScene>();
	//}
	//
	// �Đ����łȂ���΍Đ����s��(0:�Đ����Ă��Ȃ�)
	// ���݂�Init�Ń��[�v�ݒ肵�Ă���B
	//if (CheckSoundMem(_titleBGM) == 0)
	//{
	//	PlaySoundMem(_titleBGM, DX_PLAYTYPE_LOOP, true);
	//}

	int x = 0;
	int y = 0;
	auto Mouse = GetMouseInput();                //�}�E�X�̓��͏�Ԏ擾
	GetMousePoint(&x, &y);					     //�}�E�X�̍��W�擾
	if (Mouse & MOUSE_INPUT_LEFT && !(_oldMouse & MOUSE_INPUT_LEFT)) {	 //�}�E�X�̍��{�^����������Ă�����
		// �����蔻��
		if (x >= 250 && x <= 250 + 400 && y >= 400 && y <= 400+150)
		{
			//PlaySoundMem(_seClick, DX_PLAYTYPE_BACK, true);
			//DeleteSoundMem(_titleBGM);
			if (CheckSoundMem(_seClick) == 0)
			{
				PlaySoundMem(_seClick, DX_PLAYTYPE_BACK, true);
				_seFlg = true;
			}
			//return std::make_unique<SelectScene>();
		}
	}

	_oldMouse = Mouse;
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
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawGraph(250, 400, _startPNG, true);
	ScreenFlip();
}