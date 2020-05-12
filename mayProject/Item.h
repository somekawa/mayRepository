#pragma once
#include "ITEMCLASS.h"
#include "VECTOR2.h"

// アイテム情報の構造体
struct itemSt {
	char name[256];		// 名前
	char setumei[256];	// 効果の説明
	int  png;			// 画像
	int itemCostMoney;	// 金額
};

class Item
{
public:
	Item();
	~Item();
	char* GetSetumei(int num);					// 指定されたアイテムの説明を取得する
	int GetCostMoney(int num);					// 指定されたアイテムの値段を取得する
	std::pair<int, ITEM> GetPair(int num);		// 指定されたitem_pairの情報を取得する
	void SetHojuPair(int num,int noweventnum);	// アイテムの補充を行う
	void SetSoldOutPair(int num);				// アイテムの売り切れ設定を行う
	VECTOR2 GetPos(int num);					// アイテムの座標を取得する
	void SetPos(int num);						// アイテムの座標を設定する(補充で使う)
private:
	void Init(void);											// 初期化
	itemSt _itemStruct[static_cast<int>(ITEM::MAX)];			// 構造体変数
	std::pair<int, ITEM>item_pair[static_cast<int>(ITEM::MAX)];	// アイテム画像一致用
	VECTOR2 _itemPos[12];										// アイテム設置場所設定用

	//「Sold Out」の文字画像
	int _soldoutPNG;
};