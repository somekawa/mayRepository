#pragma once

// アイテムの種類
enum class ITEM {
	NON,
	POTION,
	DETOX,
	KYOUKA_POW,		// 一時的に攻撃力上昇(5ターンとか)
	HEART,			// 体力増加
	SWORD,
	SWORD_LV2,		
	SWORD_LV3,
	SHIELD,
	SHIELD_LV2,	
	SHIELD_LV3,
	ENEMY_1,		// ドロップアイテムはここから(固定ダメージ20)
	ENEMY_2,		// 次の攻撃のみ無効化
	POTION_BIG,		// 宝箱のみからでてくる大回復
	MAX
};

// enumの順番を並び替えたらcsvも並び替える

ITEM begin(ITEM);
ITEM end(ITEM);
ITEM operator++(ITEM& item);
ITEM operator* (ITEM& item);