#pragma once

// アイテムの種類
enum class ITEM {
	NON,
	POTION,			// 小回復
	DETOX,			// 解毒剤
	POW_UP,			// 一時的に攻撃力上昇(5ターン)
	HEART,			// 体力増加
	SKILL_FAST,		// プレイヤースキル発動までの時間を早めることができる
	GLASSES,		// 宝箱がトラップかどうか鑑定できる
	SWORD,			// 武器
	SWORD_LV2,		
	SWORD_LV3,
	SHIELD,			// 防具
	SHIELD_LV2,	
	SHIELD_LV3,
	ENEMY_1,		// ドロップアイテムはここから(固定ダメージ20)
	ENEMY_2,		// 次の攻撃のみ無効化
	ENEMY_3,		// 敵ターンを最大値まで巻き戻し
	ENEMY_4,		// 戦闘から確実に逃げられる(特定敵イベントとボス以外)
	POTION_BIG,		// 宝箱のみからでてくる大回復
	MAX
};

// enumの順番を並び替えたらcsvも並び替える

ITEM begin(ITEM);
ITEM end(ITEM);
ITEM operator++(ITEM& item);
ITEM operator* (ITEM& item);