#pragma once
#include "VECTOR2.h"

class MouseCtl
{
public:
	MouseCtl();
	~MouseCtl();

	bool GetClickTrg(void);			// クリックした瞬間の場合true
	bool GetClicking(void);			// クリック中の場合true
	const VECTOR2& GetPos(void);	// クリック位置の取得
	void UpDate(void);				// マウスの状態更新
private:
	VECTOR2 _pos;					// マウス座標
	char _mouseData;				// 現在のマウスのクリック状態
	char _mouseDataOld;				// 1フレーム前のマウスのクリック状態
};