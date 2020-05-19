#include "Dxlib.h"
#include <string>
#include "Item.h"

Item::Item()
{
	Init();
	// ����؂�̕���
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

	//// SWORD��SHIELD�̂Ƃ��ɁA�i�s�x���݂āA��[���鋭����ύX����?
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
	//	// ���B�x�ȉ��̂Ƃ��͏�1������悤�ɂ���
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
	// �A�C�e���֌W
	//�t�@�C����ǂݍ���
	auto FileHandle_item = FileRead_open("csv/itemData.csv");
	if (FileHandle_item == NULL)
	{
		return; //�G���[���̏���
	}

	std::string a = "image/";
	std::string b = ".png";

	for (int i = 0; i <= static_cast<int>(ITEM::MAX) - 2; i++)
	{
		// �摜�� ���� ��p
		FileRead_scanf(FileHandle_item, "%[^,],%[^,],%d", _itemStruct[i].name, _itemStruct[i].setumei, &_itemStruct[i].itemCostMoney);
		auto d = a + _itemStruct[i].name + b;
		_itemStruct[i].png = LoadGraph(d.c_str());
	}

	//�t�@�C�������
	FileRead_close(FileHandle_item);

	// ����ŁA_itemStruct��potion�Ȃ炱�̉摜�Ƃ����̂��ł���B�͂��B
	//item_pair[0].first = itempotionPNG;
	//item_pair[0].second = ITEM::POTION;
	//_itemPos[0] = { 290,100 };
	//
	//item_pair[1].first = itempotionPNG;
	//item_pair[1].second = ITEM::POTION;
	//_itemPos[1] = { 390,100 };
	//
	// x : 234234������
	//0%3=0+2=2
	//1%3=1+2=3
	//2%3=2+2=4
	//3%3=0+2
	//4%3=1+2
	// y : 111222333������
	//0/3=0+1=1
	//1/3=0+1=1
	//2/3=0+1=1
	//3/3=1+1=2

	// ����ŁA_itemStruct��potion�Ȃ炱�̉摜�Ƃ����̂��ł���B�͂��B
	// ���l�̎������ʒu(�|�[�V�����`��2)
	for (int i = 0; i <= 7; i++)
	{
		//item_pair[i].first = _itemStruct[i].png;
		//item_pair[i].second = static_cast<ITEM>(static_cast<int>(ITEM::POTION) + i);
		_itemPos[i] = { (((i % 3) + 2) * 100) + 90,((i / 3) + 1) * 100 };
	}
	// ���l�̎������A�C�e��(�|�[�V�����`��2)
	for (int i = 0; i <= 11; i++)
	{
		item_pair[i].first = _itemStruct[i].png;
		item_pair[i].second = static_cast<ITEM>(static_cast<int>(ITEM::POTION) + i);
	}

	//// �������ꕔ������(���݂͏�1)
	//item_pair[4].first = _itemStruct[5].png;
	//item_pair[4].second = ITEM::SHIELD;
	//_itemPos[4] = { (((4 % 3) + 2) * 100) + 90,((4 / 3) + 1) * 100 };
	//
	//// ��2
	//item_pair[5].first = _itemStruct[4].png;
	//item_pair[5].second = ITEM::SWORD_LV2;
	//
	//// ��2
	//item_pair[6].first = _itemStruct[6].png;
	//item_pair[6].second = ITEM::SHIELD_LV2;

	// �h���b�v�A�C�e��(�G�E��)
	for (int i = 0; i <= 4; i++)
	{
		item_pair[i+12].first = _itemStruct[i+12].png;
		item_pair[i+12].second = static_cast<ITEM>(static_cast<int>(ITEM::ENEMY_1) + i);
	}

	//// �G�̍��I��
	//item_pair[7].first = _itemStruct[7].png;
	//item_pair[7].second = ITEM::ENEMY_1;
	//// �G�̍��I��2
	//item_pair[8].first = _itemStruct[8].png;
	//item_pair[8].second = ITEM::ENEMY_2;
	//// �󔠂���̉񕜃A�C�e��
	//item_pair[9].first = _itemStruct[9].png;
	//item_pair[9].second = ITEM::POTION_BIG;
}