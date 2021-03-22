#pragma once
#include <map>
#include <string>
#include <memory>
#include "BaseScene.h"
#include "../Common/VECTOR2.h"

class MouseCtl;

class TitleScene :
	public BaseScene
{
public:
	TitleScene();
	~TitleScene();
	unique_Base Update(unique_Base own, const GameCtl& ctl) override;
	static int titleBGM;				// BGM
private:
	bool Init(void) override;			// 初期化
	void PngInit(void);					// 画像用初期化
	void Draw(void) override;			// 描画

	std::map<std::string, int> _drawHandle;	// 描画画像

	VECTOR2 _startPngSize;				// START文字画像のサイズ
	int _pngLight;						// 画像の明るさ調整用変数	
	bool _lightFlg;						// 画像の明るさ調整用フラグ

	int _seClick;						// クリック音
	bool _seFlg;						// SE用フラグ

	std::unique_ptr<MouseCtl> _mouse;	// マウス
};