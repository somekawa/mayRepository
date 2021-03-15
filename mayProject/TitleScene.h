#pragma once
#include <map>
#include <string>
#include <memory>
#include "BaseScene.h"
#include "VECTOR2.h"

class MouseCtl;

class TitleScene :
	public BaseScene
{
public:
	TitleScene();
	~TitleScene();
	// unique_Base own : 自分のSceneポインタ ,  const GameCtl &ctl : GameCtlの読み取り専用
	unique_Base Update(unique_Base own, const GameCtl& ctl);	// セーブ/ロード処理
	static int titleBGM;				// BGM
private:
	bool Init(void);					// 初期化
	void PngInit(void);					// 画像用初期化
	void Draw(void);					// 描画

	std::map<std::string, int> _drawHandle;	// 描画画像

	VECTOR2 _startPngSize;				// START文字画像のサイズ
	int _pngLight;						// 画像の明るさ調整用変数	
	bool _lightFlg;						// 画像の明るさ調整用フラグ

	int _seClick;						// クリック音
	bool _seFlg;						// SE用フラグ

	std::unique_ptr<MouseCtl> _mouse;	// マウス
};