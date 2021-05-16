#pragma once
#include <map>
#include <string>
#include <memory>
#include "BaseScene.h"

class MouseCtl;

class GameOverScene :
	public BaseScene
{
public:
	GameOverScene();
	~GameOverScene();
	unique_Base Update(unique_Base own, const GameCtl& ctl) override;	
private:
	bool Init(void) override;		// 初期化
	void Draw(void) override;		// 描画
	void PngInit(void);				// 画像用初期化

	std::map<std::string, int> _drawHandle;	// 描画画像

	VECTOR2 _btnPos;		// ボタンの描画位置
	VECTOR2 _btnSize;		// ボタンの大きさ
	int _pngBlend;			// αブレンドで使用
	
	int _seClick;			// クリック音
	bool _seFlg;			// SE用フラグ
	int _overBGM;			// BGM

	std::unique_ptr<MouseCtl> _mouse;		// マウス
};