#include "DxLib.h"
#include "../Player.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "../Menu.h"
#include "SelectScene.h"
#include "../Common/MouseCtl.h"

// static変数の実体<型>クラス名::変数名 = 初期化;
MODE SelectScene::modeSelect = MODE::NON;
bool SelectScene::pushFlg = false;

#define FOOT_CHANGE_SPEED 120	// 足跡画像の切り替え速度
#define FOOT_MAX 240			// 画面外に足跡が出る範囲の最大値
#define PI 3.141592653589793f

SelectScene::SelectScene()
{
	Init();
}

SelectScene::~SelectScene()
{
}

bool SelectScene::Init(void)
{
	_mouse = std::make_unique<MouseCtl>();

	pngInit();
	_modePngSize = { 400,150 };
	_toTitlePngSize = { 200,100 };
	_pngLight = 128;
	_lightFlg = false;
	_toGameFlg  = false;
	_toTitleFlg = false;
	pushFlg = false;

	_footSpeed = 2;
	_startPos  = { 0,-600 };
	_goalPos   = { 0,0 };
	_drawFootVec = { 100,600 };
	_footPrintsFlg = false;
	_footPrintsReverseX = false;
	_footPrintsReverseY = false;
	_dirFlg = false;
	_footPrintsRota = 0.0f;
	_dir    = FOOTDIR::UP;
	_olddir = FOOTDIR::UP;

	// 予め動きを登録しておく
	func_.try_emplace(FOOTDIR::UP, [&](){
		_footPrintsRota = 0.0f;
		_footPrintsReverseX = false;
		_footPrintsReverseY = false;
		_footPrintsFlg = false;
	});

	func_.try_emplace(FOOTDIR::DOWN, [&]() {
		_footPrintsRota = PI;
		_footPrintsReverseX = true;
		_footPrintsReverseY = false;
		_drawFootVec = { 750,0 };
		_startPos.y  = 0;
		_goalPos.y   = 600;
	});

	func_.try_emplace(FOOTDIR::LEFT, [&]() {
		_footPrintsRota = PI + (PI / 2.0f);
		_footPrintsReverseX = false;
		_footPrintsReverseY = false;
		_startPos.x = -900;
		_goalPos.x  = 0;
		_drawFootVec = { 950,500 };
	});

	func_.try_emplace(FOOTDIR::RIGHT, [&]() {
		_footPrintsRota = PI / 2.0f;
		_footPrintsReverseX = true;
		_footPrintsReverseY = false;
		_startPos.x = 0;
		_goalPos.x  = 900;
		_drawFootVec = { 0,100 };
	});

	_seClick = LoadSoundMem("sound/se/click.mp3");
	return true;
}

void SelectScene::pngInit(void)
{
	_drawHandle.try_emplace("normal", LoadGraph("image/normal.png"));
	_drawHandle.try_emplace("hard", LoadGraph("image/hard.png"));
	_drawHandle.try_emplace("brick", LoadGraph("image/brick.png"));
	_drawHandle.try_emplace("titleBackButton", LoadGraph("image/titleBackButton.png"));

	std::string footsteps = "image/footsteps.png";
	LoadDivGraph(footsteps.c_str(), 2, 1, 2, 69, 190, _footPrints);
}

unique_Base SelectScene::Update(unique_Base own, const GameCtl& ctl)
{
	_mouse->UpDate();
	if (!pushFlg)
	{
		if (_mouse->GetClickTrg())
		{	 
			VECTOR2 offsetPos(_modePngSize.x / 2 + _modePngSize.x / 8, _modePngSize.y - _modePngSize.y / 3);

			// 当たり判定(NORMAL選択時)
			if (_mouse->GetPos().x >= offsetPos.x && _mouse->GetPos().x <= offsetPos.x + _modePngSize.x &&
				_mouse->GetPos().y >= offsetPos.y && _mouse->GetPos().y <= offsetPos.y + _modePngSize.y)
			{
				DeleteSoundMem(TitleScene::titleBGM);
				if (CheckSoundMem(_seClick) == 0)
				{
					PlaySoundMem(_seClick, DX_PLAYTYPE_BACK, true);
					_toGameFlg = true;
				}
				modeSelect = MODE::NORMAL;
				Menu::Load();
			}

			// 当たり判定(HARD選択時)
			if (_mouse->GetPos().x >= offsetPos.x     && _mouse->GetPos().x <= offsetPos.x + _modePngSize.x &&
				_mouse->GetPos().y >= offsetPos.y * 3 && _mouse->GetPos().y <= offsetPos.y * 3 + _modePngSize.y)
			{
				DeleteSoundMem(TitleScene::titleBGM);
				if (CheckSoundMem(_seClick) == 0)
				{
					PlaySoundMem(_seClick, DX_PLAYTYPE_BACK, true);
					_toGameFlg = true;
				}
				modeSelect = MODE::HARD;
				Menu::Load();
			}

			// 当たり判定(タイトルへ戻る)
			if (_mouse->GetPos().x >= offsetPos.x * 3 - _toTitlePngSize.x / 2 && _mouse->GetPos().x <= (offsetPos.x * 3 - _toTitlePngSize.x / 2) + _toTitlePngSize.x &&
				_mouse->GetPos().y >= offsetPos.y * 4 + offsetPos.y / 2       && _mouse->GetPos().y <= (offsetPos.y * 4 + offsetPos.y / 2) + _toTitlePngSize.y)
			{
				if (CheckSoundMem(_seClick) == 0)
				{
					PlaySoundMem(_seClick, DX_PLAYTYPE_BACK, true);
					_toTitleFlg = true;
				}
			}
		}
	}

	// 効果音終了時の処理
	if (CheckSoundMem(_seClick) == 0)
	{
		if (pushFlg && _toGameFlg)
		{
			DeleteSoundMem(_seClick);
			return std::make_unique<GameScene>();
		}
		else if (_toTitleFlg)
		{
			DeleteSoundMem(_seClick);
			return std::make_unique<TitleScene>();
		}
		else
		{
			// 何も処理を行わない
		}
	}

	Draw();

	// 明るさ調整処理
	if (!_lightFlg)
	{
		if (_pngLight <= 255)
		{
			_pngLight++;
			if (_pngLight >= 255)
			{
				_lightFlg = true;
			}
		}
	}
	else
	{
		_pngLight--;
		if (_pngLight <= 128)
		{
			_lightFlg = false;
		}
	}

	// 足跡の移動
	if (_startPos.y <= _goalPos.y + FOOT_MAX)
	{
		_startPos.y += _footSpeed;
	}
	else if (_startPos.x <= _goalPos.x + FOOT_MAX)
	{
		_startPos.x += _footSpeed;
	}
	else
	{
		// 目標位置に到達した時、新たな向きを決める
		_dir = static_cast<FOOTDIR>(GetRand(3));
	}

	if (_dir == FOOTDIR::UP || _dir == FOOTDIR::DOWN)
	{
		// 2秒ごとにフラグを反転させる
		if (_startPos.y % FOOT_CHANGE_SPEED == 0)
		{
			_footPrintsFlg = !_footPrintsFlg;
			_drawFootVec.y = (_dir == FOOTDIR::UP ? -_startPos.y : _startPos.y);
		}
	}

	if (_dir == FOOTDIR::RIGHT || _dir == FOOTDIR::LEFT)
	{
		// 2秒ごとにフラグを反転させる
		if (_startPos.x % FOOT_CHANGE_SPEED == 0)
		{
			_footPrintsFlg = !_footPrintsFlg;
			_drawFootVec.x = (_dir == FOOTDIR::LEFT ? -_startPos.x : _startPos.x);
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
		func_[_dir]();
	}

	// 自分のSceneのユニークポインタを返す
	return std::move(own);
}

void SelectScene::Draw(void)
{
	ClsDrawScreen();
	//αブレンド
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pngLight);
	DrawGraph(0, 0, _drawHandle["brick"], true);
	DrawGraph(_modePngSize.x / 2 + _modePngSize.x / 8, _modePngSize.y - _modePngSize.y / 3, _drawHandle["normal"], true);
	DrawGraph(_modePngSize.x / 2 + _modePngSize.x / 8, _modePngSize.y * 2, _drawHandle["hard"], true);

	// 足跡
	if (_footPrintsFlg)
	{
		DrawRotaGraph(_drawFootVec.x, _drawFootVec.y, 0.5, static_cast<double>(_footPrintsRota), _footPrints[0], true, _footPrintsReverseX, _footPrintsReverseY);
	}
	else
	{
		DrawRotaGraph(_drawFootVec.x, _drawFootVec.y, 0.5, static_cast<double>(_footPrintsRota), _footPrints[1], true, _footPrintsReverseX, _footPrintsReverseY);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawGraph(_toTitlePngSize.x * 3 + _toTitlePngSize.x / 4, _toTitlePngSize.y * 4 + _toTitlePngSize.y / 2, _drawHandle["titleBackButton"], true);
	ScreenFlip();
}