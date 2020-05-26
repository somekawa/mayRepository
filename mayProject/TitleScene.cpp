#include "DxLib.h"
#include "SceneMng.h"
#include "GameCtl.h"
#include "GameClearScene.h"
#include "SelectScene.h"
#include "TitleScene.h"

// static変数の実体<型>クラス名::変数名 = 初期化;
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
	// 再生中でなければ再生を行う(0:再生していない)
	// 現在はInitでループ設定している。
	//if (CheckSoundMem(_titleBGM) == 0)
	//{
	//	PlaySoundMem(_titleBGM, DX_PLAYTYPE_LOOP, true);
	//}

	int x = 0;
	int y = 0;
	auto Mouse = GetMouseInput();                //マウスの入力状態取得
	GetMousePoint(&x, &y);					     //マウスの座標取得
	if (Mouse & MOUSE_INPUT_LEFT && !(_oldMouse & MOUSE_INPUT_LEFT)) {	 //マウスの左ボタンが押されていたら
		// 当たり判定
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
	DrawGraph(0, 0,_titlePNG, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawGraph(250, 400, _startPNG, true);
	ScreenFlip();
}