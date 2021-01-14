#include <DxLib.h>
#include "MouseCtl.h"

MouseCtl::MouseCtl()
{
	_mouseData = 0;
	_mouseDataOld = 0;
}

MouseCtl::~MouseCtl()
{
}

bool MouseCtl::GetClickTrg(void)
{
	//マウスの左ボタンが押されていたら
	if (_mouseData & MOUSE_INPUT_LEFT && !(_mouseDataOld & MOUSE_INPUT_LEFT))
	{	
		return true;
	}

	return false;
}

bool MouseCtl::GetClicking(void)
{
	//マウスの左ボタンが押されっぱなし
	if (_mouseData & MOUSE_INPUT_LEFT && (_mouseDataOld & MOUSE_INPUT_LEFT))
	{
		return true;
	}

	return false;
}

const VECTOR2& MouseCtl::GetPos(void)
{
	return _pos;
}

void MouseCtl::UpDate(void)
{
	// マウスの座標取得
	GetMousePoint(&_pos.x, &_pos.y);

	// 1フレーム前の情報を入れる
	_mouseDataOld = _mouseData;

	// マウスのクリック状態取得
	_mouseData = GetMouseInput();
}