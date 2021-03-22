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
	// ｼｽﾃﾑ処理
	SetGraphMode(screen_sizeX, screen_sizeY, 16);		// 65536色ﾓｰﾄﾞに設定
	ChangeWindowMode(true);								// true:window　false:ﾌﾙｽｸﾘｰﾝ
	SetWindowText("CARDS MAZE");						// ゲームタイトル
	if (DxLib_Init() == -1) return false;				// DXﾗｲﾌﾞﾗﾘ初期化処理	
	SetDrawScreen(DX_SCREEN_BACK);						// ひとまずﾊﾞｯｸﾊﾞｯﾌｧに描画
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
		// gameCtl活用してない
		gameCtl->Update();
		activeScene = activeScene->Update(std::move(activeScene), *gameCtl);
	}
}