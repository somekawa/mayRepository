#include "DxLib.h"
#include "Player.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "Menu.h"
#include "SelectScene.h"
#include "MouseCtl.h"

// static変数の実体<型>クラス名::変数名 = 初期化;
MODE SelectScene::modeTest = MODE::NON;
bool SelectScene::pushFlg = false;

#define PI 3.141592653589793f

SelectScene::SelectScene()
{
	Init();
}

SelectScene::~SelectScene()
{
	delete mouse;
}

bool SelectScene::Init(void)
{
	mouse = new MouseCtl();

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
	_drawHandle.try_emplace("normal", LoadGraph("image/normal.png"));
	_drawHandle.try_emplace("hard", LoadGraph("image/hard.png"));
	_drawHandle.try_emplace("renga", LoadGraph("image/renga.png"));
	_drawHandle.try_emplace("titleBackButton", LoadGraph("image/titleBackButton.png"));

	std::string asiato = "image/asiato.png";
	LoadDivGraph(asiato.c_str(), 2, 1, 2, 69, 190, _asiato);
}

unique_Base SelectScene::Update(unique_Base own, const GameCtl& ctl)
{
	mouse->UpDate();
	if (!pushFlg)
	{
		if (mouse->GetClickTrg())
		{	 
			// 当たり判定(NORMAL選択時)
			if (mouse->GetPos().x >= 250 && mouse->GetPos().x <= 250 + 400 && mouse->GetPos().y >= 100 && mouse->GetPos().y <= 100 + 150)
			{
				DeleteSoundMem(TitleScene::_titleBGM);
				if (CheckSoundMem(_seClick) == 0)
				{
					PlaySoundMem(_seClick, DX_PLAYTYPE_BACK, true);
					_toGameFlg = true;
				}
				modeTest = MODE::NORMAL;
				Menu::Load();
			}
			// 当たり判定(HARD選択時)
			if (mouse->GetPos().x >= 250 && mouse->GetPos().x <= 250 + 400 && mouse->GetPos().y >= 300 && mouse->GetPos().y <= 300 + 150)
			{
				DeleteSoundMem(TitleScene::_titleBGM);
				if (CheckSoundMem(_seClick) == 0)
				{
					PlaySoundMem(_seClick, DX_PLAYTYPE_BACK, true);
					_toGameFlg = true;
				}
				modeTest = MODE::HARD;
				Menu::Load();
			}

			// 当たり判定(タイトルへ戻る)
			if (mouse->GetPos().x >= 650 && mouse->GetPos().x <= 650 + 200 && mouse->GetPos().y >= 450 && mouse->GetPos().y <= 450 + 100)
			{
				if (CheckSoundMem(_seClick) == 0)
				{
					PlaySoundMem(_seClick, DX_PLAYTYPE_BACK, true);
					_toTitleFlg = true;
				}
			}
		}
	}

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

	// 明るさ調整処理
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

	// 足跡用
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
		// 2秒ごとにフラグを反転させる
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
		// 2秒ごとにフラグを反転させる
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

	// 向きと反転
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
			_asiatoRota = static_cast<float>(PI);
			_asiatoReverseX = true;
			_asiatoReverseY = false;
			_drawAsiVec = { 750,0 };
			_startPos.y = 0;
			_goalPos.y = 600;
		}
		else if (_dir == ASIDIR::RIGHT)
		{
			_asiatoRota = static_cast<float>(PI)/2.0f;
			_asiatoReverseX = true;
			_asiatoReverseY = false;
			_startPos.x = 0;
			_goalPos.x = 900;
			_drawAsiVec = { 0,100 };
		}
		else if (_dir == ASIDIR::LEFT)
		{
			_asiatoRota = static_cast<float>(PI) + static_cast<float>(PI) / 2.0f;
			_asiatoReverseX = false;
			_asiatoReverseY = false;
			_startPos.x = -900;
			_goalPos.x = 0;
			_drawAsiVec = { 950,500 };
		}
	}

	// 自分のSceneのユニークポインタを返す 所有権も忘れずに!
	return std::move(own);
}

void SelectScene::Draw(void)
{
	ClsDrawScreen();
	//αブレンド
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pngLight);
	DrawGraph(0, 0, _drawHandle["renga"], true);
	DrawGraph(250, 100, _drawHandle["normal"], true);
	DrawGraph(250, 300, _drawHandle["hard"], true);
	// 足跡
	if (_asiatoFlg)
	{
		DrawRotaGraph(_drawAsiVec.x, _drawAsiVec.y, 0.5f, _asiatoRota, _asiato[0], true, _asiatoReverseX, _asiatoReverseY);
	}
	else
	{
		DrawRotaGraph(_drawAsiVec.x, _drawAsiVec.y, 0.5f, _asiatoRota, _asiato[1], true, _asiatoReverseX, _asiatoReverseY);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawGraph(650, 450, _drawHandle["titleBackButton"], true);
	ScreenFlip();
}