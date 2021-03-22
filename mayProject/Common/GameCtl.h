#pragma once
#include <array>

using KEY_ARRAY = std::array<char, 256>; //	arrayを使ったキー処理

// キーの状態
enum KEY_TYPE
{
	KEY_TYPE_NOW,	// 現在の押下情報
	KEY_TYPE_OLD,	// 1フレーム前の押下情報
	KEY_TYPE_MAX
};

class GameCtl
{
public:
	GameCtl();
	~GameCtl();
	bool Update(void);								// 状態を更新する
	const KEY_ARRAY &GetCtl(KEY_TYPE type) const;	// キーの入力状態を返す(末尾のconstは中に書いてあるもの全てconst扱いをするという意味)
private:
	KEY_ARRAY data;									// 現在の情報
	KEY_ARRAY dataOld;								// 1フレーム前の情報
};