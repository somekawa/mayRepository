#include <Dxlib.h>
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
	// �A�C�e���t�@�C����ǂݍ���
	const auto FileHandle_item = FileRead_open("csv/itemData.csv");
	if (FileHandle_item == NULL)
	{
		return; //�G���[���̏���
	}

	const std::string img = "image/item/";
	const std::string png = ".png";

	for (int i = 0; i <= static_cast<int>(ITEM::MAX) - 2; i++)
	{
		// �摜��,����,��p
		FileRead_scanf(FileHandle_item, "%[^,],%[^,],%d", _itemStruct[i].name, _itemStruct[i].explanation, &_itemStruct[i].itemCostMoney);
		const auto imgName = img + _itemStruct[i].name + png;
		_itemStruct[i].png = LoadGraph(imgName.c_str());
	}

	//�t�@�C�������
	FileRead_close(FileHandle_item);

	_itemBoxSize = { 100,100 };

	// ���l�̎������ʒu
	for (int i = 0; i <= 7; i++)
	{
		_itemPos[i] = { ( ((i % 3) + 2) * _itemBoxSize.x ) + ( _itemBoxSize.x - (_itemBoxSize.x / 10) ),
						  ((i / 3) + 1) * _itemBoxSize.y };
	}
	// �A�C�e�����̈�v
	for (int i = 0; i < static_cast<int>(ITEM::POTION_BIG); i++)
	{
		_itemPair[i].first = _itemStruct[i].png;
		_itemPair[i].second = static_cast<ITEM>(static_cast<int>(ITEM::POTION) + i);
	}
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
	_itemPos[num] = { ( ((num % 3) + 2) * _itemBoxSize.x ) + ( _itemBoxSize.x - (_itemBoxSize.x / 10) ),
					    ((num / 3) + 1) * _itemBoxSize.y };
}