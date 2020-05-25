#include "Dxlib.h"
#include <string>
#include "TitleScene.h"
#include "GameClearScene.h"

GameClearScene::GameClearScene()
{
	Init();
}

GameClearScene::~GameClearScene()
{
}

unique_Base GameClearScene::Update(unique_Base own, const GameCtl& ctl)
{
	// 再生中でなければ再生を行う(0:再生していない)
	// 現在はInitでループ設定している。
	//if (CheckSoundMem(clearBGM) == 0)
	//{
	//	PlaySoundMem(clearBGM, DX_PLAYTYPE_LOOP, true);
	//}

	int x = 0;
	int y = 0;
	auto Mouse = GetMouseInput();                //マウスの入力状態取得
	GetMousePoint(&x, &y);					     //マウスの座標取得
	if (Mouse & MOUSE_INPUT_LEFT/* && !(oldMouse & MOUSE_INPUT_LEFT)*/) {	 //マウスの左ボタンが押されていたら
		// 当たり判定(とりあえず左上のボックスと)
		if (x >= 650 && x <= 650 + 200 && y >= 500 && y <= 500 + 100)
		{
			//PlaySoundMem(_seClick, DX_PLAYTYPE_BACK, true);
			DeleteSoundMem(_clearBGM);
			if (CheckSoundMem(_seClick) == 0)
			{
				PlaySoundMem(_seClick, DX_PLAYTYPE_BACK, true);
				flag = true;
			}
		}
		//return std::make_unique<TitleScene>();
	}

	if (flag)
	{
		if (CheckSoundMem(_seClick) == 0)
		{
			DeleteSoundMem(_seClick);
			return std::make_unique<TitleScene>();
		}
	}

	//oldMouse = mouse;

	// だんだん暗くする
	if (_pngBlend >= 0)
	{
		_pngBlend--;
	}

	parTest();
	Draw();
	// 自分のSceneのユニークポインタを返す 所有権も忘れずに!
	return std::move(own);
}

bool GameClearScene::Init(void)
{
	std::string particle = "image/particle.png";
	partestPNG = LoadGraph(particle.c_str());

	std::string white = "image/white.png";
	whitePNG = LoadGraph(white.c_str());

	std::string gameclear = "image/gameclear.png";
	_gameClearPNG = LoadGraph(gameclear.c_str());

	std::string clear = "image/night_forest.png";
	_backPNG = LoadGraph(clear.c_str());

	std::string titleBackButton = "image/titleBackButton.png";
	_backTitleButtonPNG = LoadGraph(titleBackButton.c_str());

	_pngBlend = 256;	
	float expand = 0.0f;
	for (int i = 0; i < 3; i++)
	{
		parEx[i] = expand;
		parExFlg[i] = false;
		posRandFlg[i] = false;
		parPos[i] = { -100,-100 };
		expand += 0.3f;
	}
	_seClick = LoadSoundMem("sound/se/click.mp3");

	// BGMテスト(データが結構大きめ)
	_clearBGM = LoadSoundMem("sound/bgm/clear.mp3");
	PlaySoundMem(_clearBGM, DX_PLAYTYPE_LOOP, true);
	return true;
}

void GameClearScene::Draw(void)
{
	ClsDrawScreen();
	DrawGraph(0, 0, _backPNG, true);
	DrawGraph(0, -256 + (256 - _pngBlend), _gameClearPNG, true);
	DrawGraph(650, 500, _backTitleButtonPNG, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pngBlend);
	DrawGraph(0, 0, whitePNG, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	// 加算ブレンド
	//SetDrawBlendMode(DX_BLENDMODE_ADD, _pngBlend);
	//DrawGraph(0, 0, _backPNG, true);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//DrawGraph(0, -256 + (256 - _pngBlend), _gameClearPNG, true);


	// 描画ブレンドモードを加算合成にする
	SetDrawBlendMode(DX_BLENDMODE_ADD, _pngLight);
	for (int i = 0; i < 3; i++)
	{
		DrawRotaGraph(parPos[i].x + 25, parPos[i].y + 25, parEx[i], 0, partestPNG, true);
	}
	// 描画ブレンドモードをノーブレンドにする
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	ScreenFlip();
}

void GameClearScene::parTest(void)
{
	for (int i = 0; i < 3; i++)
	{
		if (!parExFlg[i])
		{
			if (parEx[i] <= 0.7f)
			{
				parEx[i] += 0.01f;
			}
			else
			{
				parExFlg[i] = !parExFlg[i];
			}
		}
		else
		{
			if (parEx[i] >= 0.0f)
			{
				parEx[i] -= 0.01f;
			}
			else
			{
				parExFlg[i] = !parExFlg[i];
				posRandFlg[i] = true;
			}
		}

		if (posRandFlg[i])
		{
			parPos[i].x = GetRand(850);
			parPos[i].y = GetRand(200)+300;
			posRandFlg[i] = false;
		}
	}
	//if (!parExFlg[0])
	//{
	//	if (parEx[0] <= 1.0f)
	//	{
	//		parEx[0] += 0.01f;
	//	}
	//	else
	//	{
	//		parExFlg[0] = !parExFlg[0];
	//	}
	//}
	//else
	//{
	//	if (parEx[0] >= 0.0f)
	//	{
	//		parEx[0] -= 0.01f;
	//	}
	//	else
	//	{
	//		parExFlg[0] = !parExFlg[0];
	//		posRandFlg[0] = true;
	//		a = 0;
	//	}
	//}

	//if (posRandFlg[0])
	//{
	//	parPos[0].x = GetRand(850);
	//	parPos[0].y = GetRand(550);
	//	posRandFlg[0] = false;
	//}
}
