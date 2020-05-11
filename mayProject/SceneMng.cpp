#include "DxLib.h"
#include "TitleScene.h"
#include "SceneMng.h"
#include "GameCtl.h"

#define SCREEN_SIZE_X 600					// 画面解像度 横
#define SCREEN_SIZE_Y 500					// 画面解像度 縦

SceneMng::SceneMng()
{
	SysInit();
}

SceneMng::~SceneMng()
{
}

bool SceneMng::SysInit(void)
{
	// ｼｽﾃﾑ処理
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 16);		// 65536色ﾓｰﾄﾞに設定
	ChangeWindowMode(true);								// true:window　false:ﾌﾙｽｸﾘｰﾝ
	SetWindowText("5月制作");					// 学籍番号と名前を入れておく
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