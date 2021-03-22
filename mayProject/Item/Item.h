#pragma once
#include "ITEMCLASS.h"
#include "../Common/VECTOR2.h"

// アイテム情報の構造体
struct itemSt {
	char name[256];			// 名前
	char explanation[256];	// 効果の説明
	int  png;				// 画像
	int itemCostMoney;		// 金額
};

class Item
{
public:
	Item();
	~Item();
	char* GetExplanation(const int& num);				// 指定されたアイテムの説明を取得する
	int GetCostMoney(const int& num);					// 指定されたアイテムの値段を取得する
	std::pair<int, ITEM> GetPair(const int& num);		// 指定された_itemPairの情報を取得する
	VECTOR2 GetPos(const int& num);						// アイテムの座標を取得する
	void SetPos(const int& num);						// アイテムの座標を設定する
private:
	void Init(void);											// 初期化
	itemSt _itemStruct[static_cast<int>(ITEM::MAX)];			// 構造体変数
	std::pair<int, ITEM>_itemPair[static_cast<int>(ITEM::MAX)];	// アイテム画像一致用
	VECTOR2 _itemPos[12];										// アイテム設置場所設定用
	VECTOR2 _itemBoxSize;										// アイテムボックスのサイズ
};