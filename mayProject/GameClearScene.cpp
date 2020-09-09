#include "Dxlib.h"
#include <string>
#include "TitleScene.h"
#include "GameClearScene.h"
#include "MouseCtl.h"

#define PARTICLE_NUM 3

// パーティクル構造体
struct particle
{
	VECTOR2 parPos;		// 座標
	float parEx;		// 拡大率
	bool parExFlg;		// 拡大率切替用フラグ
	bool posRandFlg;	// 座標のランダム用フラグ
	int light;			// 明るさ調整
} particle_status[PARTICLE_NUM];

GameClearScene::GameClearScene()
{
	Init();
}

GameClearScene::~GameClearScene()
{
	delete mouse;
}

bool GameClearScene::Init(void)
{
	mouse = new MouseCtl();

	PngInit();

	float expand = 0.0f;
	for (int i = 0; i < PARTICLE_NUM; i++)
	{
		particle_status[i].parEx = expand;
		particle_status[i].parExFlg = false;
		particle_status[i].posRandFlg = false;
		particle_status[i].parPos = { -100,-100 };
		particle_status[i].light = 50;
		expand += 0.3f;
	}

	_pngBlend = 256;
	_seFlg = false;
	_seClick = LoadSoundMem("sound/se/click.mp3");

	_clearBGM = LoadSoundMem("sound/bgm/clear.mp3");
	PlaySoundMem(_clearBGM, DX_PLAYTYPE_LOOP, true);
	return true;
}

void GameClearScene::PngInit(void)
{
	_drawHandle.try_emplace("particle", LoadGraph("image/particle.png"));
	_drawHandle.try_emplace("white", LoadGraph("image/white.png"));
	_drawHandle.try_emplace("gameclear", LoadGraph("image/gameclear.png"));
	_drawHandle.try_emplace("night_forest", LoadGraph("image/night_forest.png"));
	_drawHandle.try_emplace("titleBackButton", LoadGraph("image/titleBackButton.png"));
}

unique_Base GameClearScene::Update(unique_Base own, const GameCtl& ctl)
{
	mouse->UpDate();
	if (mouse->GetClickTrg()) 
	{			
		if (mouse->GetPos().x >= 650 && mouse->GetPos().x <= 650 + 200 && mouse->GetPos().y >= 500 && mouse->GetPos().y <= 500 + 100)
		{
			DeleteSoundMem(_clearBGM);
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

	// だんだん暗くする
	if (_pngBlend >= 0)
	{
		_pngBlend--;
	}

	Particle();
	Draw();
	// 自分のSceneのユニークポインタを返す 所有権も忘れずに!
	return std::move(own);
}

void GameClearScene::Draw(void)
{
	ClsDrawScreen();
	DrawGraph(0, 0, _drawHandle["night_forest"], true);
	DrawGraph(0, -256 + (256 - _pngBlend), _drawHandle["gameclear"], true);
	DrawGraph(650, 500, _drawHandle["titleBackButton"], true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pngBlend);
	DrawGraph(0, 0, _drawHandle["white"], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 描画ブレンドモードを加算合成にする
	for (int i = 0; i < PARTICLE_NUM; i++)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, particle_status[i].light);
		DrawRotaGraph(particle_status[i].parPos.x + 25, particle_status[i].parPos.y + 25, particle_status[i].parEx, 0, _drawHandle["particle"], true);
	}
	// 描画ブレンドモードをノーブレンドにする
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	ScreenFlip();
}

void GameClearScene::Particle(void)
{
	for (int i = 0; i < PARTICLE_NUM; i++)
	{
		if (!particle_status[i].parExFlg)
		{
			if (particle_status[i].parEx <= 0.7f)
			{
				particle_status[i].light++;
				particle_status[i].parEx += 0.01f;
			}
			else
			{
				particle_status[i].parExFlg = !particle_status[i].parExFlg;
			}
		}
		else
		{
			if (particle_status[i].parEx >= 0.0f)
			{
				particle_status[i].light--;
				particle_status[i].parEx -= 0.01f;
			}
			else
			{
				particle_status[i].parExFlg = !particle_status[i].parExFlg;
				particle_status[i].posRandFlg = true;
			}
		}

		if (particle_status[i].posRandFlg)
		{
			particle_status[i].parPos.x = GetRand(850);
			particle_status[i].parPos.y = GetRand(200)+300;
			particle_status[i].posRandFlg = false;
		}
	}
}