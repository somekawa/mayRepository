#pragma once

// カード内容
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

// 種類だけ知りたい時用
enum class CARDS_TYPE
{
	NON,
	ATTACK,
	GUARD,
	INN
};

CARD_MEMBER begin(CARD_MEMBER);
CARD_MEMBER end(CARD_MEMBER);
CARD_MEMBER operator++(CARD_MEMBER& mode);
CARD_MEMBER operator* (CARD_MEMBER& mode);