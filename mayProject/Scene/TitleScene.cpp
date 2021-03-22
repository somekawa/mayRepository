#include "DxLib.h"
#include "SceneMng.h"
#include "../Common/GameCtl.h"
#include "SelectScene.h"
#include "TitleScene.h"
#include "../Common/MouseCtl.h"

// static変数の実体<型>クラス名::変数名 = 初期化;
int TitleScene::titleBGM = 0;

TitleScene::TitleScene()
{
	Init();
}

TitleScene::~TitleScene()
{
}

bool TitleScene::Init(void)
{
	_mouse = std::make_unique<MouseCtl>();
	PngInit();
	_startPngSize = { 400,150 };
	_pngLight = 128;
	_lightFlg = false;
	_seFlg = false;
	_seClick = LoadSoundMem("sound/se/click.mp3");
	titleBGM = LoadSoundMem("sound/bgm/title.mp3");
	PlaySoundMem(titleBGM, DX_PLAYTYPE_LOOP, true);
	return true;
}

void TitleScene::PngInit(void)
{
	_drawHandle.try_emplace("title", LoadGraph("image/title.png"));
	_drawHandle.try_emplace("start", LoadGraph("image/start.png"));
}

unique_Base TitleScene::Update(unique_Base own, const GameCtl& ctl)
{
	_mouse->UpDate();
	if (_mouse->GetClickTrg()) {	 //マウスの左ボタンが押されていたら
		// 当たり判定
		VECTOR2 offsetPos = { _startPngSize.x / 2 + _startPngSize.x / 8, _startPngSize.y * 3 - _startPngSize .y / 3};
		if (_mouse->GetPos().x >= offsetPos.x && _mouse->GetPos().x <= offsetPos.x + _startPngSize.x && _mouse->GetPos().y >= offsetPos.y && _mouse->GetPos().y <= offsetPos.y + _startPngSize.y)
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
	
	// 自分のSceneのユニークポインタを返す 所有権も忘れずに!
	return std::move(own);
}

void TitleScene::Draw(void)
{
	ClsDrawScreen();
	//αブレンド
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pngLight);
	DrawGraph(0, 0,_drawHandle["title"], true);
	DrawGraph(_startPngSize.x / 2 + _startPngSize.x / 8, _startPngSize.y * 3 - _startPngSize.y / 3, _drawHandle["start"], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	ScreenFlip();
}