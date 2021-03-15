#pragma once
#include <map>
#include <string>
#include <memory>
#include <functional>
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
enum class FOOTDIR {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class SelectScene :
	public BaseScene
{
public:
	SelectScene();
	~SelectScene();
	unique_Base Update(unique_Base own, const GameCtl& ctl);	
	static MODE modeSelect;				// 難易度選択
	static bool pushFlg;				// 難易度選択ボタンを押されたらtrueにする
private:
	bool Init(void);					// 初期化
	void pngInit(void);					// 画像用初期化
	void Draw(void);					// 描画

	std::map<std::string, int> _drawHandle;	// 描画画像
	VECTOR2 _modePngSize;					// 難易度の文字サイズ
	VECTOR2 _toTitlePngSize;				// タイトルへ戻るの文字サイズ

	int _pngLight;						// 画像の明るさ調整用変数	
	bool _lightFlg;						// 画像の明るさ調整用フラグ
	bool _toGameFlg;					// GameSceneへの移行用フラグ
	bool _toTitleFlg;					// TitleSceneへの移行用フラグ
	int _seClick;						// クリック音

	// 足跡
	int _footPrints[2];					// 画像
	int _footSpeed;						// 移動速度
	VECTOR2 _startPos;					// 進行方向に対するスタート地点座標
	VECTOR2 _goalPos;					// 進行方向に対するゴール地点座標
	VECTOR2 _drawFootVec;				// 画像描画座標
	bool _footPrintsFlg;				// 2秒ごとに左右の足跡で切り替える
	bool _footPrintsReverseX;			// 画像X軸反転用
	bool _footPrintsReverseY;			// 画像Y軸反転用
	bool _dirFlg;						// ゴール地点についたときに次の方向を決める
	float _footPrintsRota;				// 画像回転
	FOOTDIR _dir;						// 現在の進行方向
	FOOTDIR _olddir;					// 1つ前の進行方向

	std::map<FOOTDIR, std::function<void(void)>> func_;

	std::unique_ptr<MouseCtl> _mouse;	// マウス
};