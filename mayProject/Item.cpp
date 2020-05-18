#include "Dxlib.h"
#include <string>
#include "Item.h"

Item::Item()
{
	Init();
	// 売り切れの文字
	std::string out = "image/soldout.png";
	_soldoutPNG = LoadGraph(out.c_str());
}

Item::~Item()
{
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

void Item::SetHojuPair(int num, int noweventnum)
{
	item_pair[num].first = _itemStruct[num].png;
	item_pair[num].second = static_cast<ITEM>(static_cast<int>(ITEM::POTION) + num);

	//// SWORDとSHIELDのときに、進行度をみて、補充する強さを変更する?
	//if (noweventnum >= 3)
	//{
	//	if (item_pair[num].second == ITEM::SWORD)
	//	{
	//		item_pair[num].second = ITEM::SWORD_LV2;
	//	}
	//	if (item_pair[num].second == ITEM::SHIELD)
	//	{
	//		item_pair[num].second = ITEM::SHIELD_LV2;
	//	}
	//}
	//else
	//{
	//	// 到達度以下のときは盾1が入るようにする
	//	if (item_pair[num].second == ITEM::SWORD_LV2)
	//	{
	//		item_pair[num].second = ITEM::SHIELD;
	//		item_pair[num].first = _itemStruct[5].png;
	//	}
	//}
}

void Item::SetSoldOutPair(int num)
{
	item_pair[num].first = _soldoutPNG;
	item_pair[num].second = ITEM::NON;
}

VECTOR2 Item::GetPos(int num)
{
	return _itemPos[num];
}

void Item::SetPos(int num)
{
	_itemPos[num] = { (((num % 3) + 2) * 100) + 90,((num / 3) + 1) * 100 };
}

void Item::Init(void)
{
	// アイテム関係
	//ファイルを読み込む
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

	// これで、_itemStructがpotionならこの画像というのができる。はず。
	//item_pair[0].first = itempotionPNG;
	//item_pair[0].second = ITEM::POTION;
	//_itemPos[0] = { 290,100 };
	//
	//item_pair[1].first = itempotionPNG;
	//item_pair[1].second = ITEM::POTION;
	//_itemPos[1] = { 390,100 };
	//
	// x : 234234をつくる
	//0%3=0+2=2
	//1%3=1+2=3
	//2%3=2+2=4
	//3%3=0+2
	//4%3=1+2
	// y : 111222333をつくる
	//0/3=0+1=1
	//1/3=0+1=1
	//2/3=0+1=1
	//3/3=1+1=2

	// これで、_itemStructがpotionならこの画像というのができる。はず。
	// 商人の持ち物位置(ポーション〜盾2)
	for (int i = 0; i <= 7; i++)
	{
		//item_pair[i].first = _itemStruct[i].png;
		//item_pair[i].second = static_cast<ITEM>(static_cast<int>(ITEM::POTION) + i);
		_itemPos[i] = { (((i % 3) + 2) * 100) + 90,((i / 3) + 1) * 100 };
	}
	// 商人の持ち物アイテム(ポーション〜盾2)
	for (int i = 0; i <= 10; i++)
	{
		item_pair[i].first = _itemStruct[i].png;
		item_pair[i].second = static_cast<ITEM>(static_cast<int>(ITEM::POTION) + i);
	}

	//// 持ち物一部こっち(現在は盾1)
	//item_pair[4].first = _itemStruct[5].png;
	//item_pair[4].second = ITEM::SHIELD;
	//_itemPos[4] = { (((4 % 3) + 2) * 100) + 90,((4 / 3) + 1) * 100 };
	//
	//// 剣2
	//item_pair[5].first = _itemStruct[4].png;
	//item_pair[5].second = ITEM::SWORD_LV2;
	//
	//// 盾2
	//item_pair[6].first = _itemStruct[6].png;
	//item_pair[6].second = ITEM::SHIELD_LV2;

	// ドロップアイテム(敵・宝箱)
	for (int i = 0; i <= 3; i++)
	{
		item_pair[i+11].first = _itemStruct[i+11].png;
		item_pair[i+11].second = static_cast<ITEM>(static_cast<int>(ITEM::ENEMY_1) + i);
	}

	//// 敵の魂的な
	//item_pair[7].first = _itemStruct[7].png;
	//item_pair[7].second = ITEM::ENEMY_1;
	//// 敵の魂的な2
	//item_pair[8].first = _itemStruct[8].png;
	//item_pair[8].second = ITEM::ENEMY_2;
	//// 宝箱からの回復アイテム
	//item_pair[9].first = _itemStruct[9].png;
	//item_pair[9].second = ITEM::POTION_BIG;
}