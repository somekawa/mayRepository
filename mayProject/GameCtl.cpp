#include "DxLib.h"
#include "GameCtl.h"

GameCtl::GameCtl()
{
}

GameCtl::~GameCtl()
{
}

const KEY_ARRAY & GameCtl::GetCtl(KEY_TYPE type) const // 末尾のconstは中に書いてあるのもすべてconstという意味
{
	if (type == KEY_TYPE_OLD)
	{
		return dataOld;
	}
	return data;					// 参照なので、そのまま返して大丈夫。
}

bool GameCtl::Update(void)
{
	dataOld = data;					// オペレータのオーバーロードされている
	GetHitKeyStateAll(&data[0]);	// dataの先頭にアクセスしたい
	return true;
}