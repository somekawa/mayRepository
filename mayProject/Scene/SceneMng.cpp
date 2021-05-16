#include "DxLib.h"
#include "TitleScene.h"
#include "SceneMng.h"
#include "../Common/GameCtl.h"

SceneMng::SceneMng() :screen_sizeX(900), screen_sizeY(600)
{
	SysInit();
}

SceneMng::~SceneMng()
{
}

bool SceneMng::SysInit(void)
{
	// システム処理
	SetGraphMode(screen_sizeX, screen_sizeY, 16);		// 65536色モードに設定
	ChangeWindowMode(true);								// true:window　false:フルスクリーン
	SetWindowText("CARDS MAZE");						// ゲームタイトル

	// DXライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		return false;
	}				
	SetDrawScreen(DX_SCREEN_BACK);						// バックバッファに描画
	gameCtl = std::make_unique<GameCtl>();				// GameCtlのインスタンス
	return true;
}

void SceneMng::Run(void)
{
	// 関数のポインタ経由でアクセス
	// ゲームループ
	activeScene = std::make_unique<TitleScene>();
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		gameCtl->Update();
		activeScene = activeScene->Update(std::move(activeScene), *gameCtl);
	}
}