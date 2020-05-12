#pragma once
#include "ITEMCLASS.h"
#include "VECTOR2.h"

// �A�C�e�����̍\����
struct itemSt {
	char name[256];		// ���O
	char setumei[256];	// ���ʂ̐���
	int  png;			// �摜
	int itemCostMoney;	// ���z
};

class Item
{
public:
	Item();
	~Item();
	char* GetSetumei(int num);					// �w�肳�ꂽ�A�C�e���̐������擾����
	int GetCostMoney(int num);					// �w�肳�ꂽ�A�C�e���̒l�i���擾����
	std::pair<int, ITEM> GetPair(int num);		// �w�肳�ꂽitem_pair�̏����擾����
	void SetHojuPair(int num,int noweventnum);	// �A�C�e���̕�[���s��
	void SetSoldOutPair(int num);				// �A�C�e���̔���؂�ݒ���s��
	VECTOR2 GetPos(int num);					// �A�C�e���̍��W���擾����
	void SetPos(int num);						// �A�C�e���̍��W��ݒ肷��(��[�Ŏg��)
private:
	void Init(void);											// ������
	itemSt _itemStruct[static_cast<int>(ITEM::MAX)];			// �\���̕ϐ�
	std::pair<int, ITEM>item_pair[static_cast<int>(ITEM::MAX)];	// �A�C�e���摜��v�p
	VECTOR2 _itemPos[12];										// �A�C�e���ݒu�ꏊ�ݒ�p

	//�uSold Out�v�̕����摜
	int _soldoutPNG;
};