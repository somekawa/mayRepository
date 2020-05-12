#include "ITEMCLASS.h"
#include <type_traits>

ITEM begin(ITEM)
{
	return ITEM::POTION;
}

ITEM end(ITEM)
{
	return ITEM::MAX;		// end‚ÍÀÛ‚Ì—v‘f+1‚µ‚È‚¢‚Æ‚¢‚¯‚È‚¢‚Ì‚ÅDOWN‚¶‚á‚È‚¢
}

ITEM operator++(ITEM& item)
{
	// Šî’ê‚ÌŒ^‚ğ’²‚×‚é‚â‚Â
	return item = ITEM(std::underlying_type< ITEM >::type(item) + 1);
}

ITEM operator*(ITEM& item)
{
	return item;
}