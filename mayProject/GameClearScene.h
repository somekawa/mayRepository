#pragma once
#include <map>
#include <string>
#include "BaseScene.h"

class MouseCtl;

class GameClearScene :
	public BaseScene
{
public:
	GameClearScene();
	~GameClearScene();
	// unique_Base own : 自分のSceneポインタ ,  const GameCtl &ctl : GameCtlの読み取り専用
	unique_Base Update(unique_Base own, const GameCtl& ctl);
private:
	bool Init(void);					// 初期化
	void PngInit(void);					// 画像用初期化
	void Draw(void);					// 描画
	void Particle(void);				// パーティクル用関数

	std::map<std::string, int> _drawHandle;	// 描画画像

	int _pngBlend;						// 加算ブレンドで使用

	int _seClick;						// クリック音
	int _clearBGM;						// BGM
	bool _seFlg;						// SE用フラグ

	MouseCtl* mouse;
};