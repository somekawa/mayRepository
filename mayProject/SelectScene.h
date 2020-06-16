#pragma once
#include "BaseScene.h"
#include "VECTOR2.h"

class MouseCtl;

 //難易度選択
enum class MODE {
	NON ,
	NORMAL,
	HARD,
};

// 足跡の進行方向
enum class ASIDIR {
	UP,
	DOWN,
	RIGHT,
	LEFT
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
	static bool pushFlg;				// 難易度選択ボタンを押されたらtrueにする
private:
	bool Init(void);					// 初期化
	void pngInit(void);					// 画像用初期化
	void Draw(void);					// 描画

	int _titleBackPNG;					//「タイトルへ」の文字画像
	int _backPNG;						// 背景画像
	int _normalPNG;						//「NORMAL」の文字画像
	int _hardPNG;						//「HARD」の文字画像

	int _pngLight;						// 画像の明るさ調整用変数	
	bool _lightFlg;						// 画像の明るさ調整用フラグ
	bool _toGameFlg;					// GameSceneへの移行用フラグ
	bool _toTitleFlg;					// TitleSceneへの移行用フラグ
	int _seClick;						// クリック音

	// 足跡
	int _asiato[2];						// 画像
	VECTOR2 _startPos;					// 進行方向に対するスタート地点座標
	VECTOR2 _goalPos;					// 進行方向に対するゴール地点座標
	VECTOR2 _drawAsiVec;				// 画像描画座標
	bool _asiatoFlg;					// 2秒ごとに左右の足跡で切り替える
	bool _asiatoReverseX;				// 画像X軸反転用
	bool _asiatoReverseY;				// 画像Y軸反転用
	bool _dirFlg;						// ゴール地点についたときに次の方向を決める
	float _asiatoRota;					// 画像回転
	ASIDIR _dir;						// 現在の進行方向
	ASIDIR _olddir;						// 1つ前の進行方向

	MouseCtl* mouse;
};