#include "ITEMCLASS.h"
#include <type_traits>

ITEM begin(ITEM)
{
	return ITEM::POTION;
}

ITEM end(ITEM)
{
	return ITEM::MAX;		// end�͎��ۂ̗v�f+1���Ȃ��Ƃ����Ȃ��̂�DOWN����Ȃ�
}

ITEM operator++(ITEM& item)
{
	// ���̌^�𒲂ׂ���
	return item = ITEM(std::underlying_type< ITEM >::type(item) + 1);
}

ITEM operator*(ITEM& item)
{
	return item;
}