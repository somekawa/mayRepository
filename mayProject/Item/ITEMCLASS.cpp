#include <type_traits>
#include "ITEMCLASS.h"

ITEM begin(ITEM)
{
	return ITEM::POTION;
}

ITEM end(ITEM)
{
	return ITEM::MAX;		
}

ITEM operator++(ITEM& item)
{
	return item = ITEM(std::underlying_type< ITEM >::type(item) + 1);
}

ITEM operator*(ITEM& item)
{
	return item;
}