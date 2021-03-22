#pragma once
#include "ITEMCLASS.h"
#include "../Common/VECTOR2.h"

// �A�C�e�����̍\����
struct itemSt {
	char name[256];			// ���O
	char explanation[256];	// ���ʂ̐���
	int  png;				// �摜
	int itemCostMoney;		// ���z
};

class Item
{
public:
	Item();
	~Item();
	char* GetExplanation(const int& num);				// �w�肳�ꂽ�A�C�e���̐������擾����
	int GetCostMoney(const int& num);					// �w�肳�ꂽ�A�C�e���̒l�i���擾����
	std::pair<int, ITEM> GetPair(const int& num);		// �w�肳�ꂽ_itemPair�̏����擾����
	VECTOR2 GetPos(const int& num);						// �A�C�e���̍��W���擾����
	void SetPos(const int& num);						// �A�C�e���̍��W��ݒ肷��
private:
	void Init(void);											// ������
	itemSt _itemStruct[static_cast<int>(ITEM::MAX)];			// �\���̕ϐ�
	std::pair<int, ITEM>_itemPair[static_cast<int>(ITEM::MAX)];	// �A�C�e���摜��v�p
	VECTOR2 _itemPos[12];										// �A�C�e���ݒu�ꏊ�ݒ�p
	VECTOR2 _itemBoxSize;										// �A�C�e���{�b�N�X�̃T�C�Y
};