#pragma once
#include "BaseScene.h"
class SelectScene :
	public BaseScene
{
public:
	SelectScene();
	~SelectScene();
	// unique_Base own : 自分のSceneポインタ ,  const GameCtl &ctl : GameCtlの読み取り専用
	unique_Base Update(unique_Base own, const GameCtl& ctl);	// セーブ/ロード処理
private:
	bool Init(void);					// 初期化
	void Draw(void);					// 描画

	int _oldMouse;
	int _titleBackPNG;		//「タイトルへ」の文字画像
	int _backPNG;			// 背景画像
	int _normalPNG;			
	int _hardPNG;

	int _pngLight;						// 画像の明るさ調整用変数	
	bool _lightFlg;						// 画像の明るさ調整用フラグ
};