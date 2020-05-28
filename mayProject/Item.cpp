#include "Dxlib.h"
#include <string>
#include "Item.h"

Item::Item()
{
	Init();
	// 売り切れの文字
	//std::string out = "image/soldout.png";
	//_soldoutPNG = LoadGraph(out.c_str());
}

Item::~Item()
{
}

void Item::Init(void)
{
	// アイテムファイルを読み込む
	auto FileHandle_item = FileRead_open("csv/itemData.csv");
	if (FileHandle_item == NULL)
	{
		return; //エラー時の処理
	}

	std::string a = "image/";
	std::string b = ".png";

	for (int i = 0; i <= static_cast<int>(ITEM::MAX) - 2; i++)
	{
		// 画像名 説明 費用
		FileRead_scanf(FileHandle_item, "%[^,],%[^,],%d", _itemStruct[i].name, _itemStruct[i].setumei, &_itemStruct[i].itemCostMoney);
		auto d = a + _itemStruct[i].name + b;
		_itemStruct[i].png = LoadGraph(d.c_str());
	}

	//ファイルを閉じる
	FileRead_close(FileHandle_item);

	// 商人の持ち物位置
	for (int i = 0; i <= 7; i++)
	{
		_itemPos[i] = { (((i % 3) + 2) * 100) + 90,((i / 3) + 1) * 100 };
	}
	// 商人の持ち物アイテム
	for (int i = 0; i <= 15; i++)
	{
		item_pair[i].first = _itemStruct[i].png;
		item_pair[i].second = static_cast<ITEM>(static_cast<int>(ITEM::POTION) + i);
	}

	// ドロップアイテム(敵・宝箱)
	//for (int i = 0; i <= 4; i++)
	//{
	//	item_pair[i+12].first = _itemStruct[i+12].png;
	//	item_pair[i+12].second = static_cast<ITEM>(static_cast<int>(ITEM::ENEMY_1) + i);
	//}

	item_pair[16].first = _itemStruct[16].png;
	item_pair[16].second = ITEM::POTION_BIG;
}

char* Item::GetSetumei(int num)
{
	return _itemStruct[num].setumei;
}

int Item::GetCostMoney(int num)
{
	return _itemStruct[num].itemCostMoney;
}

std::pair<int, ITEM> Item::GetPair(int num)
{
	return item_pair[num];
}

//void Item::SetHojuPair(int num, int noweventnum)
//{
//	item_pair[num].first = _itemStruct[num].png;
//	item_pair[num].second = static_cast<ITEM>(static_cast<int>(ITEM::POTION) + num);
//
//	//// SWORDとSHIELDのときに、進行度をみて、補充する強さを変更する?
//	//if (noweventnum >= 3)
//	//{
//	//	if (item_pair[num].second == ITEM::SWORD)
//	//	{
//	//		item_pair[num].second = ITEM::SWORD_LV2;
//	//	}
//	//	if (item_pair[num].second == ITEM::SHIELD)
//	//	{
//	//		item_pair[num].second = ITEM::SHIELD_LV2;
//	//	}
//	//}
//	//else
//	//{
//	//	// 到達度以下のときは盾1が入るようにする
//	//	if (item_pair[num].second == ITEM::SWORD_LV2)
//	//	{
//	//		item_pair[num].second = ITEM::SHIELD;
//	//		item_pair[num].first = _itemStruct[5].png;
//	//	}
//	//}
//}
//
//void Item::SetSoldOutPair(int num)
//{
//	item_pair[num].first = _soldoutPNG;
//	item_pair[num].second = ITEM::NON;
//}

VECTOR2 Item::GetPos(int num)
{
	return _itemPos[num];
}

void Item::SetPos(int num)
{
	_itemPos[num] = { (((num % 3) + 2) * 100) + 90,((num / 3) + 1) * 100 };
}