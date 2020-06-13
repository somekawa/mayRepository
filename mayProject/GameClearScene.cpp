#include "Dxlib.h"
#include <string>
#include "TitleScene.h"
#include "GameClearScene.h"

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
}

bool GameClearScene::Init(void)
{
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
	std::string particle = "image/particle.png";
	_particlePNG = LoadGraph(particle.c_str());

	std::string white = "image/white.png";
	_whitePNG = LoadGraph(white.c_str());

	std::string gameclear = "image/gameclear.png";
	_gameClearPNG = LoadGraph(gameclear.c_str());

	std::string clear = "image/night_forest.png";
	_backPNG = LoadGraph(clear.c_str());

	std::string titleBackButton = "image/titleBackButton.png";
	_backTitleButtonPNG = LoadGraph(titleBackButton.c_str());
}

unique_Base GameClearScene::Update(unique_Base own, const GameCtl& ctl)
{
	int x = 0;
	int y = 0;
	auto Mouse = GetMouseInput();                // マウスの入力状態取得
	GetMousePoint(&x, &y);					     // マウスの座標取得
	if (Mouse & MOUSE_INPUT_LEFT) {				 // マウスの左ボタンが押されていたら
		// 当たり判定
		if (x >= 650 && x <= 650 + 200 && y >= 500 && y <= 500 + 100)
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
	DrawGraph(0, 0, _backPNG, true);
	DrawGraph(0, -256 + (256 - _pngBlend), _gameClearPNG, true);
	DrawGraph(650, 500, _backTitleButtonPNG, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pngBlend);
	DrawGraph(0, 0, _whitePNG, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 描画ブレンドモードを加算合成にする
	for (int i = 0; i < PARTICLE_NUM; i++)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, particle_status[i].light);
		DrawRotaGraph(particle_status[i].parPos.x + 25, particle_status[i].parPos.y + 25, particle_status[i].parEx, 0, _particlePNG, true);
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