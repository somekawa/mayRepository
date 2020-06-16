#include <DxLib.h>
#include "MouseCtl.h"

MouseCtl::MouseCtl()
{
	mouseData = 0;
	mouseDataOld = 0;
}

MouseCtl::~MouseCtl()
{
}

bool MouseCtl::GetClickTrg(void)
{
	//マウスの左ボタンが押されていたら
	if (mouseData & MOUSE_INPUT_LEFT && !(mouseDataOld & MOUSE_INPUT_LEFT))
	{	
		return true;
	}

	return false;
}

bool MouseCtl::GetClicking(void)
{
	//マウスの左ボタンが押されっぱなし
	if (mouseData & MOUSE_INPUT_LEFT && (mouseDataOld & MOUSE_INPUT_LEFT))
	{
		return true;
	}

	return false;
}

const VECTOR2& MouseCtl::GetPos(void)
{
	return pos;
}

void MouseCtl::UpDate(void)
{
	// マウスの座標取得
	GetMousePoint(&pos.x, &pos.y);

	// 1フレーム前の情報を入れる
	mouseDataOld = mouseData;

	// マウスのクリック状態取得
	mouseData = GetMouseInput();
}