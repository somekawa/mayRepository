#pragma once
#include "VECTOR2.h"
#include "BaseScene.h"

class GameClearScene :
	public BaseScene
{
public:
	GameClearScene();
	~GameClearScene();
	// unique_Base own : 自分のSceneポインタ ,  const GameCtl &ctl : GameCtlの読み取り専用
	unique_Base Update(unique_Base own, const GameCtl& ctl);	// セーブ/ロード処理
private:
	bool Init(void);					// 初期化
	void Draw(void);					// 描画

	int _gameClearPNG;					//「Game Clear」の文字画像
	int _backTitleButtonPNG;			//「タイトルへ」の文字画像
	int _backPNG;						// 背景画像

	int _pngBlend;						// 加算ブレンドで使用

	int _seClick;						// クリック音
	int _clearBGM;						// BGM

	bool flag = false;

	int whitePNG;
	int partestPNG;

	void parTest(void);
	VECTOR2 parPos[3];
	float parEx[3];
	bool parExFlg[3];
	int _pngLight = 50;
	bool posRandFlg[3];

	//int oldMouse;
};