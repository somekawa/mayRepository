#pragma once
#include <map>
#include <string>
#include "BaseScene.h"

class MouseCtl;

class GameOverScene :
	public BaseScene
{
public:
	GameOverScene();
	~GameOverScene();
	// unique_Base own : 自分のSceneポインタ ,  const GameCtl &ctl : GameCtlの読み取り専用
	unique_Base Update(unique_Base own, const GameCtl& ctl);	// セーブ/ロード処理
private:
	bool Init(void);		// 初期化
	void Draw(void);		// 描画
	void PngInit(void);		// 画像用初期化

	std::map<std::string, int> _drawHandle;	// 描画画像

	int _pngBlend;			// αブレンドで使用
	
	int _seClick;			// クリック音
	bool _seFlg;			// SE用フラグ
	int _overBGM;			// BGM

	MouseCtl* mouse;
};