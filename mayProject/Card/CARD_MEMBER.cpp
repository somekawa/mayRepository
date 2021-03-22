#include "CARD_MEMBER.h"
#include <type_traits>

CARD_MEMBER begin(CARD_MEMBER)
{
	return CARD_MEMBER::ATTACK_1;
}

CARD_MEMBER end(CARD_MEMBER)
{
	return CARD_MEMBER::MAX;		
}

CARD_MEMBER operator++(CARD_MEMBER& mode)
{
	return mode = CARD_MEMBER(std::underlying_type< CARD_MEMBER >::type(mode) + 1);
}

CARD_MEMBER operator*(CARD_MEMBER& mode)
{
	return mode;
}