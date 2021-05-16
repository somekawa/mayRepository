#include <Dxlib.h>
#include <string>
#include "TitleScene.h"
#include "GameClearScene.h"
#include "../Common/MouseCtl.h"

#define PARTICLE_NUM 3

// パーティクル構造体
struct particle
{
	VECTOR2 parPos;		// 座標
	VECTOR2 parSize;	// 大きさ
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
	_mouse = std::make_unique<MouseCtl>();

	PngInit();

	float expand = 0.0f;
	for (int i = 0; i < PARTICLE_NUM; i++)
	{
		particle_status[i].parEx = expand;
		particle_status[i].parExFlg   = false;
		particle_status[i].posRandFlg = false;
		particle_status[i].parPos  = { -100,-100 };
		particle_status[i].parSize = {  25,25 };
		particle_status[i].light   = 50;
		expand += 0.3f;
	}

	_btnPos  = { 650,500 };
	_btnSize = { 200,100 };
	_pngBlend  = 256;
	_parOffset = { 0,300 };
	_seFlg = false;
	_seClick = LoadSoundMem("sound/se/click.mp3");

	_clearBGM = LoadSoundMem("sound/bgm/clear.mp3");
	PlaySoundMem(_clearBGM, DX_PLAYTYPE_LOOP, true);
	return true;
}

void GameClearScene::PngInit(void)
{
	_drawHandle.try_emplace("particle" , LoadGraph("image/particle.png"));
	_drawHandle.try_emplace("white"    , LoadGraph("image/white.png"));
	_drawHandle.try_emplace("gameclear", LoadGraph("image/gameclear.png"));
	_drawHandle.try_emplace("night_forest"   , LoadGraph("image/night_forest.png"));
	_drawHandle.try_emplace("titleBackButton", LoadGraph("image/titleBackButton.png"));
}

unique_Base GameClearScene::Update(unique_Base own, const GameCtl& ctl)
{
	_mouse->UpDate();

	// クリック時にBGMを消して、効果音を鳴らす
	if (_mouse->GetClickTrg())
	{			
		if (_mouse->GetPos().x >= _btnPos.x && _mouse->GetPos().x <= _btnPos.x + _btnSize.x &&
			_mouse->GetPos().y >= _btnPos.y && _mouse->GetPos().y <= _btnPos.y + _btnSize.y)
		{
			DeleteSoundMem(_clearBGM);
			if (CheckSoundMem(_seClick) == 0)
			{
				PlaySoundMem(_seClick, DX_PLAYTYPE_BACK, true);
				_seFlg = true;
			}
		}
	}

	// 効果音終了後にタイトルシーンへ戻る
	if (_seFlg && (CheckSoundMem(_seClick) == 0))
	{
		DeleteSoundMem(_seClick);
		return std::make_unique<TitleScene>();
	}

	// 画面をだんだん暗くする
	if (_pngBlend >= 0)
	{
		_pngBlend--;
	}

	Particle();
	Draw();

	// 自分のSceneのユニークポインタを返す
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
		DrawRotaGraph(particle_status[i].parPos.x + particle_status[i].parSize.x, particle_status[i].parPos.y + particle_status[i].parSize.y, particle_status[i].parEx, 0, _drawHandle["particle"], true);
	}
	// 描画ブレンドモードをノーブレンドにする
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	ScreenFlip();
}

void GameClearScene::Particle(void)
{
	for (int i = 0; i < PARTICLE_NUM; i++)
	{
		// パーティクルの拡大/縮小の処理
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

		// 新しい場所にパーティクルを移動させる
		if (particle_status[i].posRandFlg)
		{
			particle_status[i].parPos.x = GetRand(850) + _parOffset.x;
			particle_status[i].parPos.y = GetRand(200) + _parOffset.y;
			particle_status[i].posRandFlg = false;
		}
	}
}