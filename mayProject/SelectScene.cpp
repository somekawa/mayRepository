#include "DxLib.h"
#include "Player.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "Menu.h"
#include "SelectScene.h"

// static�ϐ��̎���<�^>�N���X��::�ϐ��� = ������;
MODE SelectScene::modeTest = MODE::NON;
bool SelectScene::pushFlg = false;

#define PI 3.141592653589793

SelectScene::SelectScene()
{
	Init();
}

SelectScene::~SelectScene()
{
}

bool SelectScene::Init(void)
{
	pngInit();
	_pngLight = 128;
	_lightFlg = false;
	_toGameFlg = false;
	_toTitleFlg = false;
	pushFlg = false;

	_startPos = { 0,-600 };
	_goalPos = { 0,0 };
	_drawAsiVec = { 100,600 };
	_asiatoFlg = false;
	_asiatoReverseX = false;
	_asiatoReverseY = false;
	_dirFlg = false;
	_asiatoRota = 0.0f;
	_dir = ASIDIR::UP;
	_olddir = ASIDIR::UP;

	_seClick = LoadSoundMem("sound/se/click.mp3");
	return true;
}

void SelectScene::pngInit(void)
{
	std::string normal = "image/normal.png";
	_normalPNG = LoadGraph(normal.c_str());

	std::string hard = "image/hard.png";
	_hardPNG = LoadGraph(hard.c_str());

	std::string renga = "image/renga.png";
	_backPNG = LoadGraph(renga.c_str());

	std::string titleBackButton = "image/titleBackButton.png";
	_titleBackPNG = LoadGraph(titleBackButton.c_str());

	std::string asiato = "image/asiato.png";
	LoadDivGraph(asiato.c_str(), 2, 1, 2, 69, 190, _asiato);
}

unique_Base SelectScene::Update(unique_Base own, const GameCtl& ctl)
{
	int x = 0;
	int y = 0;
	auto Mouse = GetMouseInput();                //�}�E�X�̓��͏�Ԏ擾
	GetMousePoint(&x, &y);					     //�}�E�X�̍��W�擾

	if (!pushFlg)
	{
		if (Mouse & MOUSE_INPUT_LEFT && !(_oldMouse & MOUSE_INPUT_LEFT)) {	 //�}�E�X�̍��{�^����������Ă�����
			// �����蔻��(NORMAL�I����)
			if (x >= 250 && x <= 250 + 400 && y >= 100 && y <= 100 + 150)
			{
				DeleteSoundMem(TitleScene::_titleBGM);
				if (CheckSoundMem(_seClick) == 0)
				{
					PlaySoundMem(_seClick, DX_PLAYTYPE_BACK, true);
					_toGameFlg = true;
				}
				modeTest = MODE::NORMAL;
				Menu::Load();
				//return std::make_unique<GameScene>();
			}
			// �����蔻��(HARD�I����)
			if (x >= 250 && x <= 250 + 400 && y >= 300 && y <= 300 + 150)
			{
				DeleteSoundMem(TitleScene::_titleBGM);
				if (CheckSoundMem(_seClick) == 0)
				{
					PlaySoundMem(_seClick, DX_PLAYTYPE_BACK, true);
					_toGameFlg = true;
				}
				modeTest = MODE::HARD;
				Menu::Load();
				//return std::make_unique<GameScene>();
			}

			// �����蔻��(�^�C�g���֖߂�)
			if (x >= 650 && x <= 650 + 200 && y >= 450 && y <= 450 + 100)
			{
				if (CheckSoundMem(_seClick) == 0)
				{
					PlaySoundMem(_seClick, DX_PLAYTYPE_BACK, true);
					_toTitleFlg = true;
				}
				//return std::make_unique<TitleScene>();
			}
		}
	}

	_oldMouse = Mouse;

	if (pushFlg && _toGameFlg && CheckSoundMem(_seClick) == 0)
	{
		DeleteSoundMem(_seClick);
		return std::make_unique<GameScene>();
	}
	else if (_toTitleFlg && CheckSoundMem(_seClick) == 0)
	{
		DeleteSoundMem(_seClick);
		return std::make_unique<TitleScene>();
	}

	Draw();

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

	// ���՗p
	if (_startPos.y <= _goalPos.y + 240)
	{
		_startPos.y += 2;
	}
	else if (_startPos.x <= _goalPos.x + 240)
	{
		_startPos.x += 2;
	}
	else
	{
		int rand = GetRand(3);
		_dir = static_cast<ASIDIR>(rand);
	}

	if (_dir == ASIDIR::UP || _dir == ASIDIR::DOWN)
	{
		// 2�b���ƂɃt���O�𔽓]������
		if (_startPos.y % 120 == 0)
		{
			_asiatoFlg = !_asiatoFlg;
			if (_dir == ASIDIR::UP)
			{
				_drawAsiVec.y = -_startPos.y;
			}
			else
			{
				_drawAsiVec.y = _startPos.y;
			}
		}
	}

	if (_dir == ASIDIR::RIGHT || _dir == ASIDIR::LEFT)
	{
		// 2�b���ƂɃt���O�𔽓]������
		if (_startPos.x % 120 == 0)
		{
			_asiatoFlg = !_asiatoFlg;
			if (_dir == ASIDIR::LEFT)
			{
				_drawAsiVec.x = -_startPos.x;
			}
			else
			{
				_drawAsiVec.x = _startPos.x;
			}
		}
	}

	if (_olddir != _dir)
	{
		_dirFlg = true;
	}

	_olddir = _dir;

	// �����Ɣ��]
	if (_dirFlg)
	{
		_dirFlg = false;
		if (_dir == ASIDIR::UP)
		{
			_asiatoRota = 0.0f;
			_asiatoReverseX = false;
			_asiatoReverseY = false;
			_asiatoFlg = false;
		}
		else if (_dir == ASIDIR::DOWN)
		{
			_asiatoRota = PI;
			_asiatoReverseX = true;
			_asiatoReverseY = false;
			_drawAsiVec = { 750,0 };
			_startPos.y = 0;
			_goalPos.y = 600;
		}
		else if (_dir == ASIDIR::RIGHT)
		{
			_asiatoRota = PI/2;
			_asiatoReverseX = true;
			_asiatoReverseY = false;
			_startPos.x = 0;
			_goalPos.x = 900;
			_drawAsiVec = { 0,100 };
		}
		else if (_dir == ASIDIR::LEFT)
		{
			_asiatoRota = PI + PI / 2;
			_asiatoReverseX = false;
			_asiatoReverseY = false;
			_startPos.x = -900;
			_goalPos.x = 0;
			_drawAsiVec = { 950,500 };
		}
	}

	// ������Scene�̃��j�[�N�|�C���^��Ԃ� ���L�����Y�ꂸ��!
	return std::move(own);
}

void SelectScene::Draw(void)
{
	ClsDrawScreen();
	//���u�����h
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pngLight);
	DrawGraph(0, 0, _backPNG, true);
	DrawGraph(250, 100, _normalPNG, true);
	DrawGraph(250, 300, _hardPNG, true);
	// ����
	if (_asiatoFlg)
	{
		DrawRotaGraph(_drawAsiVec.x, _drawAsiVec.y, 0.5f, _asiatoRota, _asiato[0], true, _asiatoReverseX, _asiatoReverseY);
	}
	else
	{
		DrawRotaGraph(_drawAsiVec.x, _drawAsiVec.y, 0.5f, _asiatoRota, _asiato[1], true, _asiatoReverseX, _asiatoReverseY);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawGraph(650, 450, _titleBackPNG, true);
	ScreenFlip();
}