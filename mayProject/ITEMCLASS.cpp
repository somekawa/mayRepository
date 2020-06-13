#include "ITEMCLASS.h"
#include <type_traits>

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
	// Šî’ê‚ÌŒ^‚ð’²‚×‚é‚â‚Â
	return item = ITEM(std::underlying_type< ITEM >::type(item) + 1);
}

ITEM operator*(ITEM& item)
{
	return item;
}