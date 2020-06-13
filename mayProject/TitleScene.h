#pragma once
#include "BaseScene.h"
#include "VECTOR2.h"

class TitleScene :
	public BaseScene
{
public:
	TitleScene();
	~TitleScene();
	// unique_Base own : 自分のSceneポインタ ,  const GameCtl &ctl : GameCtlの読み取り専用
	unique_Base Update(unique_Base own, const GameCtl& ctl);	// セーブ/ロード処理
	static int _titleBGM;				// BGM
private:
	bool Init(void);					// 初期化
	void PngInit(void);					// 画像用初期化
	void Draw(void);					// 描画

	int _titlePNG;						// タイトル画像
	int _startPNG;						//「START」の文字画像

	int _pngLight;						// 画像の明るさ調整用変数	
	bool _lightFlg;						// 画像の明るさ調整用フラグ

	// ゲームシーンからタイトルへ戻ってきたときにボタンの位置が同じせいで、またゲームシーンに入るのを防ぐ
	int _oldMouse;						// 1フレーム前のクリック情報保存用

	int _seClick;						// クリック音
	bool _seFlg;						// SE用フラグ
};