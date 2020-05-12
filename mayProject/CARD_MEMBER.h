#pragma once
enum class CARD_MEMBER
{
	NON,
	ATTACK_1,
	ATTACK_2,
	ATTACK_3,
	GUARD_1,
	GUARD_2,
	GUARD_3,
	HEAL_1,
	HEAL_2,
	HEAL_3,
	MAX
};

// éÌóﬁÇæÇØímÇËÇΩÇ¢éûóp
enum class CARDS_SYURUI {
	NON,
	ATTACK,
	GUARD,
	HEAL
};

CARD_MEMBER begin(CARD_MEMBER);
CARD_MEMBER end(CARD_MEMBER);
CARD_MEMBER operator++(CARD_MEMBER& mode);
CARD_MEMBER operator* (CARD_MEMBER& mode);

// enum class FIRE_DIR:int {		
// :ÅZÅZÅZÇ≈å^ÇïœÇ¶ÇÁÇÍÇ‹Ç∑
// èëÇ©Ç»Ç©Ç¡ÇΩÇÁintÇ…Ç»ÇËÇ‹Ç∑