#pragma once
#include <map>
#include <string>
#include <memory>
#include "BaseScene.h"

class MouseCtl;

class GameClearScene :
	public BaseScene
{
public:
	GameClearScene();
	~GameClearScene();
	unique_Base Update(unique_Base own, const GameCtl& ctl) override;
private:
	bool Init(void) override;			// 初期化
	void PngInit(void);					// 画像用初期化
	void Draw(void) override;			// 描画
	void Particle(void);				// パーティクル用関数

	std::map<std::string, int> _drawHandle;	// 描画画像

	VECTOR2 _btnPos;					// ボタンの描画位置
	VECTOR2 _btnSize;					// ボタンの大きさ
	int _pngBlend;						// 加算ブレンドで使用
	VECTOR2 _parOffset;					// パーティクルの描画位置オフセット

	int _seClick;						// クリック音
	int _clearBGM;						// BGM
	bool _seFlg;						// SE用フラグ

	std::unique_ptr<MouseCtl> _mouse;	// マウス
};