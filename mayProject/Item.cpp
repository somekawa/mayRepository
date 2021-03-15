#include "Dxlib.h"
#include <string>
#include "Item.h"

Item::Item()
{
	Init();
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

	std::string img = "image/";
	std::string png = ".png";

	for (int i = 0; i <= static_cast<int>(ITEM::MAX) - 2; i++)
	{
		// 画像名 説明 費用
		FileRead_scanf(FileHandle_item, "%[^,],%[^,],%d", _itemStruct[i].name, _itemStruct[i].explanation, &_itemStruct[i].itemCostMoney);
		auto imgName = img + _itemStruct[i].name + png;
		_itemStruct[i].png = LoadGraph(imgName.c_str());
	}

	//ファイルを閉じる
	FileRead_close(FileHandle_item);

	_itemBoxSize = { 100,100 };

	// 商人の持ち物位置
	for (int i = 0; i <= 7; i++)
	{
		_itemPos[i] = { (((i % 3) + 2) * _itemBoxSize.x) + (_itemBoxSize.x - (_itemBoxSize.x / 10)),((i / 3) + 1) * _itemBoxSize.y };
	}
	// 商人の持ち物アイテム
	for (int i = 0; i <= 15; i++)
	{
		_itemPair[i].first = _itemStruct[i].png;
		_itemPair[i].second = static_cast<ITEM>(static_cast<int>(ITEM::POTION) + i);
	}

	// ドロップアイテム(敵・宝箱)
	//for (int i = 0; i <= 4; i++)
	//{
	//	_itemPair[i+12].first = _itemStruct[i+12].png;
	//	_itemPair[i+12].second = static_cast<ITEM>(static_cast<int>(ITEM::ENEMY_1) + i);
	//}

	_itemPair[16].first = _itemStruct[16].png;
	_itemPair[16].second = ITEM::POTION_BIG;
}

char* Item::GetExplanation(const int& num)
{
	return _itemStruct[num].explanation;
}

int Item::GetCostMoney(const int& num)
{
	return _itemStruct[num].itemCostMoney;
}

std::pair<int, ITEM> Item::GetPair(const int& num)
{
	return _itemPair[num];
}

VECTOR2 Item::GetPos(const int& num)
{
	return _itemPos[num];
}

void Item::SetPos(const int& num)
{
	_itemPos[num] = { (((num % 3) + 2) * _itemBoxSize.x) + (_itemBoxSize.x - (_itemBoxSize.x / 10)),((num / 3) + 1) * _itemBoxSize.y };
}