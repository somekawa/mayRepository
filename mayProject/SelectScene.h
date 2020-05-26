#pragma once
#include "BaseScene.h"

 //難易度選択(数字はイベント総数。csvの数字量(-1まで含めた)と一致させる)
enum class MODE {
	NON = 0,
	NORMAL = 26,
	HARD = 41,
};

class SelectScene :
	public BaseScene
{
public:
	SelectScene();
	~SelectScene();
	// unique_Base own : 自分のSceneポインタ ,  const GameCtl &ctl : GameCtlの読み取り専用
	unique_Base Update(unique_Base own, const GameCtl& ctl);	
	static MODE modeTest;
private:
	bool Init(void);					// 初期化
	void pngInit(void);					// 画像用初期化
	void Draw(void);					// 描画

	int _oldMouse;						// 1フレーム前のクリック情報保存用

	int _titleBackPNG;					//「タイトルへ」の文字画像
	int _backPNG;						// 背景画像
	int _normalPNG;						//「NORMAL」の文字画像
	int _hardPNG;						//「HARD」の文字画像

	int _pngLight;						// 画像の明るさ調整用変数	
	bool _lightFlg;						// 画像の明るさ調整用フラグ
	bool _toGameFlg;					// GameSceneへの移行用フラグ
	bool _toTitleFlg;					// TitleSceneへの移行用フラグ
	int _seClick;						// クリック音
};