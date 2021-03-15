#include "Dxlib.h"
#include <string>
#include "GameOverScene.h"
#include "TitleScene.h"
#include "MouseCtl.h"

GameOverScene::GameOverScene()
{
	Init();
}

GameOverScene::~GameOverScene()
{
}

bool GameOverScene::Init(void)
{
	_mouse = std::make_unique<MouseCtl>();

	PngInit();

	_pngBlend = 0;

	_seClick = LoadSoundMem("sound/se/click.mp3");
	_seFlg = false;

	_overBGM = LoadSoundMem("sound/bgm/over.mp3");
	PlaySoundMem(_overBGM, DX_PLAYTYPE_LOOP, true);
	return true;
}

void GameOverScene::PngInit(void)
{
	_drawHandle.try_emplace("blood", LoadGraph("image/blood.png"));
	_drawHandle.try_emplace("renga", LoadGraph("image/renga.png"));
	_drawHandle.try_emplace("over", LoadGraph("image/over.png"));
	_drawHandle.try_emplace("titleBackButton", LoadGraph("image/titleBackButton.png"));
}

unique_Base GameOverScene::Update(unique_Base own, const GameCtl& ctl)
{
	_mouse->UpDate();
	if (_mouse->GetClickTrg())
	{
		if (_mouse->GetPos().x >= 650 && _mouse->GetPos().x <= 650 + 200 && _mouse->GetPos().y >= 450 && _mouse->GetPos().y <= 450 + 100)
		{
			DeleteSoundMem(_overBGM);
			if (CheckSoundMem(_seClick) == 0)
			{
				PlaySoundMem(_seClick, DX_PLAYTYPE_BACK, true);
				_seFlg = true;
			}
		}
	}

	if (_seFlg && CheckSoundMem(_seClick) == 0)
	{
		DeleteSoundMem(_seClick);
		return std::make_unique<TitleScene>();
	}

	Draw();

	// だんだん明るく
	if (_pngBlend < 255)
	{
		_pngBlend++;
	}

	// 自分のSceneのユニークポインタを返す 所有権も忘れずに!
	return std::move(own);
}

void GameOverScene::Draw(void)
{
	ClsDrawScreen();
	// αブレンド
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawGraph(0, 0, _drawHandle["renga"], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// αブレンド
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pngBlend);
	DrawGraph(0, 0, _drawHandle["blood"], true);
	DrawGraph(200, 250, _drawHandle["over"], true);
	DrawGraph(650, 450, _drawHandle["titleBackButton"], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	ScreenFlip();
}